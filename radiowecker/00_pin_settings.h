// Pin-Definitionen für verschiedene Komponenten des Projekts

// Pin für den Lichtsensor (LDR)
#define LDR 36

// Pin für den Buzzer auf dem originalen Az-Delivery TouchMod
#define BEEPER 21

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