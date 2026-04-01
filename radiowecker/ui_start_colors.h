/* Nur Typ + extern — darf in mehreren .ino eingebunden werden ohne Doppel-Definitionen
   (00_settings.h definiert viele Globals; niemals zweimal einbinden). */
#ifndef UI_START_COLORS_H
#define UI_START_COLORS_H

#include <stdint.h>

typedef struct {
  uint16_t bg;
  uint16_t ip;
  uint16_t sleep_sym;
  uint16_t alarm_sym;
  uint16_t slider;
  uint16_t slider_bg;
  uint16_t slider_border;
  uint16_t date;
  uint16_t time_c;
  uint16_t box_bg;
  uint16_t box_border;
  uint16_t station_name;
  uint16_t station_title;
} UiStartColors;

extern UiStartColors ui_start_cols;

#endif
