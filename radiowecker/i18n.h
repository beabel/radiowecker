#ifndef I18N_H
#define I18N_H

#include <stdint.h>

/** UI-Sprache (NVS-Key "uilang", 0–5: de,en,fr,ru,es,nl). TFT: UTF-8, Fallback Montserrat 14 → lv_font_latin_supp_14 (u. a. ES/FR/DE/NL, … « ») → lv_font_cyrl_supp_14 (RU). */
enum i18n_string_id : uint16_t {
  I18N_DISCONNECT = 0,
  I18N_SOFT_AP_DISCONNECT,
  I18N_SET_WLAN_MODE,
  I18N_CONNECTING_TO,
  I18N_CONNECTING_ESTABLISH,
  I18N_NO_CONNECTION,
  I18N_IP_ADDRESS,
  I18N_RECONNECTED,
  I18N_NOT_CONNECTED,
  I18N_CONNECT_TO_AP,
  I18N_CONFIG_IP,
  I18N_OK,
  I18N_NOW,
  I18N_TODAY,
  I18N_TOMORROW,
  I18N_TEMP,
  I18N_FEELS,
  I18N_MIN_W,
  I18N_MAX_W,
  I18N_CFG_VOLUME,
  I18N_CFG_BRIGHT,
  I18N_CFG_SNOOZE,
  I18N_CFG_ALARM_SNOOZE,
  I18N_CFG_ALARM_GAIN,
  I18N_CFG_LANG,
  I18N_ALARM_OFF,
  I18N_SNOOZE_OFF,
  I18N_UNIT_MIN,
  I18N_OTA_HTTP_FW,
  I18N_OTA_LOADING,
  I18N_OTA_CONN_WLAN,
  I18N_OTA_DL_FW,
  I18N_OTA_ABORTED,
  I18N_OTA_INV_TAG,
  I18N_OTA_FAILED,
  I18N_OTA_NO_WLAN,
  I18N_OTA_URL_LONG,
  I18N_OTA_RESTART,
  I18N_OTA_CODE,
  I18N_ERR_NO_STATION,
  I18N_STR_COUNT
};

void i18n_begin(void);
uint8_t i18n_get_lang(void);
void i18n_set_lang(uint8_t lang);
bool i18n_take_ui_dirty(void);

const char *i18n_str(uint16_t id);
const char *i18n_day_long(int wday);
const char *i18n_day_short(int wday);
const char *i18n_month_short(int mon);

/** Zwei-Buchstaben-Code für JSON/Web */
const char *i18n_lang_code(void);

#endif
