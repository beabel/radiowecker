// Pin-Definitionen für verschiedene Komponenten des Projekts

// Pin für den Lichtsensor (LDR)
#define LDR 36

// Audio-Pins
#define LRCLK 25  // Left-Right Clock für die Audioausgabe
#define BCLK 26   // Bit Clock für die Audioausgabe
#define DOUT 33   // Daten-Ausgang für die Audioausgabe

// Pins für den Touchscreen
#define TOUCH_CS 14   // Chip Select für den Touchscreen
#define TOUCH_IRQ 27  // Interrupt-Anschluss für den Touchscreen

// Pins für das TFT-Display
#define TFT_CS 5    // Chip Select für das TFT-Display
#define TFT_DC 4    // Data/Command Pin für das TFT-Display
#define TFT_RST 22  // Reset-Pin für das TFT-Display
#define TFT_LED 15  // Hintergrundbeleuchtung des TFT-Displays
#define LED_ON 0    // Logikpegel für die Hintergrundbeleuchtung (0 = Ein)

/* Optionale Hardware-Tasten (nach GND, INPUT_PULLUP): Pin < 0 = aus / nicht angeschlossen (Default).
   Stop: immer wie Software-Stop (Radio aus, ggf. Wecker-Schlummer abbrechen).
   Schlummer: nur ausgewertet, wenn alarmActionsVisible (Wecker-Overlay). */
#ifndef ALARM_HW_BTN_STOP_PIN
#define ALARM_HW_BTN_STOP_PIN -1
#endif
#ifndef ALARM_HW_BTN_SNOOZE_PIN
#define ALARM_HW_BTN_SNOOZE_PIN -1
#endif