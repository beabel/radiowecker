#include "tft_color_setting.h"
#include "lv_font_clock_digits.h"
#include "00_texte.h"
#include "weather.h"

class AudioGenerator;
extern AudioGenerator *decoder;

extern "C" const lv_font_t lv_font_de_supp_14;

void showStartPage(void);
void showFavoritenPage(void);
void showSettingsPage(void);
void showAlarmPage(void);
void changeStation(void);
void toggleRadio(boolean off);
void startSnooze(void);
void safeAlarmTime(void);
void showAlarms_Day_and_Time(void);

// --- Alarm-Edit (wie zuvor) ---
typedef struct {
  uint8_t alarmday_1 = 0B00111110;
  uint8_t h_1;
  uint8_t m_1;
  uint8_t alarmday_2 = 0B00111110;
  uint8_t h_2;
  uint8_t m_2;
} AlarmEdit;

AlarmEdit alarmConfig;

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
XPT2046_Touchscreen touch(TOUCH_CS, TOUCH_IRQ);

/* Partialspeicher: 11 Zeilen statt 14 spart ~2 KiB .bss (ESP32 DRAM-Grenze) */
static lv_color_t buf1[320 * 11];
static lv_display_t *lv_disp_main;

static lv_obj_t *scr_clock;
static lv_obj_t *scr_radio;
static lv_obj_t *scr_config;
static lv_obj_t *scr_alarm;
static lv_obj_t *scr_msg;
static lv_obj_t *scr_ota;

static lv_obj_t *lbl_date;
/* Uhrzeit: vier Ziffern + Doppelpunkt, eigene 1-bpp-Schrift (nur 0–9 und :) — ohne Kachelrahmen */
static lv_obj_t *clock_time_bar;
static lv_obj_t *clock_digit_cell[4];
static lv_obj_t *clock_digit_lbl[4];
static lv_obj_t *clock_colon;

static lv_obj_t *lbl_alarm_hdr;
static lv_obj_t *lbl_ip;
static lv_obj_t *lbl_rssi;
static lv_obj_t *lbl_snooze;
static lv_obj_t *mid_weather;
static lv_obj_t *mid_radio;
static lv_obj_t *lbl_station_big;
static lv_obj_t *lbl_stream_title;
static lv_obj_t *sl_clock_vol;

static lv_obj_t *w_lbl_now_t;
static lv_obj_t *w_lbl_now_f;
static lv_obj_t *w_lbl_td_min;
static lv_obj_t *w_lbl_td_max;
static lv_obj_t *w_lbl_tm_min;
static lv_obj_t *w_lbl_tm_max;

static lv_obj_t *msg_title;
static lv_obj_t *msg_line2;
static lv_obj_t *msg_line3;
static lv_obj_t *msg_line4;
static lv_obj_t *msg_line5;

static lv_obj_t *ota_lbl;
static lv_obj_t *ota_bar;
static lv_obj_t *ota_sub;

static lv_obj_t *sl_radio_gain;
static lv_obj_t *sl_cfg_gain;
static lv_obj_t *sl_cfg_bright;
static lv_obj_t *sl_cfg_snooze;
static lv_obj_t *lbl_cfg_gain_val;
static lv_obj_t *lbl_cfg_bright_val;
static lv_obj_t *lbl_cfg_snooze_val;

static lv_font_t font_ui_primary;
static const lv_font_t *font_clock_digits; /* lv_font_clock_digits.c — nur Ziffern + Doppelpunkt */
static lv_font_t font_clock_date;          /* großes Datum inkl. Umlaute (Fallback de_supp) */
static lv_font_t font_radio_meta;   /* Sender + Streamtitel größer */
static lv_obj_t *lbl_station_radio;
static lv_obj_t *lbl_station_cfg;

static lv_obj_t *lbl_al_t1;
static lv_obj_t *lbl_al_t2;
static lv_obj_t *al_day_btn[2][7];
static lv_obj_t *fav_btn[10];
static lv_obj_t *fav_lbl[10];
static int fav_map[10];
static int fav_count;

static bool ui_slider_internal;

static char lastdate[48] = "";
static char lasttime[8] = "";

#define UI_FOOTER_STATE_SLOTS 4
static lv_obj_t *g_ft_pwr_btn[UI_FOOTER_STATE_SLOTS];
static lv_obj_t *g_ft_pwr_lbl[UI_FOOTER_STATE_SLOTS];
static lv_obj_t *g_ft_snz_btn[UI_FOOTER_STATE_SLOTS];
static lv_obj_t *g_ft_snz_lbl[UI_FOOTER_STATE_SLOTS];
static lv_obj_t *g_ft_alm_btn[UI_FOOTER_STATE_SLOTS];
static lv_obj_t *g_ft_alm_lbl[UI_FOOTER_STATE_SLOTS];
static uint8_t g_ft_n = 0;

static void ui_footer_icons_reset(void) {
  g_ft_n = 0;
}

static void ui_footer_register_state_row(lv_obj_t *bp, lv_obj_t *lp, lv_obj_t *bz, lv_obj_t *lz, lv_obj_t *ba, lv_obj_t *la) {
  if (g_ft_n >= UI_FOOTER_STATE_SLOTS) return;
  g_ft_pwr_btn[g_ft_n] = bp;
  g_ft_pwr_lbl[g_ft_n] = lp;
  g_ft_snz_btn[g_ft_n] = bz;
  g_ft_snz_lbl[g_ft_n] = lz;
  g_ft_alm_btn[g_ft_n] = ba;
  g_ft_alm_lbl[g_ft_n] = la;
  g_ft_n++;
}

static void ui_refresh_footer_icons(void) {
  const bool alarm_on = (alarmday < 8);
  const bool snooze_active = (snoozeTimeEnd != 0 && millis() < snoozeTimeEnd);
  for (uint8_t i = 0; i < g_ft_n; i++) {
    if (g_ft_pwr_lbl[i]) {
      lv_label_set_text(g_ft_pwr_lbl[i], radio ? LV_SYMBOL_PAUSE : LV_SYMBOL_PLAY);
      if (g_ft_pwr_btn[i])
        lv_obj_set_style_bg_color(g_ft_pwr_btn[i],
                                  radio ? lv_palette_main(LV_PALETTE_ORANGE) : lv_palette_main(LV_PALETTE_BLUE),
                                  LV_PART_MAIN);
    }
    if (g_ft_snz_lbl[i]) {
      lv_label_set_text(g_ft_snz_lbl[i], snooze_active ? LV_SYMBOL_REFRESH : "Zz");
      if (g_ft_snz_btn[i])
        lv_obj_set_style_bg_color(g_ft_snz_btn[i],
                                  snooze_active ? lv_palette_main(LV_PALETTE_ORANGE) : lv_palette_main(LV_PALETTE_BLUE),
                                  LV_PART_MAIN);
    }
    if (g_ft_alm_lbl[i]) {
      lv_label_set_text(g_ft_alm_lbl[i], alarm_on ? LV_SYMBOL_BELL : LV_SYMBOL_CLOSE);
      if (g_ft_alm_btn[i])
        lv_obj_set_style_bg_color(g_ft_alm_btn[i],
                                  alarm_on ? lv_palette_main(LV_PALETTE_ORANGE) : lv_palette_main(LV_PALETTE_BLUE),
                                  LV_PART_MAIN);
    }
  }
}

/* Touch: bei falscher Zuordnung nacheinander probieren: TS_SWAP_XY, TS_INVERT_X/Y */
#ifndef TS_CAL_MIN
#define TS_CAL_MIN 200
#endif
#ifndef TS_CAL_MAX
#define TS_CAL_MAX 3900
#endif
#ifndef TS_SWAP_XY
#define TS_SWAP_XY 0
#endif
#ifndef TS_INVERT_X
#define TS_INVERT_X 0
#endif
#ifndef TS_INVERT_Y
#define TS_INVERT_Y 0
#endif

static lv_coord_t ts_last_x;
static lv_coord_t ts_last_y;

static void ts_map(int16_t rawx, int16_t rawy, lv_coord_t *ox, lv_coord_t *oy) {
  int16_t rx = rawx, ry = rawy;
#if TS_SWAP_XY
  int16_t t = rx;
  rx = ry;
  ry = t;
#endif
  int x = map(constrain(rx, TS_CAL_MIN, TS_CAL_MAX), TS_CAL_MIN, TS_CAL_MAX, 0, tft.width() - 1);
  int y = map(constrain(ry, TS_CAL_MIN, TS_CAL_MAX), TS_CAL_MIN, TS_CAL_MAX, 0, tft.height() - 1);
#if TS_INVERT_X
  x = tft.width() - 1 - x;
#endif
#if TS_INVERT_Y
  y = tft.height() - 1 - y;
#endif
  *ox = (lv_coord_t)x;
  *oy = (lv_coord_t)y;
}

static lv_color_t rgb565_to_lv(uint16_t c) {
  uint8_t r5 = (c >> 11) & 0x1F;
  uint8_t g6 = (c >> 5) & 0x3F;
  uint8_t b5 = c & 0x1F;
  uint8_t r = (r5 << 3) | (r5 >> 2);
  uint8_t g = (g6 << 2) | (g6 >> 4);
  uint8_t b = (b5 << 3) | (b5 >> 2);
  return lv_color_make(r, g, b);
}

static void disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map) {
  uint32_t w = (uint32_t)(area->x2 - area->x1 + 1);
  uint32_t h = (uint32_t)(area->y2 - area->y1 + 1);
  tft.drawRGBBitmap(area->x1, area->y1, (uint16_t *)px_map, (int16_t)w, (int16_t)h);
  lv_display_flush_ready(disp);
}

static uint32_t lvgl_tick_get_cb(void) {
  return (uint32_t)millis();
}

static void touch_read(lv_indev_t *indev, lv_indev_data_t *data) {
  (void)indev;
  if (touch.touched()) {
    TS_Point p = touch.getPoint();
    lv_coord_t x, y;
    ts_map(p.x, p.y, &x, &y);
    ts_last_x = x;
    ts_last_y = y;
    data->state = LV_INDEV_STATE_PRESSED;
    data->point.x = x;
    data->point.y = y;
  } else {
    data->state = LV_INDEV_STATE_RELEASED;
    data->point.x = ts_last_x;
    data->point.y = ts_last_y;
  }
}

void ui_sync_all_gain_sliders(void) {
  ui_slider_internal = true;
  if (sl_clock_vol) lv_slider_set_value(sl_clock_vol, constrain((int)curGain, 0, 100), LV_ANIM_OFF);
  if (sl_radio_gain) lv_slider_set_value(sl_radio_gain, constrain((int)curGain, 0, 100), LV_ANIM_OFF);
  if (sl_cfg_gain) lv_slider_set_value(sl_cfg_gain, constrain((int)curGain, 0, 100), LV_ANIM_OFF);
  ui_slider_internal = false;
}

static void cfg_sync_value_labels(void);

/* Touch: 1–4 % am Schieberegler schwer treffbar → auf 0 % (LDR) schnappen; von 0 % aus 1–4 → 5 % (symmetrisch hoch). */
static const int BRIGHT_SNAP_EDGE = 4;
static const int BRIGHT_MANUAL_MIN = 5;

static int bright_slider_snap(int raw, uint8_t prev_bright) {
  if (raw <= 0) return 0;
  if (raw > BRIGHT_SNAP_EDGE) return raw;
  return (prev_bright == 0) ? BRIGHT_MANUAL_MIN : 0;
}

static void on_gain_slider(lv_event_t *e) {
  if (ui_slider_internal) return;
  lv_obj_t *sl = (lv_obj_t *)lv_event_get_target(e);
  int v = (int)lv_slider_get_value(sl);
  curGain = (uint8_t)constrain(v, 0, 100);
  pref.putUShort("gain", curGain);
  setGain(curGain);
  ui_sync_all_gain_sliders();
  cfg_sync_value_labels();
}

static void on_bright_slider(lv_event_t *e) {
  if (ui_slider_internal) return;
  (void)e;
  int raw = (int)lv_slider_get_value(sl_cfg_bright);
  int v = bright_slider_snap(raw, bright);
  if (v != raw) {
    ui_slider_internal = true;
    lv_slider_set_value(sl_cfg_bright, v, LV_ANIM_OFF);
    ui_slider_internal = false;
  }
  bright = (uint8_t)constrain(v, 0, 100);
  pref.putUShort("bright", bright);
  setBGLight(bright == 0 ? 0 : bright);
  cfg_sync_value_labels();
}

static void on_snooze_slider(lv_event_t *e) {
  if (ui_slider_internal) return;
  (void)e;
  int v = (int)lv_slider_get_value(sl_cfg_snooze);
  snoozeTime = (uint8_t)constrain(v, 0, 60);
  pref.putUShort("snooze", snoozeTime);
  cfg_sync_value_labels();
}

/* Startseite (schwarzer Hintergrund) */
static void style_slider(lv_obj_t *sl) {
  lv_obj_set_style_bg_color(sl, lv_color_hex(0x404040), LV_PART_MAIN);
  lv_obj_set_style_bg_color(sl, lv_palette_main(LV_PALETTE_ORANGE), LV_PART_INDICATOR);
  lv_obj_set_style_bg_color(sl, lv_palette_darken(LV_PALETTE_ORANGE, 2), LV_PART_KNOB);
}

/* Favoriten-, Einstellungs-, Alarm-UI (hellgrauer Screen) — keine dunkle „Schlucht“ an den Rändern */
static void style_slider_light(lv_obj_t *sl) {
  lv_obj_set_style_bg_color(sl, lv_color_hex(0x888888), LV_PART_MAIN);
  lv_obj_set_style_bg_color(sl, lv_palette_main(LV_PALETTE_ORANGE), LV_PART_INDICATOR);
  lv_obj_set_style_bg_color(sl, lv_palette_darken(LV_PALETTE_ORANGE, 2), LV_PART_KNOB);
}

static void cfg_sync_value_labels(void) {
  char b[16];
  if (lbl_cfg_gain_val) {
    snprintf(b, sizeof(b), "%u", (unsigned)constrain((int)curGain, 0, 100));
    lv_label_set_text(lbl_cfg_gain_val, b);
  }
  if (lbl_cfg_bright_val) {
    snprintf(b, sizeof(b), "%u%%", (unsigned)constrain((int)bright, 0, 100));
    lv_label_set_text(lbl_cfg_bright_val, b);
  }
  if (lbl_cfg_snooze_val) {
    snprintf(b, sizeof(b), "%u min", (unsigned)constrain((int)snoozeTime, 0, 60));
    lv_label_set_text(lbl_cfg_snooze_val, b);
  }
}

// --- Stations-Navigation (Touch & Web ohne server.send) ---
bool station_navigate_prev(void) {
  int attempts = 0;
  do {
    if (curStation == 0) curStation = STATIONS - 1;
    else curStation--;
    attempts++;
    if (attempts >= STATIONS) {
      curStation = STATIONS - 1;
      RADIO_SERIAL(Serial.println(F("Keine aktive Station gefunden.")));
      return false;
    }
  } while (!stationlist[curStation].enabled);
  changeStation();
  return true;
}

bool station_navigate_next(void) {
  int attempts = 0;
  do {
    if (curStation >= STATIONS - 1) curStation = 0;
    else curStation++;
    attempts++;
    if (attempts >= STATIONS) {
      curStation = 0;
      RADIO_SERIAL(Serial.println(F("Keine aktive Station gefunden.")));
      return false;
    }
  } while (!stationlist[curStation].enabled);
  changeStation();
  return true;
}

static void rebuild_favorites(void) {
  fav_count = 0;
  for (int i = 0; i < STATIONS && fav_count < 10; i++) {
    if (stationlist[i].enabled) {
      fav_map[fav_count] = i;
      const char *nm = stationlist[i].name;
      /* Voller UTF-8-Name — %.6s hätte Umlaute mitten im Zeichen zerschnitten */
      lv_label_set_text(fav_lbl[fav_count], nm);
      lv_obj_clear_flag(fav_btn[fav_count], LV_OBJ_FLAG_HIDDEN);
      if (actStation == i) {
        lv_obj_set_style_bg_color(fav_btn[fav_count], lv_palette_main(LV_PALETTE_ORANGE), LV_PART_MAIN);
      } else {
        lv_obj_set_style_bg_color(fav_btn[fav_count], lv_palette_main(LV_PALETTE_BLUE), LV_PART_MAIN);
      }
      fav_count++;
    }
  }
  for (int j = fav_count; j < 10; j++) {
    lv_obj_add_flag(fav_btn[j], LV_OBJ_FLAG_HIDDEN);
  }
}

static void on_fav_btn(lv_event_t *e) {
  int idx = (int)(intptr_t)lv_event_get_user_data(e);
  if (idx < 0 || idx >= fav_count) return;
  curStation = fav_map[idx];
  changeStation();
  showStartPage();
}

static void cfg_station_prev_action(void) {
  if (curStation == 0) curStation = STATIONS - 1;
  else curStation--;
  int guard = 0;
  while (!stationlist[curStation].enabled && guard++ < STATIONS) {
    if (curStation == 0) curStation = STATIONS - 1;
    else curStation--;
  }
  updateStation();
}

static void cfg_station_next_action(void) {
  if (curStation >= STATIONS - 1) curStation = 0;
  else curStation++;
  int guard = 0;
  while (!stationlist[curStation].enabled && guard++ < STATIONS) {
    if (curStation >= STATIONS - 1) curStation = 0;
    else curStation++;
  }
  updateStation();
}

static void on_station_prev(lv_event_t *e) {
  (void)e;
  cfg_station_prev_action();
}

static void on_station_next(lv_event_t *e) {
  (void)e;
  cfg_station_next_action();
}

static void on_cfg_station_prev(lv_event_t *e) {
  (void)e;
  on_station_prev(e);
}

static void on_cfg_station_next(lv_event_t *e) {
  (void)e;
  on_station_next(e);
}

static void footer_style_btn(lv_obj_t *b) {
  lv_obj_set_size(b, 62, 58);
  lv_obj_set_style_radius(b, 4, LV_PART_MAIN);
  lv_obj_set_style_bg_color(b, lv_palette_main(LV_PALETTE_BLUE), LV_PART_MAIN);
  lv_obj_set_style_text_color(b, lv_color_white(), LV_PART_MAIN);
}

/* Sender ↑/↓ in strow: gleiche blaue Fläche wie Footer — sonst LVGL-Default (wirkt schwarz auf hellgrau). */
static void station_arrow_btn_style(lv_obj_t *b) {
  lv_obj_set_style_radius(b, 4, LV_PART_MAIN);
  lv_obj_set_style_bg_color(b, lv_palette_main(LV_PALETTE_BLUE), LV_PART_MAIN);
  lv_obj_set_style_text_color(b, lv_color_white(), LV_PART_MAIN);
}

static void evt_footer_power(lv_event_t *e) {
  (void)e;
  toggleRadio(radio);
}

static void evt_footer_snooze(lv_event_t *e) {
  (void)e;
  startSnooze();
}

static void evt_footer_alarm(lv_event_t *e) {
  (void)e;
  toggleAlarm();
}

static void evt_footer_mid_radio(lv_event_t *e) {
  (void)e;
  changeStation();
}

static void evt_footer_cfg_radio(lv_event_t *e) {
  (void)e;
  showFavoritenPage();
}

static void evt_footer_mid_cfg(lv_event_t *e) {
  (void)e;
  changeStation();
}

static void evt_footer_alarm_from_cfg(lv_event_t *e) {
  (void)e;
  startpage = false;
  favoritenpage = false;
  settingspage = false;
  alarmpage = true;
  showAlarmPage();
}

static void evt_footer_save_alarm(lv_event_t *e) {
  (void)e;
  safeAlarmTime();
}

static void evt_footer_home_alarm(lv_event_t *e) {
  (void)e;
  startpage = true;
  favoritenpage = false;
  settingspage = false;
  alarmpage = false;
  showStartPage();
}

static void evt_open_radio_page(lv_event_t *e) {
  (void)e;
  startpage = false;
  favoritenpage = false;
  settingspage = true;
  alarmpage = false;
  showSettingsPage();
}

static void on_clock_prev(lv_event_t *e) {
  (void)e;
  station_navigate_prev();
}

static void on_clock_next(lv_event_t *e) {
  (void)e;
  station_navigate_next();
}

static uint8_t day_mask_from_col(uint8_t col) {
  return (uint8_t)(1u << ((col + 1) % 7));
}

static void on_alarm_day(lv_event_t *e) {
  uintptr_t u = (uintptr_t)lv_event_get_user_data(e);
  uint8_t row = (uint8_t)(u >> 8);
  uint8_t col = (uint8_t)(u & 0xFF);
  uint8_t m = day_mask_from_col(col);
  if (row == 0) alarmConfig.alarmday_1 ^= m;
  else alarmConfig.alarmday_2 ^= m;
  showAlarms_Day_and_Time();
}

static void alarm_adj_h(uint8_t alarm_idx, int delta) {
  if (alarm_idx == 0) {
    int h = alarmConfig.h_1 + delta;
    if (h < 0) h = 23;
    if (h > 23) h = 0;
    alarmConfig.h_1 = (uint8_t)h;
  } else {
    int h = alarmConfig.h_2 + delta;
    if (h < 0) h = 23;
    if (h > 23) h = 0;
    alarmConfig.h_2 = (uint8_t)h;
  }
  showAlarms_Day_and_Time();
}

static void alarm_step_min(uint8_t *m, int delta) {
  int mm = *m;
  if (delta > 0) {
    if (mm % 5 == 0) mm += 5;
    else mm = ((mm + 4) / 5) * 5;
    if (mm >= 60) mm = 0;
  } else {
    if (mm % 5 == 0) mm -= 5;
    else mm = ((mm - 1) / 5) * 5;
    if (mm < 0) mm = 55;
  }
  *m = (uint8_t)mm;
}

static void alarm_adj_m(uint8_t alarm_idx, int delta) {
  if (alarm_idx == 0) alarm_step_min(&alarmConfig.m_1, delta);
  else alarm_step_min(&alarmConfig.m_2, delta);
  showAlarms_Day_and_Time();
}

static void on_al_h_up1(lv_event_t *e) {
  (void)e;
  alarm_adj_h(0, 1);
}
static void on_al_h_dn1(lv_event_t *e) {
  (void)e;
  alarm_adj_h(0, -1);
}
static void on_al_m_up1(lv_event_t *e) {
  (void)e;
  alarm_adj_m(0, 1);
}
static void on_al_m_dn1(lv_event_t *e) {
  (void)e;
  alarm_adj_m(0, -1);
}
static void on_al_h_up2(lv_event_t *e) {
  (void)e;
  alarm_adj_h(1, 1);
}
static void on_al_h_dn2(lv_event_t *e) {
  (void)e;
  alarm_adj_h(1, -1);
}
static void on_al_m_up2(lv_event_t *e) {
  (void)e;
  alarm_adj_m(1, 1);
}
static void on_al_m_dn2(lv_event_t *e) {
  (void)e;
  alarm_adj_m(1, -1);
}

static lv_obj_t *make_footer_nav(lv_obj_t *parent) {
  lv_obj_t *row = lv_obj_create(parent);
  lv_obj_remove_style_all(row);
  lv_obj_set_size(row, LV_PCT(100), 62);
  lv_obj_set_style_bg_color(row, rgb565_to_lv(COLOR_KNOEPFE_BG), LV_PART_MAIN);
  lv_obj_set_style_bg_opa(row, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_pad_all(row, 2, LV_PART_MAIN);
  lv_obj_set_flex_flow(row, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(row, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
  lv_obj_set_scrollbar_mode(row, LV_SCROLLBAR_MODE_OFF);
  return row;
}

static void build_msg_screen(void) {
  scr_msg = lv_obj_create(NULL);
  lv_obj_set_style_bg_color(scr_msg, rgb565_to_lv(COLOR_KNOEPFE_BG), LV_PART_MAIN);
  lv_obj_set_style_bg_opa(scr_msg, LV_OPA_COVER, LV_PART_MAIN);
  auto msg_line_style = [](lv_obj_t *lb) {
    lv_obj_set_style_text_color(lb, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_text_align(lb, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
  };
  msg_title = lv_label_create(scr_msg);
  lv_obj_set_width(msg_title, LV_PCT(100));
  lv_label_set_long_mode(msg_title, LV_LABEL_LONG_WRAP);
  lv_obj_align(msg_title, LV_ALIGN_TOP_MID, 0, 20);
  msg_line_style(msg_title);

  msg_line2 = lv_label_create(scr_msg);
  lv_obj_set_width(msg_line2, LV_PCT(100));
  lv_label_set_long_mode(msg_line2, LV_LABEL_LONG_WRAP);
  lv_obj_align(msg_line2, LV_ALIGN_TOP_MID, 0, 70);
  msg_line_style(msg_line2);

  msg_line3 = lv_label_create(scr_msg);
  lv_obj_set_width(msg_line3, LV_PCT(100));
  lv_label_set_long_mode(msg_line3, LV_LABEL_LONG_WRAP);
  lv_obj_align(msg_line3, LV_ALIGN_TOP_MID, 0, 110);
  msg_line_style(msg_line3);

  msg_line4 = lv_label_create(scr_msg);
  lv_obj_set_width(msg_line4, LV_PCT(100));
  lv_label_set_long_mode(msg_line4, LV_LABEL_LONG_WRAP);
  lv_obj_align(msg_line4, LV_ALIGN_TOP_MID, 0, 150);
  msg_line_style(msg_line4);

  msg_line5 = lv_label_create(scr_msg);
  lv_obj_set_width(msg_line5, LV_PCT(100));
  lv_label_set_long_mode(msg_line5, LV_LABEL_LONG_WRAP);
  lv_obj_align(msg_line5, LV_ALIGN_TOP_MID, 0, 190);
  msg_line_style(msg_line5);
}

static void build_ota_screen(void) {
  scr_ota = lv_obj_create(NULL);
  lv_obj_set_style_bg_color(scr_ota, rgb565_to_lv(COLOR_KNOEPFE_BG), LV_PART_MAIN);
  lv_obj_set_style_bg_opa(scr_ota, LV_OPA_COVER, LV_PART_MAIN);
  ota_lbl = lv_label_create(scr_ota);
  lv_label_set_text(ota_lbl, "OTA");
  lv_obj_align(ota_lbl, LV_ALIGN_TOP_MID, 0, 16);
  lv_obj_set_style_text_color(ota_lbl, lv_color_black(), LV_PART_MAIN);

  ota_bar = lv_bar_create(scr_ota);
  lv_obj_set_size(ota_bar, 300, 22);
  lv_obj_align(ota_bar, LV_ALIGN_CENTER, 0, -10);
  lv_bar_set_range(ota_bar, 0, 100);
  lv_obj_set_style_bg_color(ota_bar, lv_color_hex(0x888888), LV_PART_MAIN);
  lv_obj_set_style_bg_color(ota_bar, lv_palette_main(LV_PALETTE_ORANGE), LV_PART_INDICATOR);

  ota_sub = lv_label_create(scr_ota);
  lv_obj_set_width(ota_sub, LV_PCT(100));
  lv_label_set_long_mode(ota_sub, LV_LABEL_LONG_WRAP);
  lv_obj_align(ota_sub, LV_ALIGN_BOTTOM_MID, 0, -24);
  lv_obj_set_style_text_align(ota_sub, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
  lv_obj_set_style_text_color(ota_sub, lv_color_black(), LV_PART_MAIN);
}

static void build_clock_screen(void) {
  scr_clock = lv_obj_create(NULL);
  lv_obj_set_style_bg_color(scr_clock, rgb565_to_lv(COLOR_BG), LV_PART_MAIN);
  lv_obj_set_style_bg_opa(scr_clock, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_pad_all(scr_clock, 0, LV_PART_MAIN);
  lv_obj_clear_flag(scr_clock, LV_OBJ_FLAG_SCROLLABLE);

  lv_obj_t *hdr = lv_obj_create(scr_clock);
  lv_obj_remove_style_all(hdr);
  lv_obj_set_style_bg_opa(hdr, LV_OPA_TRANSP, LV_PART_MAIN);
  lv_obj_set_size(hdr, 320, 22);
  lv_obj_align(hdr, LV_ALIGN_TOP_MID, 0, 0);
  lv_obj_set_flex_flow(hdr, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(hdr, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
  lv_obj_set_style_pad_left(hdr, 4, LV_PART_MAIN);
  lv_obj_set_style_pad_right(hdr, 4, LV_PART_MAIN);

  lbl_alarm_hdr = lv_label_create(hdr);
  lv_obj_set_width(lbl_alarm_hdr, 88);
  lv_label_set_long_mode(lbl_alarm_hdr, LV_LABEL_LONG_DOT);
  lv_obj_set_style_text_font(lbl_alarm_hdr, &font_ui_primary, LV_PART_MAIN);

  lbl_ip = lv_label_create(hdr);
  lv_obj_set_flex_grow(lbl_ip, 1);
  lv_obj_set_style_text_align(lbl_ip, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
  lv_obj_set_style_text_font(lbl_ip, &font_ui_primary, LV_PART_MAIN);
  lv_obj_set_style_text_color(lbl_ip, rgb565_to_lv(COLOR_IP), LV_PART_MAIN);

  lbl_snooze = lv_label_create(hdr);
  lv_label_set_text(lbl_snooze, "Zz");
  lv_obj_set_width(lbl_snooze, 22);
  lv_obj_set_style_text_font(lbl_snooze, &font_ui_primary, LV_PART_MAIN);

  lbl_rssi = lv_label_create(hdr);
  lv_obj_set_width(lbl_rssi, 52);
  lv_obj_set_style_text_font(lbl_rssi, &font_ui_primary, LV_PART_MAIN);
  lv_obj_set_style_text_align(lbl_rssi, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN);

  lv_obj_t *btn_l;
  lv_obj_t *btn_r;

  /* Uhr: 7-Segment-Bitmaps 72×78 px, volle Zeilenbreite ~4×72 + Doppelpunkt (tools/gen_clock_font.ps1) */
  const lv_coord_t clock_top = 26;
  const lv_coord_t clock_bar_h = 86;
  const lv_coord_t clock_digit_w = 72;
  const lv_coord_t clock_digit_h = 82;
  const lv_coord_t date_below_clock = 4;
  const lv_coord_t date_row_h = 36;
  const lv_coord_t gap_date_to_mid = 0;

  clock_time_bar = lv_obj_create(scr_clock);
  lv_obj_remove_style_all(clock_time_bar);
  lv_obj_set_size(clock_time_bar, 320, clock_bar_h);
  lv_obj_align(clock_time_bar, LV_ALIGN_TOP_MID, 0, clock_top);
  lv_obj_set_flex_flow(clock_time_bar, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(clock_time_bar, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
  lv_obj_set_style_pad_column(clock_time_bar, 2, LV_PART_MAIN);
  lv_obj_clear_flag(clock_time_bar, LV_OBJ_FLAG_SCROLLABLE);

  auto mk_digit_cell = [&](int idx) {
    clock_digit_cell[idx] = lv_obj_create(clock_time_bar);
    lv_obj_remove_style_all(clock_digit_cell[idx]);
    lv_obj_set_size(clock_digit_cell[idx], clock_digit_w, clock_digit_h);
    lv_obj_set_style_bg_opa(clock_digit_cell[idx], LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_opa(clock_digit_cell[idx], LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_pad_all(clock_digit_cell[idx], 0, LV_PART_MAIN);
    clock_digit_lbl[idx] = lv_label_create(clock_digit_cell[idx]);
    lv_label_set_text(clock_digit_lbl[idx], "0");
    lv_obj_set_style_text_font(clock_digit_lbl[idx], font_clock_digits, LV_PART_MAIN);
    lv_obj_set_style_text_color(clock_digit_lbl[idx], rgb565_to_lv(COLOR_TIME), LV_PART_MAIN);
    lv_obj_set_style_text_letter_space(clock_digit_lbl[idx], 0, LV_PART_MAIN);
    lv_obj_center(clock_digit_lbl[idx]);
  };

  mk_digit_cell(0);
  mk_digit_cell(1);
  clock_colon = lv_label_create(clock_time_bar);
  lv_label_set_text(clock_colon, ":");
  lv_obj_set_style_text_font(clock_colon, font_clock_digits, LV_PART_MAIN);
  lv_obj_set_style_text_color(clock_colon, rgb565_to_lv(COLOR_TIME), LV_PART_MAIN);
  lv_obj_set_style_pad_top(clock_colon, 0, LV_PART_MAIN);
  mk_digit_cell(2);
  mk_digit_cell(3);

  lv_obj_update_layout(clock_time_bar);

  lbl_date = lv_label_create(scr_clock);
  lv_obj_set_width(lbl_date, 308);
  lv_label_set_long_mode(lbl_date, LV_LABEL_LONG_WRAP);
  lv_obj_set_style_text_align(lbl_date, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
  lv_obj_set_style_text_color(lbl_date, rgb565_to_lv(COLOR_DATE), LV_PART_MAIN);
  lv_obj_set_style_text_font(lbl_date, &font_clock_date, LV_PART_MAIN);
  lv_obj_set_style_text_line_space(lbl_date, 2, LV_PART_MAIN);
  lv_obj_align(lbl_date, LV_ALIGN_TOP_MID, 0, clock_top + clock_bar_h + date_below_clock);

  const lv_coord_t mid_y = clock_top + clock_bar_h + date_below_clock + date_row_h + gap_date_to_mid;

  mid_weather = lv_obj_create(scr_clock);
  lv_obj_remove_style_all(mid_weather);
  lv_obj_set_size(mid_weather, 314, 76);
  lv_obj_align(mid_weather, LV_ALIGN_TOP_MID, 0, mid_y);
  lv_obj_set_flex_flow(mid_weather, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(mid_weather, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
  lv_obj_set_style_pad_column(mid_weather, 2, LV_PART_MAIN);
  lv_obj_clear_flag(mid_weather, LV_OBJ_FLAG_CLICKABLE);

  auto mkcol = [&](lv_obj_t *parent) {
    lv_obj_t *c = lv_obj_create(parent);
    lv_obj_set_size(c, 100, 72);
    lv_obj_set_style_bg_color(c, rgb565_to_lv(COLOR_STATION_BOX_BG), LV_PART_MAIN);
    lv_obj_set_style_border_color(c, rgb565_to_lv(COLOR_STATION_BOX_BORDER), LV_PART_MAIN);
    lv_obj_set_style_border_width(c, 1, LV_PART_MAIN);
    lv_obj_set_style_pad_all(c, 2, LV_PART_MAIN);
    lv_obj_set_flex_flow(c, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(c, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    lv_obj_set_scrollbar_mode(c, LV_SCROLLBAR_MODE_OFF);
    lv_obj_clear_flag(c, LV_OBJ_FLAG_CLICKABLE);
    return c;
  };
  lv_obj_t *c0 = mkcol(mid_weather);
  lv_obj_t *c1 = mkcol(mid_weather);
  lv_obj_t *c2 = mkcol(mid_weather);
  (void)c0;
  auto mk3 = [&](lv_obj_t *col, lv_obj_t **a, lv_obj_t **b, lv_obj_t **c) {
    *a = lv_label_create(col);
    *b = lv_label_create(col);
    *c = lv_label_create(col);
    lv_obj_set_width(*a, LV_PCT(100));
    lv_obj_set_width(*b, LV_PCT(100));
    lv_obj_set_width(*c, LV_PCT(100));
    lv_label_set_long_mode(*a, LV_LABEL_LONG_DOT);
    lv_label_set_long_mode(*b, LV_LABEL_LONG_DOT);
    lv_label_set_long_mode(*c, LV_LABEL_LONG_DOT);
    lv_obj_set_style_text_color(*a, rgb565_to_lv(COLOR_STATION_TITLE), LV_PART_MAIN);
    lv_obj_set_style_text_color(*b, rgb565_to_lv(COLOR_STATION_TITLE), LV_PART_MAIN);
    lv_obj_set_style_text_color(*c, rgb565_to_lv(COLOR_STATION_TITLE), LV_PART_MAIN);
    lv_obj_set_style_text_font(*a, &font_ui_primary, LV_PART_MAIN);
    lv_obj_set_style_text_font(*b, &font_ui_primary, LV_PART_MAIN);
    lv_obj_set_style_text_font(*c, &font_ui_primary, LV_PART_MAIN);
    lv_obj_clear_flag(*a, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(*b, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(*c, LV_OBJ_FLAG_CLICKABLE);
  };
  lv_obj_t *t0a, *t0b, *t0c;
  lv_obj_t *t1a, *t1b, *t1c;
  lv_obj_t *t2a, *t2b, *t2c;
  mk3(c0, &t0a, &t0b, &t0c);
  mk3(c1, &t1a, &t1b, &t1c);
  mk3(c2, &t2a, &t2b, &t2c);
  w_lbl_now_t = t0b;
  w_lbl_now_f = t0c;
  w_lbl_td_min = t1b;
  w_lbl_td_max = t1c;
  w_lbl_tm_min = t2b;
  w_lbl_tm_max = t2c;
  lv_label_set_text(t0a, TXT_NOW);
  lv_label_set_text(t1a, TXT_TODAY);
  lv_label_set_text(t2a, TXT_TOMMORROW);
  /* LVGL 9 + LV_WIDGETS_HAS_DEFAULT_VALUE: leere Labels heißen sonst "Text" bis displayWeather läuft. */
  lv_label_set_text(t0b, TXT_TEMP " -- C");
  lv_label_set_text(t0c, TXT_FEELS " -- C");
  lv_label_set_text(t1b, TXT_MIN " -- C");
  lv_label_set_text(t1c, TXT_MAX " -- C");
  lv_label_set_text(t2b, TXT_MIN " -- C");
  lv_label_set_text(t2c, TXT_MAX " -- C");

  mid_radio = lv_obj_create(scr_clock);
  lv_obj_remove_style_all(mid_radio);
  lv_obj_set_size(mid_radio, 314, 76);
  lv_obj_align(mid_radio, LV_ALIGN_TOP_MID, 0, mid_y);
  lv_obj_set_style_bg_color(mid_radio, rgb565_to_lv(COLOR_STATION_BOX_BG), LV_PART_MAIN);
  lv_obj_set_style_border_color(mid_radio, rgb565_to_lv(COLOR_STATION_BOX_BORDER), LV_PART_MAIN);
  lv_obj_set_style_border_width(mid_radio, 1, LV_PART_MAIN);
  lv_obj_set_style_pad_all(mid_radio, 6, LV_PART_MAIN);
  lv_obj_set_flex_flow(mid_radio, LV_FLEX_FLOW_COLUMN);
  /* START + CENTER: Titel links unter dem zentrierten Sendernamen (WRAP+CENTER buggt in LVGL9 teils: Anfang abgeschnitten) */
  lv_obj_set_flex_align(mid_radio, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
  lv_obj_add_flag(mid_radio, LV_OBJ_FLAG_HIDDEN);

  lbl_station_big = lv_label_create(mid_radio);
  lv_obj_set_width(lbl_station_big, LV_PCT(100));
  lv_label_set_long_mode(lbl_station_big, LV_LABEL_LONG_DOT);
  lv_obj_set_style_text_align(lbl_station_big, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
  lv_obj_set_style_text_color(lbl_station_big, rgb565_to_lv(COLOR_STATION_NAME), LV_PART_MAIN);
  lv_obj_set_style_text_font(lbl_station_big, &font_radio_meta, LV_PART_MAIN);
  lv_label_set_text(lbl_station_big, "-");

  lbl_stream_title = lv_label_create(mid_radio);
  lv_obj_set_width(lbl_stream_title, LV_PCT(100));
  lv_label_set_long_mode(lbl_stream_title, LV_LABEL_LONG_WRAP);
  lv_obj_set_style_text_align(lbl_stream_title, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN);
  lv_obj_set_style_text_color(lbl_stream_title, rgb565_to_lv(COLOR_STATION_TITLE), LV_PART_MAIN);
  lv_obj_set_style_text_font(lbl_stream_title, &font_radio_meta, LV_PART_MAIN);
  lv_obj_clear_flag(lbl_stream_title, LV_OBJ_FLAG_SCROLLABLE);
  lv_label_set_text(lbl_stream_title, "");

  /* Sender vor/zurück: unter der Uhr im Datumsband (nicht über der Uhr), Breite wie zuvor. */
  const lv_coord_t clock_nav_w = 34;
  const lv_coord_t clock_nav_y = clock_top + clock_bar_h + date_below_clock;
  const lv_coord_t clock_nav_h = date_row_h;
  btn_l = lv_button_create(scr_clock);
  lv_obj_set_size(btn_l, clock_nav_w, clock_nav_h);
  lv_obj_align(btn_l, LV_ALIGN_TOP_LEFT, 0, clock_nav_y);
  lv_obj_set_style_bg_opa(btn_l, LV_OPA_30, LV_PART_MAIN);
  lv_obj_add_event_cb(btn_l, on_clock_prev, LV_EVENT_CLICKED, NULL);
  lv_obj_t *l1 = lv_label_create(btn_l);
  lv_label_set_text(l1, LV_SYMBOL_LEFT);
  lv_obj_center(l1);

  btn_r = lv_button_create(scr_clock);
  lv_obj_set_size(btn_r, clock_nav_w, clock_nav_h);
  lv_obj_align(btn_r, LV_ALIGN_TOP_RIGHT, 0, clock_nav_y);
  lv_obj_set_style_bg_opa(btn_r, LV_OPA_30, LV_PART_MAIN);
  lv_obj_add_event_cb(btn_r, on_clock_next, LV_EVENT_CLICKED, NULL);
  lv_obj_t *l2 = lv_label_create(btn_r);
  lv_label_set_text(l2, LV_SYMBOL_RIGHT);
  lv_obj_center(l2);

  /* Tap-Bereich unter der Kopfzeile bis über Wetter/Radio-Block */
  lv_obj_t *btn_clock_radio = lv_button_create(scr_clock);
  lv_obj_set_size(btn_clock_radio, 288, (lv_coord_t)(mid_y + 76 - 22));
  lv_obj_align(btn_clock_radio, LV_ALIGN_TOP_MID, 0, 22);
  lv_obj_add_flag(btn_clock_radio, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_set_style_bg_opa(btn_clock_radio, LV_OPA_TRANSP, LV_PART_MAIN);
  lv_obj_set_style_border_opa(btn_clock_radio, LV_OPA_TRANSP, LV_PART_MAIN);
  lv_obj_set_style_shadow_opa(btn_clock_radio, LV_OPA_TRANSP, LV_PART_MAIN);
  lv_obj_add_event_cb(btn_clock_radio, evt_open_radio_page, LV_EVENT_CLICKED, NULL);

  sl_clock_vol = lv_slider_create(scr_clock);
  lv_obj_set_size(sl_clock_vol, 300, 18);
  lv_obj_align(sl_clock_vol, LV_ALIGN_BOTTOM_MID, 0, -8);
  lv_slider_set_range(sl_clock_vol, 0, 100);
  style_slider(sl_clock_vol);
  lv_obj_add_event_cb(sl_clock_vol, on_gain_slider, LV_EVENT_VALUE_CHANGED, NULL);
  lv_obj_add_event_cb(sl_clock_vol, on_gain_slider, LV_EVENT_RELEASED, NULL);

  /* Transparenter Tap-Layer liegt sonst über den Pfeilen — Senderwechsel geht nicht */
  lv_obj_move_foreground(btn_l);
  lv_obj_move_foreground(btn_r);
}

static void add_radio_footer(lv_obj_t *parent, bool settings_next) {
  lv_obj_t *row = make_footer_nav(parent);
  lv_obj_align(row, LV_ALIGN_BOTTOM_MID, 0, 0);
  lv_obj_t *b0 = lv_button_create(row);
  footer_style_btn(b0);
  lv_obj_t *x0 = lv_label_create(b0);
  lv_label_set_text(x0, LV_SYMBOL_POWER);
  lv_obj_center(x0);
  lv_obj_add_event_cb(b0, evt_footer_power, LV_EVENT_CLICKED, NULL);

  lv_obj_t *b1 = lv_button_create(row);
  footer_style_btn(b1);
  lv_obj_t *x1 = lv_label_create(b1);
  lv_label_set_text(x1, "Zz");
  lv_obj_center(x1);
  lv_obj_add_event_cb(b1, evt_footer_snooze, LV_EVENT_CLICKED, NULL);

  lv_obj_t *b2 = lv_button_create(row);
  footer_style_btn(b2);
  lv_obj_t *x2 = lv_label_create(b2);
  lv_label_set_text(x2, LV_SYMBOL_BELL);
  lv_obj_center(x2);
  lv_obj_add_event_cb(b2, evt_footer_alarm, LV_EVENT_CLICKED, NULL);

  lv_obj_t *b3 = lv_button_create(row);
  footer_style_btn(b3);
  lv_obj_t *x3 = lv_label_create(b3);
  lv_label_set_text(x3, LV_SYMBOL_NEXT);
  lv_obj_center(x3);
  lv_obj_add_event_cb(b3, evt_footer_mid_radio, LV_EVENT_CLICKED, NULL);

  lv_obj_t *b4 = lv_button_create(row);
  footer_style_btn(b4);
  lv_obj_t *x4 = lv_label_create(b4);
  lv_label_set_text(x4, settings_next ? LV_SYMBOL_SETTINGS : LV_SYMBOL_HOME);
  lv_obj_center(x4);
  if (settings_next)
    lv_obj_add_event_cb(b4, evt_footer_cfg_radio, LV_EVENT_CLICKED, NULL);
  else
    lv_obj_add_event_cb(b4, evt_footer_home_alarm, LV_EVENT_CLICKED, NULL);

  ui_footer_register_state_row(b0, x0, b1, x1, b2, x2);
}

static void build_radio_screen(void) {
  scr_radio = lv_obj_create(NULL);
  lv_obj_set_style_bg_color(scr_radio, rgb565_to_lv(COLOR_KNOEPFE_BG), LV_PART_MAIN);
  lv_obj_set_style_bg_opa(scr_radio, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_pad_all(scr_radio, 0, LV_PART_MAIN);

  sl_radio_gain = lv_slider_create(scr_radio);
  lv_obj_set_width(sl_radio_gain, 312);
  lv_obj_set_height(sl_radio_gain, 16);
  lv_obj_align(sl_radio_gain, LV_ALIGN_TOP_MID, 0, 14);
  lv_slider_set_range(sl_radio_gain, 0, 100);
  style_slider_light(sl_radio_gain);
  lv_obj_add_event_cb(sl_radio_gain, on_gain_slider, LV_EVENT_VALUE_CHANGED, NULL);
  lv_obj_add_event_cb(sl_radio_gain, on_gain_slider, LV_EVENT_RELEASED, NULL);

  lv_obj_t *gf = lv_obj_create(scr_radio);
  lv_obj_remove_style_all(gf);
  lv_obj_set_size(gf, 320, 90);
  lv_obj_align(gf, LV_ALIGN_TOP_MID, 0, 38);
  lv_obj_set_style_bg_color(gf, rgb565_to_lv(COLOR_KNOEPFE_BG), LV_PART_MAIN);
  lv_obj_set_style_bg_opa(gf, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_flex_flow(gf, LV_FLEX_FLOW_ROW_WRAP);
  lv_obj_set_style_pad_column(gf, 2, LV_PART_MAIN);
  lv_obj_set_style_pad_row(gf, 2, LV_PART_MAIN);
  lv_obj_set_flex_align(gf, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

  for (int i = 0; i < 10; i++) {
    fav_btn[i] = lv_button_create(gf);
    lv_obj_set_size(fav_btn[i], 60, 40);
    station_arrow_btn_style(fav_btn[i]);
    fav_lbl[i] = lv_label_create(fav_btn[i]);
    lv_obj_set_width(fav_lbl[i], 54);
    lv_label_set_long_mode(fav_lbl[i], LV_LABEL_LONG_DOT);
    lv_obj_set_style_text_font(fav_lbl[i], &font_ui_primary, LV_PART_MAIN);
    lv_obj_set_style_text_color(fav_lbl[i], lv_color_white(), LV_PART_MAIN);
    lv_obj_center(fav_lbl[i]);
    lv_obj_add_event_cb(fav_btn[i], on_fav_btn, LV_EVENT_CLICKED, (void *)(intptr_t)i);
    lv_obj_add_flag(fav_btn[i], LV_OBJ_FLAG_HIDDEN);
  }

  lv_obj_t *strow = lv_obj_create(scr_radio);
  lv_obj_remove_style_all(strow);
  lv_obj_set_size(strow, 320, 50);
  lv_obj_align(strow, LV_ALIGN_TOP_MID, 0, 128);
  lv_obj_set_style_bg_color(strow, rgb565_to_lv(COLOR_SETTING_BG), LV_PART_MAIN);
  lv_obj_set_style_bg_opa(strow, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_border_color(strow, rgb565_to_lv(COLOR_SETTING_BORDER), LV_PART_MAIN);
  lv_obj_set_style_border_width(strow, 1, LV_PART_MAIN);
  lv_obj_set_style_pad_all(strow, 2, LV_PART_MAIN);
  lv_obj_set_flex_flow(strow, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(strow, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

  lv_obj_t *bsu = lv_button_create(strow);
  lv_obj_set_size(bsu, 48, 40);
  station_arrow_btn_style(bsu);
  lv_obj_t *lsu = lv_label_create(bsu);
  lv_label_set_text(lsu, LV_SYMBOL_UP);
  lv_obj_center(lsu);
  lv_obj_add_event_cb(bsu, on_station_prev, LV_EVENT_CLICKED, NULL);

  lbl_station_radio = lv_label_create(strow);
  lv_obj_set_width(lbl_station_radio, 210);
  lv_label_set_long_mode(lbl_station_radio, LV_LABEL_LONG_DOT);
  lv_obj_set_style_text_align(lbl_station_radio, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
  lv_obj_set_style_text_font(lbl_station_radio, &font_radio_meta, LV_PART_MAIN);
  lv_obj_set_style_text_color(lbl_station_radio, lv_color_black(), LV_PART_MAIN);
  lv_obj_set_style_bg_opa(lbl_station_radio, LV_OPA_TRANSP, LV_PART_MAIN);

  lv_obj_t *bsd = lv_button_create(strow);
  lv_obj_set_size(bsd, 48, 40);
  station_arrow_btn_style(bsd);
  lv_obj_t *lsd = lv_label_create(bsd);
  lv_label_set_text(lsd, LV_SYMBOL_DOWN);
  lv_obj_center(lsd);
  lv_obj_add_event_cb(bsd, on_station_next, LV_EVENT_CLICKED, NULL);

  add_radio_footer(scr_radio, true);
}

static void config_row_slider(lv_obj_t *parent, const char *label, lv_obj_t **sl, lv_obj_t **val_lbl, int y0, int vmax) {
  lv_obj_t *box = lv_obj_create(parent);
  lv_obj_set_size(box, 320, 44);
  lv_obj_align(box, LV_ALIGN_TOP_LEFT, 0, y0);
  lv_obj_set_style_bg_color(box, rgb565_to_lv(COLOR_SETTING_BG), LV_PART_MAIN);
  lv_obj_set_style_bg_opa(box, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_border_color(box, rgb565_to_lv(COLOR_SETTING_BORDER), LV_PART_MAIN);
  lv_obj_set_style_border_width(box, 1, LV_PART_MAIN);
  lv_obj_set_style_pad_all(box, 4, LV_PART_MAIN);
  lv_obj_t *lb = lv_label_create(box);
  lv_label_set_text(lb, label);
  lv_obj_set_style_text_font(lb, &font_ui_primary, LV_PART_MAIN);
  lv_obj_set_style_text_color(lb, lv_color_black(), LV_PART_MAIN);
  lv_obj_align(lb, LV_ALIGN_TOP_LEFT, 4, 2);
  *val_lbl = lv_label_create(box);
  lv_label_set_text(*val_lbl, "--");
  lv_obj_set_style_text_font(*val_lbl, &font_ui_primary, LV_PART_MAIN);
  lv_obj_set_style_text_color(*val_lbl, lv_color_black(), LV_PART_MAIN);
  lv_obj_align(*val_lbl, LV_ALIGN_TOP_RIGHT, -6, 2);
  *sl = lv_slider_create(box);
  lv_obj_set_size(*sl, 300, 14);
  lv_obj_align(*sl, LV_ALIGN_BOTTOM_MID, 0, -4);
  lv_slider_set_range(*sl, 0, vmax);
  style_slider_light(*sl);
}

static void build_config_screen(void) {
  scr_config = lv_obj_create(NULL);
  lv_obj_set_style_bg_color(scr_config, rgb565_to_lv(COLOR_KNOEPFE_BG), LV_PART_MAIN);
  lv_obj_set_style_bg_opa(scr_config, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_pad_all(scr_config, 0, LV_PART_MAIN);

  config_row_slider(scr_config, "Lautstärke", &sl_cfg_gain, &lbl_cfg_gain_val, 0, 100);
  lv_obj_add_event_cb(sl_cfg_gain, on_gain_slider, LV_EVENT_VALUE_CHANGED, NULL);
  lv_obj_add_event_cb(sl_cfg_gain, on_gain_slider, LV_EVENT_RELEASED, NULL);

  config_row_slider(scr_config, "Helligkeit", &sl_cfg_bright, &lbl_cfg_bright_val, 44, 100);
  lv_obj_add_event_cb(sl_cfg_bright, on_bright_slider, LV_EVENT_VALUE_CHANGED, NULL);
  lv_obj_add_event_cb(sl_cfg_bright, on_bright_slider, LV_EVENT_RELEASED, NULL);

  config_row_slider(scr_config, "Einschlafzeit", &sl_cfg_snooze, &lbl_cfg_snooze_val, 88, 60);
  lv_obj_add_event_cb(sl_cfg_snooze, on_snooze_slider, LV_EVENT_VALUE_CHANGED, NULL);
  lv_obj_add_event_cb(sl_cfg_snooze, on_snooze_slider, LV_EVENT_RELEASED, NULL);

  lv_obj_t *strow = lv_obj_create(scr_config);
  lv_obj_remove_style_all(strow);
  lv_obj_set_size(strow, 320, 44);
  lv_obj_align(strow, LV_ALIGN_TOP_LEFT, 0, 132);
  lv_obj_set_style_bg_color(strow, rgb565_to_lv(COLOR_SETTING_BG), LV_PART_MAIN);
  lv_obj_set_style_bg_opa(strow, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_border_color(strow, rgb565_to_lv(COLOR_SETTING_BORDER), LV_PART_MAIN);
  lv_obj_set_style_border_width(strow, 1, LV_PART_MAIN);
  lv_obj_set_style_pad_all(strow, 2, LV_PART_MAIN);
  lv_obj_set_flex_flow(strow, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(strow, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

  lv_obj_t *bsu = lv_button_create(strow);
  lv_obj_set_size(bsu, 48, 40);
  station_arrow_btn_style(bsu);
  lv_obj_t *lsu = lv_label_create(bsu);
  lv_label_set_text(lsu, LV_SYMBOL_UP);
  lv_obj_center(lsu);
  lv_obj_add_event_cb(bsu, on_cfg_station_prev, LV_EVENT_CLICKED, NULL);

  lbl_station_cfg = lv_label_create(strow);
  lv_obj_set_width(lbl_station_cfg, 210);
  lv_label_set_long_mode(lbl_station_cfg, LV_LABEL_LONG_DOT);
  lv_obj_set_style_text_align(lbl_station_cfg, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
  lv_obj_set_style_text_font(lbl_station_cfg, &font_radio_meta, LV_PART_MAIN);
  lv_obj_set_style_text_color(lbl_station_cfg, lv_color_black(), LV_PART_MAIN);
  lv_obj_set_style_bg_opa(lbl_station_cfg, LV_OPA_TRANSP, LV_PART_MAIN);

  lv_obj_t *bsd = lv_button_create(strow);
  lv_obj_set_size(bsd, 48, 40);
  station_arrow_btn_style(bsd);
  lv_obj_t *lsd = lv_label_create(bsd);
  lv_label_set_text(lsd, LV_SYMBOL_DOWN);
  lv_obj_center(lsd);
  lv_obj_add_event_cb(bsd, on_cfg_station_next, LV_EVENT_CLICKED, NULL);

  lv_obj_t *row = make_footer_nav(scr_config);
  lv_obj_align(row, LV_ALIGN_BOTTOM_MID, 0, 0);
  const char *syms[] = {LV_SYMBOL_POWER, "Zz", LV_SYMBOL_BELL, LV_SYMBOL_NEXT, LV_SYMBOL_LIST};
  lv_event_cb_t cbs[] = {evt_footer_power, evt_footer_snooze, evt_footer_alarm, evt_footer_mid_cfg, evt_footer_alarm_from_cfg};
  lv_obj_t *fb0 = NULL, *fx0 = NULL, *fb1 = NULL, *fx1 = NULL, *fb2 = NULL, *fx2 = NULL;
  for (int i = 0; i < 5; i++) {
    lv_obj_t *b = lv_button_create(row);
    footer_style_btn(b);
    lv_obj_t *x = lv_label_create(b);
    lv_label_set_text(x, syms[i]);
    lv_obj_center(x);
    lv_obj_add_event_cb(b, cbs[i], LV_EVENT_CLICKED, NULL);
    if (i == 0) {
      fb0 = b;
      fx0 = x;
    } else if (i == 1) {
      fb1 = b;
      fx1 = x;
    } else if (i == 2) {
      fb2 = b;
      fx2 = x;
    }
  }
  ui_footer_register_state_row(fb0, fx0, fb1, fx1, fb2, fx2);
}

static void build_alarm_screen(void) {
  scr_alarm = lv_obj_create(NULL);
  lv_obj_set_style_bg_color(scr_alarm, rgb565_to_lv(COLOR_KNOEPFE_BG), LV_PART_MAIN);
  lv_obj_set_style_bg_opa(scr_alarm, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_pad_all(scr_alarm, 0, LV_PART_MAIN);

  lv_obj_t *z1 = lv_obj_create(scr_alarm);
  lv_obj_set_size(z1, 320, 88);
  lv_obj_align(z1, LV_ALIGN_TOP_LEFT, 0, 0);
  lv_obj_set_style_bg_color(z1, rgb565_to_lv(COLOR_SETTING_BG), LV_PART_MAIN);
  lv_obj_set_style_bg_opa(z1, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_border_width(z1, 1, LV_PART_MAIN);
  lv_obj_set_style_border_color(z1, rgb565_to_lv(COLOR_SETTING_BORDER), LV_PART_MAIN);

  lv_obj_t *z2 = lv_obj_create(scr_alarm);
  lv_obj_set_size(z2, 320, 88);
  lv_obj_align(z2, LV_ALIGN_TOP_LEFT, 0, 88);
  lv_obj_set_style_bg_color(z2, rgb565_to_lv(COLOR_SETTING_BG), LV_PART_MAIN);
  lv_obj_set_style_bg_opa(z2, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_border_width(z2, 1, LV_PART_MAIN);
  lv_obj_set_style_border_color(z2, rgb565_to_lv(COLOR_SETTING_BORDER), LV_PART_MAIN);

  for (uint8_t row = 0; row < 2; row++) {
    lv_obj_t *zone = (row == 0) ? z1 : z2;
    uint8_t y0 = (uint8_t)(8 + row * 88);
    for (uint8_t col = 0; col < 7; col++) {
      al_day_btn[row][col] = lv_button_create(scr_alarm);
      lv_obj_set_size(al_day_btn[row][col], 40, 28);
      lv_obj_align(al_day_btn[row][col], LV_ALIGN_TOP_LEFT, 10 + col * 42, y0);
      lv_obj_set_style_bg_color(al_day_btn[row][col], rgb565_to_lv(COLOR_SETTING_BG), LV_PART_MAIN);
      lv_obj_set_style_text_color(al_day_btn[row][col], lv_color_black(), LV_PART_MAIN);
      lv_obj_t *lb = lv_label_create(al_day_btn[row][col]);
      lv_label_set_text(lb, days_short[(col + 1) % 7]);
      lv_obj_set_style_text_color(lb, lv_color_black(), LV_PART_MAIN);
      lv_obj_center(lb);
      uintptr_t ud = ((uintptr_t)row << 8) | col;
      lv_obj_add_event_cb(al_day_btn[row][col], on_alarm_day, LV_EVENT_CLICKED, (void *)ud);
    }
  }

  lbl_al_t1 = lv_label_create(scr_alarm);
  lv_obj_align(lbl_al_t1, LV_ALIGN_TOP_MID, 0, 52);
  lv_obj_set_style_text_font(lbl_al_t1, &font_ui_primary, LV_PART_MAIN);
  lv_obj_set_style_text_color(lbl_al_t1, lv_color_black(), LV_PART_MAIN);

  lbl_al_t2 = lv_label_create(scr_alarm);
  lv_obj_align(lbl_al_t2, LV_ALIGN_TOP_MID, 0, 140);
  lv_obj_set_style_text_font(lbl_al_t2, &font_ui_primary, LV_PART_MAIN);
  lv_obj_set_style_text_color(lbl_al_t2, lv_color_black(), LV_PART_MAIN);

  auto mkpair = [&](lv_coord_t x, lv_coord_t y, lv_event_cb_t up, lv_event_cb_t dn) {
    lv_obj_t *bu = lv_button_create(scr_alarm);
    lv_obj_set_size(bu, 44, 36);
    lv_obj_align(bu, LV_ALIGN_TOP_LEFT, x, y);
    station_arrow_btn_style(bu);
    lv_obj_t *lu = lv_label_create(bu);
    lv_label_set_text(lu, LV_SYMBOL_UP);
    lv_obj_center(lu);
    lv_obj_add_event_cb(bu, up, LV_EVENT_CLICKED, NULL);
    lv_obj_t *bd = lv_button_create(scr_alarm);
    lv_obj_set_size(bd, 44, 36);
    lv_obj_align(bd, LV_ALIGN_TOP_LEFT, x + 48, y);
    station_arrow_btn_style(bd);
    lv_obj_t *ld = lv_label_create(bd);
    lv_label_set_text(ld, LV_SYMBOL_DOWN);
    lv_obj_center(ld);
    lv_obj_add_event_cb(bd, dn, LV_EVENT_CLICKED, NULL);
  };
  mkpair(4, 44, on_al_h_up1, on_al_h_dn1);
  mkpair(214, 44, on_al_m_up1, on_al_m_dn1);
  mkpair(4, 132, on_al_h_up2, on_al_h_dn2);
  mkpair(214, 132, on_al_m_up2, on_al_m_dn2);

  lv_obj_t *row = make_footer_nav(scr_alarm);
  lv_obj_align(row, LV_ALIGN_BOTTOM_MID, 0, 0);
  lv_obj_t *b0 = lv_button_create(row);
  footer_style_btn(b0);
  lv_obj_t *x0 = lv_label_create(b0);
  lv_label_set_text(x0, LV_SYMBOL_POWER);
  lv_obj_center(x0);
  lv_obj_add_event_cb(b0, evt_footer_power, LV_EVENT_CLICKED, NULL);
  lv_obj_t *b1 = lv_button_create(row);
  footer_style_btn(b1);
  lv_obj_t *x1 = lv_label_create(b1);
  lv_label_set_text(x1, "Zz");
  lv_obj_center(x1);
  lv_obj_add_event_cb(b1, evt_footer_snooze, LV_EVENT_CLICKED, NULL);
  lv_obj_t *b2 = lv_button_create(row);
  footer_style_btn(b2);
  lv_obj_t *x2 = lv_label_create(b2);
  lv_label_set_text(x2, LV_SYMBOL_BELL);
  lv_obj_center(x2);
  lv_obj_add_event_cb(b2, evt_footer_alarm, LV_EVENT_CLICKED, NULL);
  lv_obj_t *b3 = lv_button_create(row);
  footer_style_btn(b3);
  lv_obj_t *x3 = lv_label_create(b3);
  lv_label_set_text(x3, LV_SYMBOL_OK);
  lv_obj_center(x3);
  lv_obj_add_event_cb(b3, evt_footer_save_alarm, LV_EVENT_CLICKED, NULL);
  lv_obj_t *b4 = lv_button_create(row);
  footer_style_btn(b4);
  lv_obj_t *x4 = lv_label_create(b4);
  lv_label_set_text(x4, LV_SYMBOL_HOME);
  lv_obj_center(x4);
  lv_obj_add_event_cb(b4, evt_footer_home_alarm, LV_EVENT_CLICKED, NULL);

  ui_footer_register_state_row(b0, x0, b1, x1, b2, x2);
}

static void ui_refresh_header(void) {
  if (!lbl_rssi || !lbl_ip || !lbl_snooze || !lbl_alarm_hdr) return;
  int rssi = WiFi.RSSI();
  char rssiChar[8];
  snprintf(rssiChar, sizeof(rssiChar), "%d", rssi);
  lv_color_t wc = lv_color_white();
  if (rssi <= -70) wc = lv_palette_main(LV_PALETTE_RED);
  else if (rssi <= -50) wc = lv_palette_main(LV_PALETTE_YELLOW);
  else wc = lv_palette_main(LV_PALETTE_GREEN);
  lv_obj_set_style_text_color(lbl_rssi, wc, LV_PART_MAIN);
  lv_label_set_text(lbl_rssi, rssiChar);

  IPAddress ip = WiFi.localIP();
  char ipstr[20];
  snprintf(ipstr, sizeof(ipstr), "%u.%u.%u.%u", (unsigned)ip[0], (unsigned)ip[1], (unsigned)ip[2], (unsigned)ip[3]);
  lv_label_set_text(lbl_ip, ipstr);

  if (snoozeTimeEnd != 0)
    lv_obj_set_style_text_color(lbl_snooze, rgb565_to_lv(COLOR_SLEEP_SYMBOL), LV_PART_MAIN);
  else
    lv_obj_set_style_text_color(lbl_snooze, rgb565_to_lv(COLOR_BG), LV_PART_MAIN);

  if (startpage) {
    char txt[50] = "";
    lv_color_t ac = rgb565_to_lv(COLOR_ALARM_SYMBOL);
    if (alarmday < 8) {
      uint8_t h = alarmtime / 60;
      uint8_t m = alarmtime % 60;
      snprintf(txt, sizeof(txt), "%s %02u:%02u", days_short[alarmday], h, m);
    } else {
      ac = lv_palette_main(LV_PALETTE_RED);
      snprintf(txt, sizeof(txt), "AUS");
    }
    lv_obj_set_style_text_color(lbl_alarm_hdr, ac, LV_PART_MAIN);
    lv_label_set_text(lbl_alarm_hdr, txt);
  }
}

static void ui_refresh_mid_clock(void) {
  if (!mid_radio || !mid_weather) return;
  if (radio) {
    lv_obj_add_flag(mid_weather, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(mid_radio, LV_OBJ_FLAG_HIDDEN);
    lv_label_set_text(lbl_station_big, stationlist[actStation].name);
    lv_label_set_text(lbl_stream_title, title);
    lv_obj_scroll_to_y(lbl_stream_title, 0, LV_ANIM_OFF);
  } else {
    lv_obj_clear_flag(mid_weather, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(mid_radio, LV_OBJ_FLAG_HIDDEN);
  }
}

void touch_loop(void) {
  /* Tick: lv_tick_set_cb(millis) nach lv_init() — kein lv_tick_inc nötig */
  lv_timer_handler();
}

void setup_display(void) {
  pinMode(TFT_LED, OUTPUT);
  pinMode(LDR, INPUT);
  lastldr = analogRead(LDR);
  setBGLight(100);

  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(ILI9341_BLACK);

  touch.begin();

  lv_init();
  lv_tick_set_cb(lvgl_tick_get_cb);

  font_ui_primary = lv_font_montserrat_14;
  font_ui_primary.fallback = &lv_font_de_supp_14;

  font_clock_digits = &lv_font_clock_digits;
  font_clock_date = lv_font_montserrat_18;
  font_clock_date.fallback = &lv_font_de_supp_14;
  font_radio_meta = lv_font_montserrat_18;
  font_radio_meta.fallback = &lv_font_de_supp_14;

  lv_disp_main = lv_display_create(320, 240);
  lv_display_set_flush_cb(lv_disp_main, disp_flush);
  lv_display_set_buffers(lv_disp_main, buf1, NULL, sizeof(buf1), LV_DISPLAY_RENDER_MODE_PARTIAL);

  lv_indev_t *indev = lv_indev_create();
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
  lv_indev_set_read_cb(indev, touch_read);
  lv_indev_set_display(indev, lv_disp_main);

  ui_footer_icons_reset();
  build_msg_screen();
  yield();
  build_ota_screen();
  yield();
  build_clock_screen();
  yield();
  build_radio_screen();
  yield();
  build_config_screen();
  yield();
  build_alarm_screen();
  yield();

  ui_refresh_footer_icons();
  ui_sync_all_gain_sliders();
  lv_slider_set_value(sl_cfg_bright, bright, LV_ANIM_OFF);
  lv_slider_set_value(sl_cfg_snooze, snoozeTime, LV_ANIM_OFF);
  cfg_sync_value_labels();
}

void displayClear(void) {
  if (msg_title) lv_label_set_text(msg_title, "");
  if (msg_line2) lv_label_set_text(msg_line2, "");
  if (msg_line3) lv_label_set_text(msg_line3, "");
  if (msg_line4) lv_label_set_text(msg_line4, "");
  if (msg_line5) lv_label_set_text(msg_line5, "");
  if (scr_msg) {
    lv_obj_set_style_bg_color(scr_msg, rgb565_to_lv(COLOR_KNOEPFE_BG), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(scr_msg, LV_OPA_COVER, LV_PART_MAIN);
  }
}

void displayMessage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const char *text, uint8_t align, boolean big,
                    uint16_t fc, uint16_t bg, uint8_t lines) {
  (void)x;
  (void)w;
  (void)h;
  (void)lines;
  lv_obj_t *target = msg_title;
  if (y > 170) target = msg_line5;
  else if (y > 130) target = msg_line4;
  else if (y > 85) target = msg_line3;
  else if (y > 35) target = msg_line2;
  else target = msg_title;

  lv_label_set_text(target, text ? text : "");
  lv_obj_set_style_text_color(target, rgb565_to_lv(fc), LV_PART_MAIN);
  lv_obj_set_style_text_font(target, &font_ui_primary, LV_PART_MAIN);
  lv_obj_set_style_text_align(target,
                              align == ALIGNCENTER ? LV_TEXT_ALIGN_CENTER : (align == ALIGNRIGHT ? LV_TEXT_ALIGN_RIGHT : LV_TEXT_ALIGN_LEFT),
                              LV_PART_MAIN);
  lv_obj_set_style_bg_color(scr_msg, rgb565_to_lv(bg), LV_PART_MAIN);
  lv_screen_load(scr_msg);
  lv_timer_handler();
}

void ota_ui_begin(const char *title) {
  if (!scr_ota) return;
  lv_label_set_text(ota_lbl, title ? title : "OTA");
  lv_label_set_text(ota_sub, "");
  lv_bar_set_value(ota_bar, 0, LV_ANIM_OFF);
  lv_screen_load(scr_ota);
  lv_timer_handler();
}

void ota_ui_set_sub(const char *sub) {
  if (ota_sub) lv_label_set_text(ota_sub, sub ? sub : "");
  lv_timer_handler();
}

void textInBox(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const char *text, uint8_t align, boolean big, uint16_t fc, uint16_t bg,
               uint8_t lines) {
  (void)lines;
  displayMessage(x, y, w, h, text, align, big, fc, bg, 1);
}

static void ui_clock_digits_set(uint8_t hour, uint8_t minute) {
  if (!clock_digit_lbl[0]) return;
  char newd[4];
  newd[0] = (char)('0' + (hour / 10) % 10);
  newd[1] = (char)('0' + hour % 10);
  newd[2] = (char)('0' + (minute / 10) % 10);
  newd[3] = (char)('0' + minute % 10);
  for (int i = 0; i < 4; i++) {
    char s[2] = {newd[i], '\0'};
    lv_label_set_text(clock_digit_lbl[i], s);
  }
}

void updateTime(boolean redraw) {
  char tim[40];
  if (!getLocalTime(&ti)) {
    ui_refresh_footer_icons();
    return;
  }

  sprintf(tim, "%s %i. %s %i", days[ti.tm_wday], ti.tm_mday, months[ti.tm_mon], ti.tm_year + 1900);
  if (redraw || strcmp(tim, lastdate) != 0) {
    strncpy(lastdate, tim, sizeof(lastdate) - 1);
    lastdate[sizeof(lastdate) - 1] = 0;
    lv_label_set_text(lbl_date, tim);
  }

  strftime(tim, sizeof(tim), "%H:%M", &ti);
  if (redraw || strcmp(tim, lasttime) != 0) {
    strncpy(lasttime, tim, sizeof(lasttime) - 1);
    lasttime[sizeof(lasttime) - 1] = 0;
    ui_clock_digits_set((uint8_t)ti.tm_hour, (uint8_t)ti.tm_min);
    ui_refresh_header();
  }
  ui_refresh_footer_icons();
}

void displayDateTime(void) {
  updateTime(false);
}

void showProgress(uint32_t prc) {
  if (lv_screen_active() != scr_ota) lv_screen_load(scr_ota);
  lv_bar_set_value(ota_bar, (int)prc, LV_ANIM_OFF);
  char tmp[24];
  snprintf(tmp, sizeof(tmp), "%u%%", (unsigned)prc);
  lv_label_set_text(ota_sub, tmp);
  lv_timer_handler();
}

void showSlider(uint16_t y, float value, uint16_t vmax, uint16_t bgColor, uint16_t lineColor) {
  (void)y;
  (void)bgColor;
  (void)lineColor;
  curGain = (uint8_t)constrain((int)value, 0, 100);
  ui_sync_all_gain_sliders();
  (void)vmax;
}

void setGainValue(uint16_t value, const char *sliderPage) {
  float v;
  if (strcmp(sliderPage, "WEBSITE") != 0) {
    int y_start = 11;
    int line_length = 298;
    float start_slider = (float)value - y_start;
    float end_slider = (float)line_length - y_start;
    v = start_slider / end_slider * 100.0f;
    if (v > 100) v = 100;
    if (v < 0) v = 0;
  } else {
    v = (float)value;
  }
  curGain = (uint8_t)constrain((int)v, 0, 100);
  pref.putUShort("gain", curGain);
  ui_sync_all_gain_sliders();
  setGain(curGain);
}

void setBrightness(uint16_t value) {
  float vf = (value - 15) * 0.345f;
  if (vf > 100) vf = 100;
  if (vf < 5) vf = 0;
  bright = (uint8_t)vf;
  pref.putUShort("bright", bright);
  ui_slider_internal = true;
  if (sl_cfg_bright) lv_slider_set_value(sl_cfg_bright, bright, LV_ANIM_OFF);
  ui_slider_internal = false;
  setBGLight(bright == 0 ? 0 : bright);
}

void setSnoozeTime(uint16_t value) {
  float vf = (value - 15) * 0.21f;
  if (vf > 60) vf = 60;
  if (vf < 0) vf = 0;
  snoozeTime = (uint8_t)vf;
  pref.putUShort("snooze", snoozeTime);
  ui_slider_internal = true;
  if (sl_cfg_snooze) lv_slider_set_value(sl_cfg_snooze, snoozeTime, LV_ANIM_OFF);
  ui_slider_internal = false;
}

void setBGLight(uint8_t prct) {
  uint16_t ledb;
  /* LDR (prct==0): nicht bei jedem Loop/Tick neu schreiben → weniger Flackern; Intervall ~10 s.
     Direkt nach Wechsel von fester Helligkeit → 0 % aber sofort anwenden (sonst wirkte LDR oft „tot“). */
  static unsigned long prevLdrMs = 0;
  static bool lastWasManualBright = true;
  if (prct == 0) {
    bool enteringLdr = lastWasManualBright;
    if (!enteringLdr && (millis() - prevLdrMs < 10000UL)) return;
    prevLdrMs = millis();
    lastWasManualBright = false;
    ledb = (uint16_t)(analogRead(LDR) * 255 / 4096);
  } else {
    lastWasManualBright = true;
    ledb = (uint16_t)(255 * prct / 100);
  }
  if (ledb > 255) ledb = 255;
  /* Mindest-PWM nur bei fester Prozent-Helligkeit; LDR soll auch sehr dunkel dürfen. */
  if (prct != 0 && ledb < 3) ledb = 3;
  if (LED_ON == 0) ledb = (uint16_t)(255 - ledb);
  if (ledb != lastLedb) {
    RADIO_SERIAL(Serial.printf("percent = %i LED = %i\n", prct, ledb));
    analogWrite(TFT_LED, ledb);
    lastLedb = ledb;
  }
}

void selectStation(uint16_t x) {
  if (x < 50) cfg_station_prev_action();
  else if (x > 270) cfg_station_next_action();
}

void toggleRadio(boolean off) {
  if (off) {
    stopPlaying();
    snoozeTimeEnd = 0;
    radio = false;
  } else {
    if (connected) {
      if (startUrl(stationlist[actStation].url)) {
        radio = true;
        setGain(curGain);
      } else {
        radio = false;
        RADIO_SERIAL(Serial.println(F("toggleRadio: Stream-Start fehlgeschlagen")));
      }
    }
  }
  startpage = true;
  showStartPage();
}

void toggleAlarm(void) {
  if (alarmday > 7) {
    findNextAlarm();
    pref.putBool("alarmon", true);
  } else {
    alarmday = 8;
    pref.putBool("alarmon", false);
  }
  startpage = true;
  showStartPage();
}

void startSnooze(void) {
  /* Läuft bereits: nur Timer; Ende → toggleRadio(true) im Hauptschleifen-Timer.
     Aus: einschalten, dann Timer; Ende → wieder aus. */
  if (radio) {
    snoozeTimeEnd = millis() + (uint32_t)snoozeTime * 60000UL;
    ui_refresh_footer_icons();
    ui_refresh_header();
  } else {
    toggleRadio(false);
    snoozeTimeEnd = millis() + (uint32_t)snoozeTime * 60000UL;
  }
}

void changeStation(void) {
  actStation = curStation;
  pref.putUShort("station", curStation);
  if (startUrl(stationlist[actStation].url)) {
    radio = true;
  } else {
    radio = false;
    RADIO_SERIAL(Serial.println(F("changeStation: Stream-Start fehlgeschlagen (Sender unverändert gespeichert).")));
  }
  startpage = true;
  showStartPage();
}

void updateStation(void) {
  char txt[40];
  strncpy(txt, stationlist[curStation].name, sizeof(txt) - 1);
  txt[sizeof(txt) - 1] = 0;
  if (lbl_station_radio) lv_label_set_text(lbl_station_radio, txt);
  if (lbl_station_cfg) lv_label_set_text(lbl_station_cfg, txt);
}

void showStationList(void) {
  curStation = actStation;
  updateStation();
}

void showGain(void) {
  ui_sync_all_gain_sliders();
  cfg_sync_value_labels();
}

void showBrightness(void) {
  ui_slider_internal = true;
  if (sl_cfg_bright) lv_slider_set_value(sl_cfg_bright, bright, LV_ANIM_OFF);
  ui_slider_internal = false;
  cfg_sync_value_labels();
}

void showSnoozeTime(void) {
  ui_slider_internal = true;
  if (sl_cfg_snooze) lv_slider_set_value(sl_cfg_snooze, snoozeTime, LV_ANIM_OFF);
  ui_slider_internal = false;
  cfg_sync_value_labels();
}

void showFavoritenPage(void) {
  setBGLight(100);
  startpage = false;
  favoritenpage = true;
  settingspage = false;
  alarmpage = false;
  showGain();
  showBrightness();
  showSnoozeTime();
  showStationList();
  lv_screen_load(scr_config);
  ui_refresh_footer_icons();
  start_conf = millis();
}

void showSettingsPage(void) {
  setBGLight(100);
  startpage = false;
  favoritenpage = false;
  settingspage = true;
  alarmpage = false;
  showGain();
  rebuild_favorites();
  showStationList();
  lv_screen_load(scr_radio);
  ui_refresh_footer_icons();
  start_conf = millis();
}

void showAlarms_Day_and_Time(void) {
  char buf[24];
  for (uint8_t row = 0; row < 2; row++) {
    uint8_t maskbyte = (row == 0) ? alarmConfig.alarmday_1 : alarmConfig.alarmday_2;
    for (uint8_t col = 0; col < 7; col++) {
      uint8_t m = day_mask_from_col(col);
      bool on = (maskbyte & m) != 0;
      lv_obj_set_style_bg_color(al_day_btn[row][col], on ? lv_palette_main(LV_PALETTE_GREEN) : rgb565_to_lv(COLOR_SETTING_BG), LV_PART_MAIN);
    }
  }
  snprintf(buf, sizeof(buf), "%02u : %02u", alarmConfig.h_1, alarmConfig.m_1);
  lv_label_set_text(lbl_al_t1, buf);
  snprintf(buf, sizeof(buf), "%02u : %02u", alarmConfig.h_2, alarmConfig.m_2);
  lv_label_set_text(lbl_al_t2, buf);
}

void showAlarmPage(void) {
  alarmConfig.alarmday_1 = alarmday1;
  alarmConfig.h_1 = (uint8_t)(alarm1 / 60);
  alarmConfig.m_1 = (uint8_t)(alarm1 % 60);
  alarmConfig.alarmday_2 = alarmday2;
  alarmConfig.h_2 = (uint8_t)(alarm2 / 60);
  alarmConfig.m_2 = (uint8_t)(alarm2 % 60);
  setBGLight(100);
  startpage = false;
  favoritenpage = false;
  settingspage = false;
  alarmpage = true;
  showAlarms_Day_and_Time();
  lv_screen_load(scr_alarm);
  ui_refresh_footer_icons();
  start_conf = millis();
}

static void ok_timer_cb(lv_timer_t *t) {
  (void)t;
  showStartPage();
}

void safeAlarmTime(void) {
  start_conf -= 9000;
  uint16_t alarmtime_1 = (uint16_t)(alarmConfig.h_1 * 60 + alarmConfig.m_1);
  uint16_t alarmtime_2 = (uint16_t)(alarmConfig.h_2 * 60 + alarmConfig.m_2);
  alarm1 = alarmtime_1;
  alarmday1 = alarmConfig.alarmday_1;
  pref.putUInt("alarm1", alarm1);
  pref.putUShort("alarmday1", alarmday1);
  alarm2 = alarmtime_2;
  alarmday2 = alarmConfig.alarmday_2;
  pref.putUInt("alarm2", alarm2);
  pref.putUShort("alarmday2", alarmday2);
  lv_obj_set_style_bg_color(scr_msg, rgb565_to_lv(COLOR_KNOEPFE_BG), LV_PART_MAIN);
  lv_obj_set_style_bg_opa(scr_msg, LV_OPA_COVER, LV_PART_MAIN);
  lv_label_set_text(msg_title, TXT_OK);
  lv_obj_set_style_text_color(msg_title, lv_palette_main(LV_PALETTE_GREEN), LV_PART_MAIN);
  lv_obj_set_style_text_font(msg_title, &font_ui_primary, LV_PART_MAIN);
  lv_obj_set_style_text_align(msg_title, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
  lv_label_set_text(msg_line2, "");
  lv_label_set_text(msg_line3, "");
  lv_screen_load(scr_msg);
  findNextAlarm();
  lv_timer_t *once = lv_timer_create(ok_timer_cb, 1500, NULL);
  lv_timer_set_repeat_count(once, 1);
}

void toggleAlarmDay(uint16_t xPos, uint16_t yPos) {
  (void)xPos;
  (void)yPos;
}

void in_de_crementAlarmTimeHH(uint16_t xPos, uint16_t yPos) {
  (void)xPos;
  (void)yPos;
}

void in_de_crementAlarmTimeMM(uint16_t xPos, uint16_t yPos) {
  (void)xPos;
  (void)yPos;
}

void showStation(void) {
  if (lbl_station_big) lv_label_set_text(lbl_station_big, stationlist[actStation].name);
}

void showTitle(void) {
  if (lbl_stream_title) {
    lv_label_set_text(lbl_stream_title, title);
    lv_obj_scroll_to_y(lbl_stream_title, 0, LV_ANIM_OFF);
  }
}

void showRadio(void) {
  ui_refresh_mid_clock();
}

void showNextAlarm(void) {
  ui_refresh_header();
}

void showStartPage(void) {
  start_conf = 0;
  startpage = true;
  favoritenpage = false;
  settingspage = false;
  alarmpage = false;
  setBGLight(bright);
  lv_screen_load(scr_clock);
  lastdate[0] = 0;
  lasttime[0] = 0;
  updateTime(true);
  ui_refresh_mid_clock();
  ui_refresh_header();
  ui_sync_all_gain_sliders();
}

void FavoriteButtons(void) {
  rebuild_favorites();
}

void displayWeather(const OpenMeteoTemps *t) {
  if (!t) return;

  char lt[48], lf[48], dmin[48], dmax[48], mmin[48], mmax[48];
  snprintf(lt, sizeof(lt), "%s %d C", TXT_TEMP, (int)t->current_c);
  snprintf(lf, sizeof(lf), "%s %d C", TXT_FEELS, (int)t->feels_c);
  snprintf(dmin, sizeof(dmin), "%s %d C", TXT_MIN, (int)t->today_min);
  snprintf(dmax, sizeof(dmax), "%s %d C", TXT_MAX, (int)t->today_max);
  snprintf(mmin, sizeof(mmin), "%s %d C", TXT_MIN, (int)t->tomorrow_min);
  snprintf(mmax, sizeof(mmax), "%s %d C", TXT_MAX, (int)t->tomorrow_max);
  lv_label_set_text(w_lbl_now_t, lt);
  lv_label_set_text(w_lbl_now_f, lf);
  lv_label_set_text(w_lbl_td_min, dmin);
  lv_label_set_text(w_lbl_td_max, dmax);
  lv_label_set_text(w_lbl_tm_min, mmin);
  lv_label_set_text(w_lbl_tm_max, mmax);
}

void drawHeaderInfos(void) {
  ui_refresh_header();
}

void drawWifiInfo(void) {
  ui_refresh_header();
}

void drawSnoozeInfo(void) {
  ui_refresh_header();
}

void HandleFavoriteButtons(int xpos, int ypos) {
  (void)xpos;
  (void)ypos;
}

void DrawFooterButtons_Power_Sleep_Alarm(void) {
  ui_refresh_footer_icons();
}

void onTouchClick(TS_Point p) {
  (void)p;
}
