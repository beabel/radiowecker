#include "fonts.h"              // Header für Schriftarten
#include "tft_color_setting.h"  // Header für TFT-Farbkonstanten
#include "knoepfe.h"            // Grafikdaten für Buttons
#include "symbole.h"            // Grafikdaten für Symbole
#include "num_64_44.h"          // Grafikdaten für Favoriten-Buttons

// Struktur zur Speicherung der Alarmkonfiguration
typedef struct {
  uint8_t alarmday_1 = 0B00111110;  // Gültige Wochentage für Alarm 1 (Beispiel 00111110 bedeutet Montag bis Freitag)
  uint8_t h_1;                      // Alarmstunde für Alarm 1
  uint8_t m_1;                      // Alarmminute für Alarm 1
  uint8_t alarmday_2 = 0B00111110;  // Gültige Wochentage für Alarm 2 (Beispiel 00111110 bedeutet Montag bis Freitag)
  uint8_t h_2;                      // Alarmstunde für Alarm 2
  uint8_t m_2;                      // Alarmminute für Alarm 2
} AlarmEdit;

AlarmEdit alarmConfig;  // Instanz der Alarmkonfiguration

// Instanz des Displaytreibers
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
// Instanz des Touchscreen-Treibers
XPT2046_Touchscreen touch(TOUCH_CS, TOUCH_IRQ);
// Instanz zur Handhabung von Touch-Ereignissen
TouchEvent tevent(touch);

// Prototyp der Funktion zum Anzeigen des Schiebereglers
void showSlider(uint16_t y, float value, uint16_t vmax, uint16_t bgColor = ILI9341_LIGHTGREY, uint16_t lineColor = ILI9341_BLACK);

// Zeichnet die Footer-Buttons für Power, Sleep und Alarm auf dem Display
// Die Buttons werden je nach ihrem aktuellen Status in unterschiedlichen Farben dargestellt.
// - Power-Button: Grün, wenn das Radio läuft; Standardfarbe, wenn nicht.
// - Sleep-Button: Orange, wenn der Schlummermodus aktiv ist; Standardfarbe, wenn nicht.
// - Alarm-Button: Standardfarbe für aktiven Alarm; Rot für deaktivierten Alarm.
void DrawFooterButtons_Power_Sleep_Alarm() {
  uint16_t color_temp;  // Variable zum Speichern der aktuellen Farbe

  // Power Button
  if (radio) {                   // Wenn das Radio läuft
    color_temp = ILI9341_GREEN;  // Setze Farbe für aktiven Zustand
  } else {
    color_temp = COLOR_KNOEPFE;  // Setze Standardfarbe für inaktiven Zustand
  }
  // Zeichne das Bitmap für den Power-Button
  tft.drawBitmap(0, 176, knopf_sym[0], 64, 64, color_temp, COLOR_KNOEPFE_BG);

  // Sleep Button
  if (snoozeTimeEnd != 0) {       // Wenn der Schlummermodus aktiv ist
    color_temp = ILI9341_ORANGE;  // Setze Farbe für aktiven Schlummermodus
  } else {
    color_temp = COLOR_KNOEPFE;  // Setze Standardfarbe für inaktiven Zustand
  }
  // Zeichne das Bitmap für den Sleep-Button
  tft.drawBitmap(64, 176, knopf_sym[1], 64, 64, color_temp, COLOR_KNOEPFE_BG);

  // Alarm Button
  uint8_t symbol;
  if (alarmday < 8) {            // Wenn der Wecker aktiv ist
    color_temp = COLOR_KNOEPFE;  // Setze Standardfarbe für aktiven Wecker
    symbol = 2;                  // Wähle Symbol für aktiven Wecker
  } else {                       // Wenn der Wecker deaktiviert ist
    color_temp = ILI9341_RED;    // Setze Farbe für deaktivierten Wecker
    symbol = 3;                  // Wähle Symbol für deaktivierten Wecker
  }
  // Zeichne das Bitmap für den Alarm-Button
  tft.drawBitmap(128, 176, knopf_sym[symbol], 64, 64, color_temp, COLOR_KNOEPFE_BG);
}

// Behandelt die Interaktion mit den Footer-Buttons für Power, Sleep und Alarm
// Diese Funktion überprüft die x-Koordinate der Berührung, um festzustellen, welcher
// Button im Footer-Bereich des Displays gedrückt wurde, und führt die entsprechende
// Aktion aus, wie das Ein- oder Ausschalten des Radios, das Starten des Schlummermodus
// oder das Ein- oder Ausschalten des Alarms.
void handleFooterButtons_Power_Sleep_Alarm(int x, int y) {
  // Überprüfe die x-Koordinate, um den gedrückten Button zu identifizieren

  // Power Button (Position: 0 bis 64)
  if (x < 64) {
    toggleRadio(radio);  // Schalte das Radio ein oder aus
  }
  // Sleep Button (Position: 64 bis 128)
  else if ((x > 64) && (x < 128)) {
    startSnooze();  // Starte den Schlummermodus
  }
  // Alarm Button (Position: 128 bis 192)
  else if ((x > 128) && (x < 192)) {
    toggleAlarm();  // Schalte den Alarm ein oder aus
  }
}

// Verarbeitet die Berührung von Favoriten-Tasten auf der Radio-Seite
// Diese Funktion prüft, welche der Favoriten-Tasten auf dem Bildschirm berührt wurde
// und setzt die aktuelle Station (`curStation`) auf den entsprechenden Favoriten.
// Die Funktion nutzt die x- und y-Koordinaten der Berührung, um den Bereich der Favoriten-Tasten
// zu bestimmen und den Index der aktiven Favoriten aus einem Array von Favoriten-Indizes abzurufen.
void HandleFavoriteButtons(int xpos, int ypos) {
  int loopCount = 0;
  int favoriteIndexes[10];  // Array von 10 Elementen für die Indizes der Favoriten

  // Initialisiere das Array der Favoriten-Indizes
  for (int i = 0; i < 10; i++) {
    favoriteIndexes[i] = -1;  // Ein Wert von -1 zeigt an, dass kein Favorit vorhanden ist
  }

  // Fülle das Array mit den Indizes der aktiven Favoriten
  for (int i = 0; i < STATIONS && loopCount < 10; i++) {
    if (stationlist[i].enabled) {
      favoriteIndexes[loopCount] = i;
      loopCount++;
    }
  }

  // Überprüfe die y-Koordinate, um den Bereich der Favoriten-Tasten zu bestimmen
  if (ypos < 88) {  // obere Reihe der Favoriten-Tasten
    // Überprüfe die x-Koordinate für die spezifischen Favoriten-Tasten
    if ((xpos < 64) && (favoriteIndexes[0] > -1)) { curStation = favoriteIndexes[0]; }
    if ((xpos >= 64) && (xpos < 128) && (favoriteIndexes[1] > -1)) { curStation = favoriteIndexes[1]; }
    if ((xpos >= 128) && (xpos < 192) && (favoriteIndexes[2] > -1)) { curStation = favoriteIndexes[2]; }
    if ((xpos >= 192) && (xpos < 256) && (favoriteIndexes[3] > -1)) { curStation = favoriteIndexes[3]; }
    if ((xpos >= 256) && (favoriteIndexes[4] > -1)) { curStation = favoriteIndexes[4]; }
  } else {  // untere Reihe der Favoriten-Tasten
    // Überprüfe die x-Koordinate für die spezifischen Favoriten-Tasten
    if ((xpos < 64) && (favoriteIndexes[5] > -1)) { curStation = favoriteIndexes[5]; }
    if ((xpos >= 64) && (xpos < 128) && (favoriteIndexes[6] > -1)) { curStation = favoriteIndexes[6]; }
    if ((xpos >= 128) && (xpos < 192) && (favoriteIndexes[7] > -1)) { curStation = favoriteIndexes[7]; }
    if ((xpos >= 192) && (xpos < 256) && (favoriteIndexes[8] > -1)) { curStation = favoriteIndexes[8]; }
    if ((xpos >= 256) && (favoriteIndexes[9] > -1)) { curStation = favoriteIndexes[9]; }
  }
}


// Transformiert die Touch-Point-Koordinaten entsprechend der aktuellen Rotation des TFT-Displays
// Diese Funktion wird verwendet, um die Roh-Touch-Koordinaten in die Bildschirmkoordinaten zu konvertieren,
// basierend auf der Rotation des Displays. Die Koordinaten werden entsprechend angepasst,
// sodass sie korrekt auf dem Bildschirm angezeigt werden, unabhängig von der Display-Rotation.
TS_Point transformTouch(TS_Point p) {
  // Initialisiere pTransformed mit den gleichen Werten wie p
  TS_Point pTransformed = p;

  // Überprüfe die aktuelle Rotation des TFT-Displays
  switch (tft.getRotation()) {
    case 1:
      // Bei Rotation 1 (90 Grad im Uhrzeigersinn):
      // X-Koordinate wird umgekehrt und Y-Koordinate wird ebenfalls umgekehrt
      pTransformed.x = tft.width() - p.x;
      pTransformed.y = tft.height() - p.y;
      break;
    case 3:
      // Bei Rotation 3 (270 Grad im Uhrzeigersinn):
      // Keine Änderung erforderlich, da pTransformed bereits p ist
      break;
    default:
      // Standardfall für keine Rotation (0 Grad oder 180 Grad):
      // Keine Änderung erforderlich
      break;
  }

  // Rückgabe der transformierten Koordinaten
  return pTransformed;
}


// Callback-Funktion für alle Touch-Events
// Diese Funktion verarbeitet Berührungsereignisse auf dem Touchscreen und führt entsprechende Aktionen
// basierend auf der aktuellen Seite und der Position des Touch-Events aus.
void onTouchClick(TS_Point p) {
  // Zuerst die Touch-Koordinaten transformieren, falls das Display rotiert ist
  p = transformTouch(p);

  // Wenn sich das System nicht im Konfigurationsmodus befindet, starte die Konfigurationszeit neu
  if (!clockmode) start_conf = millis();

  // Debug-Ausgabe der berührten Koordinaten
  Serial.printf("Touch on %i, %i\n", p.x, p.y);

  if (clockmode) {             // Wenn wir im Uhrmodus sind
    if (p.y < 210) {           // Überprüfe, ob der Klick oberhalb von 210px ist
      if (p.x < 40) {          // Links (X < 40)
        beforeStation();       // Ruft die Funktion zum Wechseln zur vorherigen Station auf
      } else if (p.x > 280) {  // Rechts (X > 280)
        nextStation();         // Ruft die Funktion zum Wechseln zur nächsten Station auf
      } else {                 // Restlicher Bereich oberhalb von 210px
        clockmode = false;
        configpage = false;
        radiopage = true;
        alarmpage = false;
        showRadioPage();  // Wechselt zur Radiopage
      }
    } else {                          // Unterhalb von 210px
      setGainValue(p.x, "MainPage");  // Passt die Lautstärke an
    }
  } else if (radiopage) {  //################################################ RADIO PAGE
    // Wenn wir im Radiomodus sind
    if (p.y > 180) {                                    // Wenn Y-Koordinate größer als 180, befinden wir uns im Button-Bereich
      handleFooterButtons_Power_Sleep_Alarm(p.x, p.y);  // Verarbeite Footer-Buttons
      if ((p.x > 192) && (p.x < 256)) changeStation();  // Wechsel zur nächsten Station
      if (p.x > 256) {                                  // Wenn X-Koordinate größer als 256, wechsle zur Konfigurationsseite
        clockmode = false;
        configpage = true;
        radiopage = false;
        alarmpage = false;
        showConfigPage();
      }
    }
    // Bereich für Lautstärkeregler (0 bis 44)
    if ((p.y > 0) && (p.y < 44)) setGainValue(p.x, "RadioPage");
    // Bereich für Favoriten-Buttons (44 bis 132)
    if ((p.y > 44) && (p.y < 132)) {
      HandleFavoriteButtons(p.x, p.y);
      changeStation();
      showClock();
    }
    // Bereich für die Stationenliste (132 bis 175)
    if ((p.y > 132) && (p.y < 175)) selectStation(p.x);
  } else if (configpage) {  //################################################ CONFIG PAGE
    // Wenn wir im Konfigurationsmodus sind
    if (p.y > 180) {                                    // Wenn Y-Koordinate größer als 180, befinden wir uns im Button-Bereich
      handleFooterButtons_Power_Sleep_Alarm(p.x, p.y);  // Verarbeite Footer-Buttons
      if ((p.x > 192) && (p.x < 256)) changeStation();  // Wechsel zur nächsten Station
      if (p.x > 256) {                                  // Wenn X-Koordinate größer als 256, wechsle zur Alarmseite
        clockmode = false;
        configpage = false;
        radiopage = false;
        alarmpage = true;
        showAlarmPage();
      }
    }
    // Bereich für Lautstärkeregler (0 bis 44)
    if ((p.y > 0) && (p.y < 44)) setGainValue(p.x, "SettingPage");
    // Bereich für Helligkeitsregler (44 bis 88)
    if ((p.y > 44) && (p.y < 88)) setBrightness(p.x);
    // Bereich für Schlummerzeitregler (88 bis 132)
    if ((p.y > 88) && (p.y < 132)) setSnoozeTime(p.x);
    // Bereich für die Stationenliste (132 bis 175)
    if ((p.y > 132) && (p.y < 175)) selectStation(p.x);
  } else if (alarmpage) {  //################################################ ALARM SETTING PAGE
    // Wenn wir uns im Alarm-Einstellmodus befinden
    if (p.y > 180) {                                    // Wenn Y-Koordinate größer als 180, befinden wir uns im Button-Bereich
      handleFooterButtons_Power_Sleep_Alarm(p.x, p.y);  // Verarbeite Footer-Buttons
      if ((p.x > 192) && (p.x < 256)) safeAlarmTime();  // Speichere die Alarmzeit
      if (p.x > 256) {                                  // Wenn X-Koordinate größer als 256, wechsle zurück zur Hauptseite
        clockmode = true;
        configpage = false;
        radiopage = false;
        alarmpage = false;
        showClock();
      }
    }
    // Bereich für das Umschalten der Alarmtage (0 bis 44)
    if ((p.y > 0) && (p.y < 44)) toggleAlarmDay(p.x, p.y);
    // Bereich für das Umschalten der Alarmtage (88 bis 132)
    if ((p.y > 88) && (p.y < 132)) toggleAlarmDay(p.x, p.y);
    // Bereich für die Anpassung der Alarmzeit (HH) (44 bis 88 und 132 bis 176)
    if (((p.y > 44) && (p.y < 88)) || ((p.y > 132) && (p.y < 176))) {
      if (p.x < 50) in_de_crementAlarmTimeHH(p.x, p.y);
      if ((p.x > 50) && (p.x < 100)) in_de_crementAlarmTimeHH(p.x, p.y);
    }
    // Bereich für die Anpassung der Alarmzeit (MM) (44 bis 88 und 132 bis 176)
    if (((p.y > 44) && (p.y < 88)) || ((p.y > 132) && (p.y < 176))) {
      if ((p.x > 220) && (p.x < 270)) in_de_crementAlarmTimeMM(p.x, p.y);
      if (p.x > 270) in_de_crementAlarmTimeMM(p.x, p.y);
    }
    // Bereich für die Speichern-Funktion (kein spezieller Kommentar notwendig, da dies nicht direkt im Code spezifiziert ist)
  }
}


// Setzt die Lautstärke basierend auf der x-Position, an der der Schieberegler geklickt wurde
// Die Funktion berechnet den Lautstärkewert als Prozentsatz und passt den Schieberegler
// auf der entsprechenden Seite an. Der Lautstärkewert wird auch gespeichert und aktualisiert.
void setGainValue(uint16_t value, const char* sliderPage) {
  char txt[10];
  float v;

  // Überprüfen, ob wir uns nicht auf der "WEBSITE"-Seite befinden
  if (strcmp(sliderPage, "WEBSITE") != 0) {
    // Berechne den Lautstärkewert basierend auf der x-Position des Schiebers von 0 bis 100%
    int y_start = 11;       // Startlinie des Schiebers
    int line_length = 298;  // Länge der Schiebereglerlinie

    float start_slider = value - y_start;      // Bereich vor dem Linienstart abziehen
    float end_slider = line_length - y_start;  // Bereich vor dem Linienstart abziehen
    v = start_slider / end_slider * 100;       // Berechnung des Prozentsatzes
    if (v > 100) v = 100;                      // Begrenze den Wert auf maximal 100%
    if (v < 0) v = 0;                          // Begrenze den Wert auf mindestens 0%
  } else {
    // Bei der "WEBSITE"-Seite ist der Wert bereits direkt gegeben
    v = value;
  }

  curGain = v;  // Setze den aktuellen Lautstärkewert

  // Speichere den aktuellen Lautstärkewert und passe den Schieberegler an
  pref.putUShort("gain", curGain);

  // Zeige den Schieberegler je nach Seite an
  if (strcmp(sliderPage, "MainPage") == 0 || (clockmode && strcmp(sliderPage, "WEBSITE") == 0)) {
    showSlider(218, curGain, 100, COLOR_SLIDER_BG, COLOR_SLIDER);  // Zeige den Schieberegler für die Hauptseite oder Website
  } else {
    // Standard: Einstellungsseite
    showSlider(27, curGain, 100);         // Zeige den Schieberegler für die Einstellungsseite
    sprintf(txt, "%i %%", (int)curGain);  // Formatiere den Lautstärkewert als Text
    // Zeige den Lautstärkewert rechts vom Schieberegler an
    displayMessage(231, 8, 80, 20, txt, ALIGNRIGHT, false, ILI9341_BLACK, ILI9341_LIGHTGREY, 1);
  }

  setGain(curGain);  // Setze die Lautstärke auf den neuen Wert
}


// Setzt die Helligkeit basierend auf der x-Position, an der der Schieberegler geklickt wurde
// Die Funktion berechnet den Helligkeitswert als Prozentsatz und passt den Schieberegler
// entsprechend an. Der Helligkeitswert wird ebenfalls gespeichert und angezeigt.
void setBrightness(uint16_t value) {
  char txt[10];
  // Berechne die Helligkeit basierend auf der x-Position des Schiebers von 0 bis 100 %
  // Der Schieberegler hat einen Startwert von 15, und der Wertbereich wird auf 0 bis 100 % abgebildet
  float v = (value - 15) * 0.345;  // Umrechnung der Position in Prozent

  // Begrenze den Wert auf den Bereich von 0 bis 100 %
  if (v > 100) v = 100;  // Maximalwert auf 100 % begrenzen
  if (v < 5) v = 0;      // Minimalwert auf 5 % begrenzen (Anpassung für mögliche Abweichungen)

  bright = v;  // Setze den aktuellen Helligkeitswert

  // Speichere den aktuellen Helligkeitswert
  pref.putUShort("bright", bright);

  // Zeige den Schieberegler für die Helligkeit an
  showSlider(71, bright, 100);

  // Formatiere den Helligkeitswert als Text
  sprintf(txt, "%i %%", (int)bright);

  // Zeige den Helligkeitswert rechts vom Schieberegler an
  displayMessage(231, 52, 80, 20, txt, ALIGNRIGHT, false, ILI9341_BLACK, ILI9341_LIGHTGREY, 1);
}

// Setzt die Schlummerzeit basierend auf der x-Position, an der der Schieberegler geklickt wurde
// Die Funktion berechnet die Schlummerzeit in Minuten und passt den Schieberegler entsprechend an.
// Der Wert wird auf einen Bereich von 0 bis 60 Minuten begrenzt, gespeichert und angezeigt.
void setSnoozeTime(uint16_t value) {
  char txt[10];

  // Berechne die Schlummerzeit basierend auf der x-Position des Schiebers von 0 bis 60 Minuten
  // Der Schieberegler hat einen Startwert von 15, und der Wertbereich wird auf 0 bis 60 Minuten abgebildet
  float v = (value - 15) * 0.21;  // Umrechnung der Position in Minuten

  // Begrenze den Wert auf den Bereich von 0 bis 60 Minuten
  if (v > 60) v = 60;  // Maximalwert auf 60 Minuten begrenzen
  if (v < 0) v = 0;    // Minimalwert auf 0 Minuten begrenzen

  snoozeTime = v;  // Setze den aktuellen Schlummerzeitwert

  // Speichere den aktuellen Schlummerzeitwert
  pref.putUShort("snooze", snoozeTime);

  // Zeige den Schieberegler für die Schlummerzeit an
  showSlider(115, snoozeTime, 60);

  // Formatiere den Schlummerzeitwert als Text
  sprintf(txt, "%i min", (int)snoozeTime);

  // Zeige den Schlummerzeitwert rechts vom Schieberegler an
  displayMessage(231, 96, 80, 20, txt, ALIGNRIGHT, false, ILI9341_BLACK, ILI9341_LIGHTGREY, 1);
}

void setBGLight(uint8_t prct) {
  uint16_t ledb;  // Variable für die Helligkeit der LED

  // Wenn der Prozentsatz 0 ist, lese die Umgebungshelligkeit vom LDR (Lichtabhängiger Widerstand)
  if (prct == 0) {
    static unsigned long prevMillis = -20000;   // Zeitstempel der letzten Helligkeitsmessung
    if (millis() - prevMillis < 10000) return;  // Warte 10 Sekunden, bevor die Helligkeit erneut gemessen wird
    prevMillis = millis();                      // Aktualisiere den Zeitstempel
    ledb = analogRead(LDR) * 255 / 4096;        // Lese den LDR-Wert und skaliere ihn auf 0 bis 255
  } else {
    // Berechne die Helligkeit basierend auf dem Prozentsatz
    ledb = 255 * prct / 100;  // Skaliere den Prozentsatz auf einen Bereich von 0 bis 255
  }

  // Begrenze den Helligkeitswert auf den Bereich von 3 bis 255
  if (ledb > 255) ledb = 255;  // Maximalwert auf 255 begrenzen
  if (ledb < 3) ledb = 3;      // Minimalwert auf 3 begrenzen, um ein Ausbleichen zu vermeiden

  // Wenn LED_ON auf 0 gesetzt ist, invertiere die Helligkeit
  if (LED_ON == 0) ledb = 255 - ledb;

  // Nur dann die Helligkeit setzen, wenn sich der Wert geändert hat
  if (ledb != lastLedb) {
    // Debug-Ausgabe der Helligkeit
    Serial.printf("percent = %i LED = %i\n", prct, ledb);

    // Setze die Helligkeit der Hintergrundbeleuchtung
    analogWrite(TFT_LED, ledb);
    lastLedb = ledb;  // Speichere den neuen Helligkeitswert
  }
}

// Wählt eine Station aus der Liste der Stationen basierend auf der x-Position des Touch-Ereignisses aus
// Der linke Button (x < 50) scrollt die Liste der Stationen nach unten,
// während der rechte Button (x > 270) die Liste nach oben scrollt.
// Die Auswahl wird auf die nächste aktivierte Station gesetzt und die Anzeige aktualisiert.
void selectStation(uint16_t x) {
  // Überprüfe, ob die x-Position im Bereich des linken Buttons liegt
  if (x < 50) {  // Linker Button, um die Stationen nach unten zu scrollen
    // Gehe nach unten durch die Liste, bis eine aktivierte Station gefunden wird
    do {
      if (curStation == 0) {
        // Wenn wir am Anfang der Liste sind, gehe zum Ende der Liste
        curStation = STATIONS - 1;
      } else {
        // Andernfalls, gehe zur vorherigen Station
        curStation--;
      }
    } while (!stationlist[curStation].enabled);  // Wiederhole, bis eine aktivierte Station gefunden wird
  }

  // Überprüfe, ob die x-Position im Bereich des rechten Buttons liegt
  if (x > 270) {  // Rechter Button, um die Stationen nach oben zu scrollen
    // Gehe nach oben durch die Liste, bis eine aktivierte Station gefunden wird
    do {
      if (curStation == (STATIONS - 1)) {
        // Wenn wir am Ende der Liste sind, gehe zum Anfang der Liste
        curStation = 0;
      } else {
        // Andernfalls, gehe zur nächsten Station
        curStation++;
      }
    } while (!stationlist[curStation].enabled);  // Wiederhole, bis eine aktivierte Station gefunden wird
  }

  // Zeige die ausgewählte Station an
  updateStation();
}

// Schaltet das Radio ein oder aus, basierend auf dem boolean Parameter 'off'.
// Wenn 'off' wahr ist, wird das Radio gestoppt, die Schlummerzeit zurückgesetzt und der Radio-Status auf 'aus' gesetzt.
// Wenn 'off' falsch ist, wird das Radio eingeschaltet, die URL der aktuellen Station gestartet (oder zur ersten Station gewechselt, wenn das Starten fehlschlägt),
// und die Lautstärke auf den aktuellen Wert gesetzt.
// Der Modus wird anschließend auf Uhr gesetzt und die Uhr-Anzeige wird angezeigt.
void toggleRadio(boolean off) {
  if (off) {
    // Wenn 'off' wahr ist, stoppe das Radio
    stopPlaying();      // Stoppe den aktuellen Stream
    snoozeTimeEnd = 0;  // Schalte die Schlummerzeit aus
    radio = false;      // Setze den Radio-Status auf 'aus'
    //setGain(0);  // (Optional) Setze die Lautstärke auf 0 (auskommentiert)
  } else {
    // Wenn 'off' falsch ist, schalte das Radio ein
    if (connected) {  // Überprüfe, ob eine Verbindung besteht
      radio = true;   // Setze den Radio-Status auf 'ein'
      if (!startUrl(String(stationlist[actStation].url))) {
        // Wenn das Starten der URL fehlschlägt, wechsle zur ersten Station
        actStation = 0;                                 // Setze die aktuelle Station auf 0
        startUrl(String(stationlist[actStation].url));  // Versuche erneut, die URL zu starten
      }
      setGain(curGain);  // Setze die Lautstärke auf den aktuellen Wert
    }
  }
  // Wechsel zur Uhr-Anzeige
  clockmode = true;  // Setze den Modus auf Uhr
  showClock();       // Zeige die Uhr-Anzeige an
}

// Schaltet den Alarm ein oder aus, basierend auf dem aktuellen Wert von 'alarmday'.
// Wenn 'alarmday' größer als 7 ist (was bedeutet, dass der Alarm derzeit deaktiviert ist),
// wird der nächste verfügbare Alarm gefunden und der Alarm-Status auf 'ein' gesetzt.
// Wenn 'alarmday' 7 oder kleiner ist (was bedeutet, dass der Alarm aktiviert ist),
// wird der Alarm deaktiviert, indem 'alarmday' auf 8 gesetzt wird.
// Der Alarm-Status wird entsprechend in den Einstellungen aktualisiert.
// Anschließend wird der Modus auf Uhr gesetzt und die Uhr-Anzeige wird angezeigt.
void toggleAlarm() {
  if (alarmday > 7) {
    // Wenn der Alarm ausgeschaltet war (alarmday > 7), finde den nächsten Alarm und aktiviere ihn
    findNextAlarm();                // Suche nach dem nächsten verfügbaren Alarm
    pref.putBool("alarmon", true);  // Setze die Alarm-Status in den Einstellungen auf 'ein'
  } else {
    // Wenn der Alarm aktiviert ist, deaktiviere ihn
    alarmday = 8;                    // Setze alarmday auf 8, was als 'Alarm aus' interpretiert wird
    pref.putBool("alarmon", false);  // Setze die Alarm-Status in den Einstellungen auf 'aus'
  }
  // Wechsel zur Uhr-Anzeige
  clockmode = true;  // Setze den Modus auf Uhr
  showClock();       // Zeige die Uhr-Anzeige an
}

// Startet die Schlummerfunktion.
// Berechnet die Zeit zum Ende des Schlummermodus basierend auf der aktuellen Uhrzeit oder alternativ mit 'millis()', falls die Zeit nicht verfügbar ist.
// Setzt den Modus auf Uhr, stoppt die Radio-Wiedergabe und zeigt die Uhr-Anzeige an.
void startSnooze() {
  struct tm ti;
  if (getLocalTime(&ti)) {
    // Berechne die aktuelle Zeit in Minuten seit Mitternacht
    int currentMinutes = ti.tm_hour * 60 + ti.tm_min;
    // Setzt die Zeit für das Ende des Schlummermodus relativ zur aktuellen Zeit in Millisekunden
    snoozeTimeEnd = millis() + (snoozeTime * 60000);
  } else {
    // Fallback: Setzt die Schlummerzeit basierend auf millis(), falls keine Zeit verfügbar ist
    snoozeTimeEnd = millis() + snoozeTime * 60000;
  }
  toggleRadio(false);   // Schaltet das Radio aus
  clockmode = true;     // Wechselt in den Uhr-Modus
  showClock();          // Zeigt die Uhr-Anzeige an
}

// Setzt die ausgewählte Station als aktive Station, speichert diesen Wert und versucht, die URL der aktuellen Station zu starten.
// Falls das Starten der URL fehlschlägt, wird zur Station 0 gewechselt. Danach wird der Modus auf Uhr gesetzt und die Uhr-Anzeige angezeigt.
void changeStation() {
  actStation = curStation;  // Setzt die aktuelle Station (curStation) als aktive Station (actStation)

  // Speichert den neuen Wert der aktiven Station
  pref.putUShort("station", curStation);

  // Versucht, die URL der aktuellen Station zu starten
  if (!startUrl(String(stationlist[actStation].url))) {
    // Wenn das Starten fehlschlägt, wechselt zur Station 0
    actStation = 0;
    startUrl(String(stationlist[actStation].url));  // Startet die URL der Station 0
  }

  // Wechselt zum Uhr-Bildschirm
  clockmode = true;
  showClock();  // Zeigt die Uhr-Anzeige an
}

// Wird in der Hauptschleife aufgerufen, um Touch-Ereignisse vom Touchscreen abzufragen und zu verarbeiten.
void touch_loop() {
  tevent.pollTouchScreen();  // Fragt den Touchscreen nach Ereignissen ab
}

// Bereitet das Display für die Nutzung vor, indem es das TFT-Display und das Touchscreen-Modul initialisiert,
// die Hintergrundbeleuchtung einstellt und die Display-Konfiguration vornimmt.
void setup_display() {
  pinMode(TFT_LED, OUTPUT);  // Setzt den Pin für die Hintergrundbeleuchtung des TFT-Displays auf Ausgang
  pinMode(LDR, INPUT);       // Setzt den Pin für den Lichtsensor (LDR) auf Eingang

  lastldr = analogRead(LDR);  // Liest den aktuellen Wert des Lichtsensors (LDR) und speichert ihn

  setBGLight(100);  // Setzt die Hintergrundbeleuchtung auf 100% Helligkeit

  tft.begin();                    // Initialisiert das TFT-Display
  tft.fillScreen(ILI9341_BLACK);  // Füllt den gesamten Bildschirm mit der Farbe Schwarz
  tft.setRotation(3);             // Setzt die Display-Rotation auf 3 (kann je nach Display unterschiedlich sein)
  tft.setFont(&AT_Bold12pt7b);    // Setzt die Schriftart für den Text auf eine benutzerdefinierte Schriftart

  touch.begin();                                    // Initialisiert das Touchscreen-Modul
  tevent.setResolution(tft.width(), tft.height());  // Setzt die Auflösung für das Touch-Ereignis-Modul
  tevent.setDrawMode(false);                        // Setzt den Zeichenmodus für das Touch-Ereignis-Modul auf aus (Zeichnen ist deaktiviert)

  // Registriert die Callback-Funktion für Touch-Ereignisse
  tevent.registerOnTouchClick(onTouchClick);
}

// Zeichnet einen Text in einem Rechteck an der Position x,y mit der Größe w,h
// Der Text kann linksbündig, zentriert oder rechtsbündig ausgerichtet werden
// Die Textgröße kann auf standard oder groß gesetzt werden
// Schriftfarbe und Hintergrundfarbe können ausgewählt werden sowie die Anzahl der Zeilen
void textInBox(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const char* text, uint8_t align = ALIGNLEFT, boolean big = false, uint16_t fc = ILI9341_WHITE, uint16_t bg = ILI9341_BLACK, uint8_t lines = 1) {
  char tmp[256];               // Temporäres Array zum Speichern des kodierten Textes
  char msg[50];                // Temporäres Array zum Speichern der aktuellen Zeile
  char* token;                 // Zeiger für die Tokenisierung des Textes
  int16_t xt, yt, h0, x0, sp;  // Variablen für Text-Berechnungen
  uint16_t wt, ht, len;        // Variablen für Text-Breite und -Höhe
  uint8_t l;                   // Anzahl der verbleibenden Zeilen

  // Füllt das Rechteck mit der Hintergrundfarbe, um alten Inhalt zu löschen
  tft.fillRect(x, y, w, h, bg);

  // Setzt die Schriftart basierend auf der `big`-Variable
  if (big) tft.setFont(FNT12);  // Große Schriftart
  else tft.setFont(FNT9);       // Standard Schriftart

  tft.setTextColor(fc);  // Setzt die Textfarbe

  // Kodierung für deutsche Umlaute
  encodeUnicode(text, tmp);

  // Bestimmt die Breite und Höhe eines einzelnen Buchstabens (z.B. "n") zur Berechnung des Abstands
  tft.getTextBounds("n", 0, 100, &xt, &yt, &wt, &ht);
  sp = wt;  // Abstand zwischen Zeichen

  // Bestimmt die Breite und Höhe des gesamten Textes
  tft.getTextBounds(tmp, 0, 100, &xt, &yt, &wt, &ht);

  // Berechnet den y-Offset basierend auf der Texthöhe
  h0 = 100 - yt;
  x0 = x;
  y = y + h0 + 1;  // Setzt den Startpunkt für das Zeichnen des Textes

  if (wt < w) {                                    // Genügend Platz für den gesamten Text in einer Zeile
    if (align == ALIGNCENTER) x0 += (w - wt) / 2;  // Zentriert den Text
    if (align == ALIGNRIGHT) x0 += (w - wt);       // Rechtsbündige Ausrichtung
    tft.setCursor(x0, y);                          // Setzt den Cursor an die berechnete Position
    tft.print(tmp);                                // Zeichnet den Text
  } else {                                         // Nicht genügend Platz für den gesamten Text in einer Zeile
    // Text wird durch Wortumbruch angepasst, wenn nur eine Zeile erlaubt ist
    l = lines;
    token = strtok(tmp, " ");  // Zerlegt den Text in einzelne Wörter
    msg[0] = 0;                // Leert das Nachrichten-Array
    len = 0;                   // Initialisiert die Länge der aktuellen Zeile
    while ((token != NULL) && (l > 0)) {
      tft.getTextBounds(token, 0, 100, &xt, &yt, &wt, &ht);
      if ((len + wt + sp) < w) {  // Prüft, ob das Wort in die aktuelle Zeile passt
        if (len > 0) {
          strcat(msg, " ");  // Fügt ein Leerzeichen hinzu, wenn notwendig
          len += sp;         // Fügt den Abstand zwischen den Wörtern hinzu
        }
        len = len + wt;
        strcat(msg, token);  // Fügt das Wort zur aktuellen Zeile hinzu
      } else {
        x0 = x;
        if (align == ALIGNCENTER) x0 += (w - len) / 2;  // Zentriert die Zeile
        if (align == ALIGNRIGHT) x0 += (w - len);       // Rechtsbündige Ausrichtung
        tft.setCursor(x0, y);                           // Setzt den Cursor für die neue Zeile
        tft.print(msg);                                 // Zeichnet die aktuelle Zeile
        len = wt;                                       // Setzt die Länge auf die Breite des aktuellen Wortes
        strcpy(msg, token);                             // Startet eine neue Zeile mit dem aktuellen Wort
        y = y + ht + 1;                                 // Verschiebt die y-Position für die nächste Zeile
        l--;                                            // Verringert die verbleibende Anzahl der Zeilen
      }
      token = strtok(NULL, " ");  // Holt das nächste Wort
    }
    if ((len > 0) && (l > 0)) {  // Zeichnet die letzte Zeile, wenn noch Platz ist
      x0 = x;
      if (align == ALIGNCENTER) x0 += (w - len) / 2;  // Zentriert die Zeile
      if (align == ALIGNRIGHT) x0 += (w - len);       // Rechtsbündige Ausrichtung
      y = y + h0 + 1;                                 // Verschiebt die y-Position für die letzte Zeile
      tft.setCursor(x0, y);                           // Setzt den Cursor für die letzte Zeile
      tft.print(msg);                                 // Zeichnet die letzte Zeile
    }
  }
}

// Aktualisiert die Uhrzeit auf dem Display. Um Flackern zu vermeiden, werden nur die geänderten Teile neu gezeichnet.
// Wenn der Parameter `redraw` auf true gesetzt ist, wird der gesamte Inhalt neu gezeichnet.
void updateTime(boolean redraw) {
  char tim[40];  // Temporäres Array für die Zeit- und Datumsdarstellung

  // Diese Variablen sind statisch, um ihren Wert zwischen den Aufrufen zu behalten.
  static char lastdate[40] = "";
  static char lasttime[10] = "";

  // Holt die lokale Zeit und speichert sie in der Struktur `ti`.
  if (getLocalTime(&ti)) {
    // Formatierung des Datums als "Wochentag Tag. Monat Jahr"
    sprintf(tim, "%s %i. %s %i", days[ti.tm_wday], ti.tm_mday, months[ti.tm_mon], ti.tm_year + 1900);

    tick = millis() - ti.tm_sec * 1000;  // Berechnung der Systemzeit vom Startpunkt der aktuellen Minute

    // Überprüft, ob die Anzeige neu gezeichnet werden soll oder sich das Datum geändert hat.
    // Wenn ja, wird die Datumszeile neu gezeichnet.
    if (redraw || (strcmp(tim, lastdate) != 0)) {
      strcpy(lastdate, tim);                                                       // Speichert das neue Datum in `lastdate`
      textInBox(0, 90, 320, 25, tim, ALIGNCENTER, true, COLOR_DATE, COLOR_BG, 1);  // Zeichnet das Datum
    }

    uint8_t z;                                 // Variable zur Speicherung des Ziffernwerts
    strftime(tim, sizeof(tim), "%H:%M", &ti);  // Formatiert die aktuelle Zeit als "HH:MM"
    

    // Durchläuft den Zeit-String, um jede Ziffer zu überprüfen.
    // Wenn `redraw` true ist oder sich eine Ziffer geändert hat, wird diese Ziffer neu gezeichnet.
    for (uint8_t i = 0; i < 5; i++) {
      // Überprüft, ob die aktuelle Position im Zeit-String eine Trennstelle (":") ist.
      // Setzt `z` auf 10 für den Trennstrich oder auf die numerische Ziffer.
      z = (i == 2) ? 10 : tim[i] - '0';
      if ((z < 11) && (redraw || (tim[i] != lasttime[i]))) {
        // Zeichnet die Ziffern auf dem Display. Die Ziffern-Bilder werden durch `ziffern[z]` bereitgestellt.
        tft.drawBitmap(30 + i * 55, 18, ziffern[z], 50, 70, COLOR_TIME, COLOR_BG);
        drawHeaderInfos();      // Zeichnet Symbole und Text im Header
        Serial.printf("Zeit = %s\n", tim);         // Gibt die aktuelle Zeit an die serielle Konsole aus
      }
    }

    strcpy(lasttime, tim);  // Speichert die neue Zeit in `lasttime`
  }
}

// Zeichnet die Informationen im Header des Displays, einschließlich Wifi-Informationen, Einschlafsymbol und nächste Alarmzeit.
void drawHeaderInfos() {
  drawWifiInfo();    // Zeichnet die aktuellen Wifi-Informationen im Header
  drawSnoozeInfo();  // Zeichnet das Einschlafsymbol im Header
  showNextAlarm();   // Zeichnet das Wecker-Symbol und den nächsten Alarmtext im Header
}

// Zeichnet die Wifi-Informationen im Header des Displays
void drawWifiInfo() {
  int rssi = WiFi.RSSI();                            // Hole den RSSI-Wert für die Signalstärke
  char rssiChar[5];                                  // Char-Array zum Speichern des RSSI-Werts als Text
  snprintf(rssiChar, sizeof(rssiChar), "%d", rssi);  // RSSI-Wert in rssiChar umwandeln

  uint16_t color_wifi;  // Farbvariable für das Wifi-Symbol
  // Bestimme die Farbe basierend auf der Signalstärke (RSSI-Wert)
  if (rssi <= -70) {  // Schwacher Empfang (z.B. Rot)
    color_wifi = ILI9341_RED;
  } else if (rssi <= -50) {  // Mittlerer Empfang (Z.B. Gelb)
    color_wifi = ILI9341_YELLOW;
  } else {  // Starker Empfang (Z.B. Grün)
    color_wifi = ILI9341_GREEN;
  }

  // Zeichne das Wifi-Symbol an der Position (303, 0) mit der festgelegten Farbe
  tft.drawBitmap(303, 0, symbole[0], 17, 17, color_wifi, COLOR_BG);

  // Zeichne den RSSI-Wert als Text in der Box (Position (273, 0) mit Größe (30, 17))
  textInBox(273, 0, 30, 17, rssiChar, ALIGNCENTER, false, COLOR_WIFI_RSSI, COLOR_BG);

  // Zeichne die lokale IP-Adresse in der Box (Position (97, 0) mit Größe (159, 17))
  String localIPString = WiFi.localIP().toString();  // Hole die lokale IP-Adresse als String
  textInBox(97, 0, 159, 17, localIPString.c_str(), ALIGNCENTER, false, COLOR_IP, COLOR_BG);
}

// Zeichnet das Einschlafsymbol im Header des Displays
void drawSnoozeInfo() {
  uint16_t color_snooze;  // Variable zur Speicherung der Farbe für das Einschlafsymbol

  // Überprüfen, ob die Schlummerfunktion aktiv ist (snoozeTimeEnd != 0)
  if (snoozeTimeEnd != 0) {
    color_snooze = COLOR_SLEEP_SYMBOL;  // Farbe für das aktive Einschlafsymbol
  } else {
    color_snooze = COLOR_BG;  // Hintergrundfarbe für das nicht aktive Einschlafsymbol
  }

  // Zeichne das Einschlafsymbol an der Position (256, 0) mit der festgelegten Farbe
  // Das Symbol hat eine Größe von 17x17 Pixel
  tft.drawBitmap(256, 0, symbole[1], 17, 17, color_snooze, COLOR_BG);
}

// Löscht den gesamten Bildschirm und setzt ihn auf die Hintergrundfarbe zurück
void displayClear() {
  // Füllt den gesamten Bildschirm mit der Hintergrundfarbe
  tft.fillScreen(COLOR_BG);
}

// Zeigt Datum und Uhrzeit an der vorgesehenen Position an
void displayDateTime() {
  updateTime(false);            // Aktualisiert die Zeit ohne komplettes Neuzeichnen
}

// Zeigt den Fortschritt eines Software-Updates als Balken und Prozentsatz an
void showProgress(uint32_t prc) {
  char tmp[20];

  // Formatierung des Fortschrittsprozentsatzes als Text
  sprintf(tmp, "Progress %i%%", prc);

  // Zeigt den Fortschrittstext an der Position (5, 50) auf dem Display
  textInBox(5, 50, 310, 30, tmp, ALIGNLEFT, true, ILI9341_GREEN, COLOR_BG, 1);

  // Zeichnet den Fortschrittsbalken basierend auf dem Fortschritt
  if (prc == 0) {
    // Zeichnet nur den Umriss des Fortschrittsbalkens, wenn der Fortschritt 0% ist
    tft.drawRect(5, 80, 310, 20, ILI9341_BLUE);
  } else {
    // Füllt den Fortschrittsbalken entsprechend dem Fortschritt
    tft.fillRect(5, 80, 310 * prc / 100, 20, ILI9341_BLUE);
  }
}

// Kodiert Text von Unicode in das Display-Codeformat für die angegebene Schriftart
// Die Quell- und Zielpuffer müssen existieren
void encodeUnicode(const char* src, char* dst) {
  uint8_t i = 0;             // Index für das Quellarray
  uint8_t i1 = 0;            // Index für das Zielarray
  char c;                    // Aktuelles Zeichen
  uint16_t m = strlen(src);  // Länge des Quellstrings

  do {
    c = src[i];

    // Prüft auf UTF-8 Zeichen für deutsche Umlaute
    if (c == 195) {
      i++;
      switch (src[i]) {
        case 164: c = 130; break;  // ä -> 130
        case 182: c = 131; break;  // ö -> 131
        case 188: c = 132; break;  // ü -> 132
        case 159: c = 133; break;  // ß -> 133
        case 132: c = 127; break;  // Ä -> 127
        case 150: c = 128; break;  // Ö -> 128
        case 156: c = 129; break;  // Ü -> 129
        default: c = 255;          // Unbekanntes Zeichen, nicht kodiert
      }
    } else if (c == 194) {  // Prüft auf UTF-8 Zeichen für das Grad-Symbol
      i++;
      if (src[i] == 176) c = 134;  // Grad-Symbol -> 134
      else c = 255;                // Unbekanntes Zeichen
    }

    if (c < 135) dst[i1] = c;  // Nur Zeichen im Bereich < 135 werden in den Zielpuffer geschrieben
    i++;
    i1++;
  } while (i < m);  // Schleife bis zum Ende des Quellstrings

  dst[i1] = 0;  // Null-Terminierung des Zielstrings
}

// Zeigt einen Schieberegler an. y definiert die obere Linie, value ist die Position des Rechtecks
void showSlider(uint16_t y, float value, uint16_t vmax, uint16_t bgColor, uint16_t lineColor) {
  // Füllt den Hintergrund des Schiebereglers mit der Hintergrundfarbe
  tft.fillRect(11, y, 298, 14, bgColor);

  // Zeichnet die horizontale Linie des Schiebereglers
  tft.drawLine(15, y + 6, 305, y + 6, lineColor);  // Oberkante der Linie
  tft.drawLine(15, y + 7, 305, y + 7, lineColor);  // Unterkante der Linie

  // Berechnet die Position des Schiebereglers basierend auf dem Wert
  uint16_t pos = value * 290 / vmax;

  // Zeichnet das Schieberegler-Rechteck an der berechneten Position
  tft.fillRect(12 + pos, y, 8, 14, lineColor);
}

// Zeigt den Bereich zur Anzeige der Lautstärke auf dem Konfigurationsbildschirm an
void showGain() {
  char txt[20];

  // Hintergrund des Bereichs mit der Farbe COLOR_SETTING_BG füllen
  tft.fillRect(0, 0, 320, 44, COLOR_SETTING_BG);

  // Einen Rand um den Bereich zeichnen
  tft.drawRect(0, 0, 320, 44, COLOR_SETTING_BORDER);

  // Lautstärkewert in Prozent formatieren und in txt speichern
  sprintf(txt, "%i %%", curGain);

  // Den Lautstärkewert rechtsbündig in einem Bereich anzeigen
  textInBox(231, 8, 80, 20, txt, ALIGNRIGHT, false, ILI9341_BLACK, COLOR_SETTING_BG, 1);

  // Cursorposition und Schriftart für das Label setzen
  tft.setCursor(5, 23);
  tft.setFont(FNT9);
  tft.setTextColor(ILI9341_BLACK);

  // Text für das Label "Lautstärke" kodieren und anzeigen
  encodeUnicode("Lautstärke", txt);
  tft.print(txt);

  // Den Schieberegler anzeigen, um die aktuelle Lautstärke darzustellen
  showSlider(27, curGain, 100);
}

// Zeigt den Bereich zur Lautstärkeanzeige auf dem Hauptbildschirm an
void showGainMain() {
  // Füllt einen Bereich auf dem Bildschirm (Box) mit der Hintergrundfarbe COLOR_SLIDER_BG
  tft.fillRect(0, 210, 320, 30, COLOR_SLIDER_BG);

  // Zeichnet einen Rahmen um den zuvor gefüllten Bereich mit der Farbe COLOR_SLIDER_BORDER
  tft.drawRect(0, 210, 320, 30, COLOR_SLIDER_BORDER);

  // Zeigt einen Schieberegler an, der die aktuelle Lautstärke (curGain) darstellt
  // Der Schieberegler beginnt bei der y-Position 218, hat einen maximalen Wert von 100
  // und verwendet die Farben COLOR_SLIDER_BG und COLOR_SLIDER
  showSlider(218, curGain, 100, COLOR_SLIDER_BG, COLOR_SLIDER);
}

// Zeigt den Bereich zur Helligkeitsanzeige auf dem Konfigurationsbildschirm an
void showBrigthness() {
  char txt[20];

  // Füllt einen rechteckigen Bereich auf dem Bildschirm mit der Hintergrundfarbe COLOR_SETTING_BG
  tft.fillRect(0, 44, 320, 44, COLOR_SETTING_BG);

  // Zeichnet einen Rahmen um den zuvor gefüllten Bereich mit der Farbe COLOR_SETTING_BORDER
  tft.drawRect(0, 44, 320, 44, COLOR_SETTING_BORDER);

  // Formatiert die Helligkeit (bright) als Prozentsatz und speichert sie in txt
  sprintf(txt, "%i %%", bright);

  // Zeigt den formatierten Text in einer Box an, rechtsbündig ausgerichtet
  textInBox(231, 52, 80, 20, txt, ALIGNRIGHT, false, ILI9341_BLACK, COLOR_SETTING_BG, 1);

  // Setzt den Cursor für den Text
  tft.setCursor(5, 67);

  // Wählt die Schriftart und die Textfarbe aus
  tft.setFont(FNT9);
  tft.setTextColor(ILI9341_BLACK);

  // Kodiert den Text "Helligkeit" in das Ziel-Array txt, um Umlaute darzustellen
  encodeUnicode("Helligkeit", txt);

  // Druckt den Text "Helligkeit" auf den Bildschirm
  tft.print(txt);

  // Zeigt einen Schieberegler an, um die aktuelle Helligkeit (bright) darzustellen
  showSlider(71, bright, 100);
}

// Funktion zur Anzeige der Snooze-Zeit auf dem Konfigurationsbildschirm
void showSnoozeTime() {
  char txt[20];  // Puffer für Text

  // Rechteck für den Hintergrund zeichnen
  tft.fillRect(0, 88, 320, 44, COLOR_SETTING_BG);

  // Rechteck für den Rahmen zeichnen
  tft.drawRect(0, 88, 320, 44, COLOR_SETTING_BORDER);

  // Aktuelle Snooze-Zeit als Text formatieren
  sprintf(txt, "%i min", snoozeTime);

  // Text für die Snooze-Zeit anzeigen
  textInBox(231, 96, 80, 20, txt, ALIGNRIGHT, false, ILI9341_BLACK, COLOR_SETTING_BG, 1);

  // Cursorposition setzen für den Beschriftungstext
  tft.setCursor(5, 111);

  // Schriftart festlegen
  tft.setFont(FNT9);

  // Textfarbe auf Schwarz setzen
  tft.setTextColor(ILI9341_BLACK);

  // Text "Einschlafzeit" mit kodierten Umlauten anzeigen
  encodeUnicode("Einschlafzeit", txt);
  tft.print(txt);

  // Slider zur Anzeige der Snooze-Zeit zeichnen
  showSlider(115, snoozeTime, 60);
}

// Funktion zur Anzeige der ausgewählten Station im Stationsbereich
void updateStation() {
  char txt[40];  // Puffer für den Stationsnamen

  // Stationsname kodieren (z.B. für Umlaute) und in den Puffer speichern
  encodeUnicode(stationlist[curStation].name, txt);

  // Kodierten Stationsnamen im Stationsbereich anzeigen
  textInBox(40, 145, 240, 20, txt, ALIGNCENTER, false, ILI9341_BLACK, COLOR_SETTING_BG, 1);
}

// Funktion zur Anzeige der Stationsliste im Einstellungsbildschirm
void showStationList() {
  // Bereich für die Stationsliste zeichnen
  tft.fillRect(0, 132, 320, 44, COLOR_SETTING_BG);      // Hintergrundfarbe setzen
  tft.drawRect(0, 132, 320, 44, COLOR_SETTING_BORDER);  // Rahmen zeichnen

  // Symbole für Hoch- und Runter-Scrollen anzeigen
  tft.drawBitmap(0, 132, symbole[4], 50, 44, COLOR_SETTING_UP_DOWN);    // Pfeil nach oben
  tft.drawBitmap(270, 132, symbole[5], 50, 44, COLOR_SETTING_UP_DOWN);  // Pfeil nach unten

  // Aktuelle Station als ausgewählte Station festlegen
  curStation = actStation;

  // Ausgewählte Station anzeigen
  updateStation();
}

// Funktion zur Anzeige des vollständigen Konfigurationsbildschirms
void showConfigPage() {
  setBGLight(100);           // Helligkeit des Hintergrundlichts auf 100% setzen
  tft.fillScreen(COLOR_BG);  // Bildschirmhintergrund mit der definierten Farbe füllen

  showGain();         // Lautstärkebereich anzeigen
  showBrigthness();   // Helligkeitsbereich anzeigen
  showSnoozeTime();   // Einschlafzeitbereich anzeigen
  showStationList();  // Bereich zur Anzeige der Stationsliste anzeigen

  DrawFooterButtons_Power_Sleep_Alarm();  // Schaltflächen für Power, Sleep und Alarm im Footer zeichnen

  // Schaltfläche für das Radio zeichnen
  tft.drawBitmap(192, 176, knopf_sym[4], 64, 64, COLOR_KNOEPFE, COLOR_KNOEPFE_BG);

  // Schaltfläche zum Wechseln auf den Alarmbildschirm (Uhranzeige) zeichnen
  tft.drawBitmap(256, 176, knopf_sym[7], 64, 64, COLOR_KNOEPFE, COLOR_KNOEPFE_BG);

  start_conf = millis();  // Startzeit der Konfigurationsseite festhalten
}

// Funktion zur Anzeige des vollständigen Radiobildschirms
void showRadioPage() {
  setBGLight(100);                   // Helligkeit des Hintergrundlichts auf 100% setzen
  tft.fillScreen(COLOR_KNOEPFE_BG);  // Bildschirmhintergrund mit der definierten Knopffarbe füllen

  showGain();         // Lautstärkebereich anzeigen
  FavoriteButtons();  // Favoriten-Schaltflächen anzeigen
  showStationList();  // Bereich zur Anzeige der Stationsliste anzeigen

  DrawFooterButtons_Power_Sleep_Alarm();  // Schaltflächen für Power, Sleep und Alarm im Footer zeichnen

  // Schaltfläche für das Radio zeichnen
  tft.drawBitmap(192, 176, knopf_sym[4], 64, 64, COLOR_KNOEPFE, COLOR_KNOEPFE_BG);

  // Schaltfläche zum Wechseln in die Einstellungen zeichnen
  tft.drawBitmap(256, 176, knopf_sym[6], 64, 64, COLOR_KNOEPFE, COLOR_KNOEPFE_BG);

  start_conf = millis();  // Startzeit der Radiokonfigurationsseite festhalten
}

// Funktion zur Anzeige des vollständigen Alarmbildschirms
void showAlarmPage() {
  // Aktualisieren der Alarmkonfiguration für den ersten Alarm
  alarmConfig.alarmday_1 = alarmday1;  // Wochentag des ersten Alarms setzen
  alarmConfig.h_1 = alarm1 / 60;       // Stundenwert des ersten Alarms berechnen und setzen
  alarmConfig.m_1 = alarm1 % 60;       // Minutenwert des ersten Alarms berechnen und setzen

  // Aktualisieren der Alarmkonfiguration für den zweiten Alarm
  alarmConfig.alarmday_2 = alarmday2;  // Wochentag des zweiten Alarms setzen
  alarmConfig.h_2 = alarm2 / 60;       // Stundenwert des zweiten Alarms berechnen und setzen
  alarmConfig.m_2 = alarm2 % 60;       // Minutenwert des zweiten Alarms berechnen und setzen

  setBGLight(100);                   // Helligkeit des Hintergrundlichts auf 100% setzen
  tft.fillScreen(COLOR_KNOEPFE_BG);  // Bildschirmhintergrund mit der definierten Knopffarbe füllen

  // Bereich für den ersten Alarm zeichnen
  tft.fillRect(0, 0, 320, 88, COLOR_SETTING_BG);
  tft.drawRect(0, 0, 320, 88, COLOR_SETTING_BORDER);

  // Bereich für den zweiten Alarm zeichnen
  tft.fillRect(0, 88, 320, 88, COLOR_SETTING_BG);
  tft.drawRect(0, 88, 320, 88, COLOR_SETTING_BORDER);

  // Schaltflächen für Stunden und Minuten des ersten Alarms zeichnen (hoch/runter)
  tft.drawBitmap(0, 44, symbole[5], 50, 44, COLOR_SETTING_UP_DOWN);    // Stunde hoch
  tft.drawBitmap(50, 44, symbole[4], 50, 44, COLOR_SETTING_UP_DOWN);   // Stunde runter
  tft.drawBitmap(220, 44, symbole[5], 50, 44, COLOR_SETTING_UP_DOWN);  // Minute hoch
  tft.drawBitmap(270, 44, symbole[4], 50, 44, COLOR_SETTING_UP_DOWN);  // Minute runter

  // Schaltflächen für Stunden und Minuten des zweiten Alarms zeichnen (hoch/runter)
  tft.drawBitmap(0, 132, symbole[5], 50, 44, COLOR_SETTING_UP_DOWN);    // Stunde hoch
  tft.drawBitmap(50, 132, symbole[4], 50, 44, COLOR_SETTING_UP_DOWN);   // Stunde runter
  tft.drawBitmap(220, 132, symbole[5], 50, 44, COLOR_SETTING_UP_DOWN);  // Minute hoch
  tft.drawBitmap(270, 132, symbole[4], 50, 44, COLOR_SETTING_UP_DOWN);  // Minute runter

  // Anzeige der Tage und Uhrzeiten für die Alarme
  showAlarms_Day_and_Time();  // Erste und dritte Zeile (MO-SO-Schaltflächen) und zweite und vierte Zeile (Uhrzeit)

  // Schaltflächen für Power, Sleep und Alarm im Footer zeichnen
  DrawFooterButtons_Power_Sleep_Alarm();

  // Schaltfläche zum Speichern der Alarmeinstellungen zeichnen
  tft.drawBitmap(192, 176, knopf_sym[8], 64, 64, COLOR_KNOEPFE, COLOR_KNOEPFE_BG);

  // Schaltfläche zum Wechseln zum Uhrenbildschirm zeichnen
  tft.drawBitmap(256, 176, knopf_sym[5], 64, 64, COLOR_KNOEPFE, COLOR_KNOEPFE_BG);

  start_conf = millis();  // Startzeit der Alarmkonfigurationsseite festhalten
}

// Funktion zur Anzeige der Alarmtage und -zeiten
void showAlarms_Day_and_Time() {
  uint8_t mask;        // Bitmaske für Wochentage
  uint16_t xPos;       // X-Position für die Anzeige
  uint16_t color_txt;  // Farbvariable für Text (nicht verwendet, aber deklariert)
  uint16_t color_bg;   // Farbvariable für Hintergrund
  char buf[45];        // Puffer für die Zeitformatierung

  // Schleife zur Anzeige der Wochentage für beide Alarme
  for (uint8_t i = 1; i < 8; i++) {
    xPos = 12 + (i - 1) * 44;  // Berechnung der X-Position für die Wochentage

    // Anzeige der Wochentage für alarmday_1
    mask = 1 << (i % 7);  // Bitmaske für den aktuellen Wochentag
    color_bg = ((alarmConfig.alarmday_1 & mask) == 0) ? COLOR_SETTING_BG : ILI9341_GREEN;
    textInBox(xPos, 8, 32, 32, days_short[i % 7], ALIGNCENTER, false, ILI9341_BLACK, color_bg, 1);

    // Anzeige der Wochentage für alarmday_2
    mask = 1 << (i % 7);  // Bitmaske für den aktuellen Wochentag
    color_bg = ((alarmConfig.alarmday_2 & mask) == 0) ? COLOR_SETTING_BG : ILI9341_GREEN;
    textInBox(xPos, 96, 32, 32, days_short[i % 7], ALIGNCENTER, false, ILI9341_BLACK, color_bg, 1);
  }

  // Anzeige der Zeit für alarm_1
  sprintf(buf, "%02i : %02i\n", alarmConfig.h_1, alarmConfig.m_1);
  textInBox(100, 52, 120, 32, buf, ALIGNCENTER, false, ILI9341_BLACK, COLOR_SETTING_BG, 1);

  // Anzeige der Zeit für alarm_2
  sprintf(buf, "%02i : %02i\n", alarmConfig.h_2, alarmConfig.m_2);
  textInBox(100, 140, 120, 32, buf, ALIGNCENTER, false, ILI9341_BLACK, COLOR_SETTING_BG, 1);
}

// Schaltet einen Tag für den Alarm um
void toggleAlarmDay(uint16_t xPos, uint16_t yPos) {
  uint8_t mask = 0B00000000;  // Initialisierung der Maske

  // Bestimme die Maske basierend auf der X-Position
  if (xPos < 50) mask = 0B00000010;                     // Mo
  if ((xPos > 50) && (xPos < 96)) mask = 0B00000100;    // Di
  if ((xPos > 96) && (xPos < 140)) mask = 0B00001000;   // Mi
  if ((xPos > 140) && (xPos < 184)) mask = 0B00010000;  // Do
  if ((xPos > 184) && (xPos < 228)) mask = 0B00100000;  // Fr
  if ((xPos > 228) && (xPos < 272)) mask = 0B01000000;  // Sa
  if (xPos > 272) mask = 0B00000001;                    // So

  // Toggle für alarmday_1, wenn die Y-Position im Bereich von 0 bis 44 liegt
  if ((yPos > 0) && (yPos < 44)) {
    alarmConfig.alarmday_1 = alarmConfig.alarmday_1 ^ mask;
  }
  // Toggle für alarmday_2, wenn die Y-Position im Bereich von 88 bis 132 liegt
  if ((yPos > 88) && (yPos < 132)) {
    alarmConfig.alarmday_2 = alarmConfig.alarmday_2 ^ mask;
  }

  // Zeige die aktualisierten Alarmtage und -zeiten an
  showAlarms_Day_and_Time();
}

// Inkrementiert oder dekrementiert die Alarm-Stunden
void in_de_crementAlarmTimeHH(uint16_t xPos, uint16_t yPos) {
  // In-/Dekrementierung für alarmtime_HH_1
  if ((yPos > 44) && (yPos < 88)) {
    // Inkrementierung der Stunden für alarmtime_HH_1
    if (xPos < 50) alarmConfig.h_1 = (alarmConfig.h_1 + 1) % 24;
    // Dekrementierung der Stunden für alarmtime_HH_1
    if ((xPos > 50) && (xPos < 100)) {
      if (alarmConfig.h_1 == 0) alarmConfig.h_1 = 24;  // Bei 0 Stunden auf 24 setzen
      alarmConfig.h_1 = alarmConfig.h_1 - 1;
    }
  }
  // In-/Dekrementierung für alarmtime_HH_2
  if ((yPos > 132) && (yPos < 176)) {
    // Inkrementierung der Stunden für alarmtime_HH_2
    if (xPos < 50) alarmConfig.h_2 = (alarmConfig.h_2 + 1) % 24;
    // Dekrementierung der Stunden für alarmtime_HH_2
    if ((xPos > 50) && (xPos < 100)) {
      if (alarmConfig.h_2 == 0) alarmConfig.h_2 = 24;  // Bei 0 Stunden auf 24 setzen
      alarmConfig.h_2 = alarmConfig.h_2 - 1;
    }
  }
  // Zeigt die aktualisierten Alarmtage und -zeiten an
  showAlarms_Day_and_Time();
}

// Inkrementiert oder dekrementiert die Alarm-Minuten in 5-Minuten-Schritten
void in_de_crementAlarmTimeMM(uint16_t xPos, uint16_t yPos) {
  uint8_t min1_1, min2_1, min1_2, min2_2;
  min1_1 = alarmConfig.m_1 / 10;  // Zehnerstelle für Minuten Alarm 1
  min2_1 = alarmConfig.m_1 % 10;  // Einerstelle für Minuten Alarm 1
  min1_2 = alarmConfig.m_2 / 10;  // Zehnerstelle für Minuten Alarm 2
  min2_2 = alarmConfig.m_2 % 10;  // Einerstelle für Minuten Alarm 2

  // Inkrementierung der Minuten
  if ((xPos > 220) && (xPos < 270)) {
    if ((yPos > 44) && (yPos < 88)) {  // Inkrementierung MM_1
      // Berechne die Minuten in 5-Minuten-Schritten vorwärts
      uint8_t currentMinutes_1 = min1_1 * 10 + min2_1;
      if (currentMinutes_1 % 5 == 0) {
        currentMinutes_1 += 5;  // Füge 5 Minuten hinzu, wenn der aktuelle Wert durch 5 teilbar ist
      } else {
        currentMinutes_1 = ((currentMinutes_1 + 4) / 5) * 5;  // Runde auf nächste durch 5 teilbare Zahl
      }
      if (currentMinutes_1 >= 60) currentMinutes_1 -= 60;  // Setze auf 0 Minuten zurück, wenn 60 erreicht wird
      // Trenne die Minuten in Zehner- und Einerstelle
      min1_1 = currentMinutes_1 / 10;
      min2_1 = currentMinutes_1 % 10;
      alarmConfig.m_1 = min1_1 * 10 + min2_1;
    }
    if ((yPos > 132) && (yPos < 176)) {  // Inkrementierung MM_2
      // Berechne die Minuten in 5-Minuten-Schritten vorwärts
      uint8_t currentMinutes_2 = min1_2 * 10 + min2_2;
      if (currentMinutes_2 % 5 == 0) {
        currentMinutes_2 += 5;  // Füge 5 Minuten hinzu, wenn der aktuelle Wert durch 5 teilbar ist
      } else {
        currentMinutes_2 = ((currentMinutes_2 + 4) / 5) * 5;  // Runde auf nächste durch 5 teilbare Zahl
      }
      if (currentMinutes_2 >= 60) currentMinutes_2 -= 60;  // Setze auf 0 Minuten zurück, wenn 60 erreicht wird
      // Trenne die Minuten in Zehner- und Einerstelle
      min1_2 = currentMinutes_2 / 10;
      min2_2 = currentMinutes_2 % 10;
      alarmConfig.m_2 = min1_2 * 10 + min2_2;
    }
  }

  // Dekrementierung der Minuten
  if (xPos > 270) {
    if ((yPos > 44) && (yPos < 88)) {  // Dekrementierung MM_1
      // Berechne die Minuten in 5-Minuten-Schritten rückwärts
      uint8_t currentMinutes_1 = min1_1 * 10 + min2_1;
      if (currentMinutes_1 % 5 == 0) {
        currentMinutes_1 -= 5;  // Subtrahiere 5 Minuten, wenn der aktuelle Wert durch 5 teilbar ist
      } else {
        currentMinutes_1 = ((currentMinutes_1 - 1) / 5) * 5;  // Runde auf vorherige durch 5 teilbare Zahl
      }
      if (currentMinutes_1 >= 60) currentMinutes_1 -= 60;  // Setze auf 55 Minuten zurück, wenn < 0
      // Trenne die Minuten in Zehner- und Einerstelle
      min1_1 = currentMinutes_1 / 10;
      min2_1 = currentMinutes_1 % 10;
      alarmConfig.m_1 = min1_1 * 10 + min2_1;
    }
    if ((yPos > 132) && (yPos < 176)) {  // Dekrementierung MM_2
      // Berechne die Minuten in 5-Minuten-Schritten rückwärts
      uint8_t currentMinutes_2 = min1_2 * 10 + min2_2;
      if (currentMinutes_2 % 5 == 0) {
        currentMinutes_2 -= 5;  // Subtrahiere 5 Minuten, wenn der aktuelle Wert durch 5 teilbar ist
      } else {
        currentMinutes_2 = ((currentMinutes_2 - 1) / 5) * 5;  // Runde auf vorherige durch 5 teilbare Zahl
      }
      if (currentMinutes_2 >= 60) currentMinutes_2 -= 60;  // Setze auf 55 Minuten zurück, wenn < 0
      // Trenne die Minuten in Zehner- und Einerstelle
      min1_2 = currentMinutes_2 / 10;
      min2_2 = currentMinutes_2 % 10;
      alarmConfig.m_2 = min1_2 * 10 + min2_2;
    }
  }
  // Zeigt die aktualisierten Alarmtage und -zeiten an
  showAlarms_Day_and_Time();
}

// Speichert die geänderte Alarmzeit und zeigt 'OK' an
void safeAlarmTime() {
  start_conf -= 9000;  // Setzt den Startzeitstempel der Konfigurationsseite zurück

  uint16_t alarmtime_1, alarmtime_2;
  alarmtime_1 = alarmConfig.h_1 * 60 + alarmConfig.m_1;  // Berechnet die Minuten für Alarm 1
  alarmtime_2 = alarmConfig.h_2 * 60 + alarmConfig.m_2;  // Berechnet die Minuten für Alarm 2

  alarm1 = alarmtime_1;                    // Speichert die Alarmzeit 1
  alarmday1 = alarmConfig.alarmday_1;      // Speichert die Alarmtage 1
  pref.putUInt("alarm1", alarm1);          // Speichert die Alarmzeit 1 in den Einstellungen
  pref.putUShort("alarmday1", alarmday1);  // Speichert die Alarmtage 1 in den Einstellungen

  alarm2 = alarmtime_2;                    // Speichert die Alarmzeit 2
  alarmday2 = alarmConfig.alarmday_2;      // Speichert die Alarmtage 2
  pref.putUInt("alarm2", alarm2);          // Speichert die Alarmzeit 2 in den Einstellungen
  pref.putUShort("alarmday2", alarmday2);  // Speichert die Alarmtage 2 in den Einstellungen

  tft.fillScreen(COLOR_BG);                                                          // Bildschirmhintergrund zurücksetzen
  textInBox(0, 0, 320, 240, TXT_OK, ALIGNCENTER, true, ILI9341_GREEN, COLOR_BG, 1);  // Zeigt 'OK' an

  findNextAlarm();  // Findet den nächsten Alarm
}

// Zeigt den Namen der aktiven Station auf dem TFT-Display an
void showStation() {
  textInBox(5, 122, 310, 30, stationlist[actStation].name, ALIGNCENTER, true, COLOR_STATION_NAME, COLOR_STATION_BOX_BG);
}

// Zeigt die aktuellen Metadaten auf dem TFT-Display an
void showTitle() {
  displayMessage(8, 152, 304, 57, title, ALIGNCENTER, false, COLOR_STATION_TITLE, COLOR_STATION_BOX_BG, 2);
}

// Zeigt den Radio-Bereich auf dem Uhr-Bildschirm an
void showRadio() {
  if (radio) {
    tft.fillRect(3, 117, 314, 93, COLOR_STATION_BOX_BG);      // Hintergrund des Radio-Bereichs füllen
    tft.drawRect(3, 117, 314, 93, COLOR_STATION_BOX_BORDER);  // Rahmen des Radio-Bereichs zeichnen
    showStation();                                            // Namen der aktiven Station anzeigen
    showTitle();                                              // Titel der aktuellen Station anzeigen
  } else {
    tft.fillRect(3, 117, 314, 93, COLOR_BG);  // Hintergrund des Radio-Bereichs auf Hintergrundfarbe setzen, wenn Radio aus ist
  }
}

// Wenn ein Wecker aktiv ist, wird das nächste Alarmdatum und die nächste Alarmzeit angezeigt
void showNextAlarm() {
  uint16_t color_alarm;  // Farbvariable für das Alarm-Symbol
  uint8_t symbol;        // Symbol-ID für das Alarm-Symbol
  char txt[50] = "";     // Text für die Anzeige
  uint8_t h, m;          // Stunden- und Minutenvariable für die Alarmzeit

  if (clockmode) {
    if (alarmday < 8) {                                          // Wenn der Wecker aktiviert ist
      color_alarm = COLOR_ALARM_SYMBOL;                          // Farbe des Alarm-Symbols
      symbol = 2;                                                // Symbol-ID für das Glocken-Symbol
      h = alarmtime / 60;                                        // Stunden berechnen
      m = alarmtime % 60;                                        // Minuten berechnen
      sprintf(txt, "%s %02i:%02i", days_short[alarmday], h, m);  // Text für die Alarmzeit formatieren
    } else {                                                     // Wenn der Wecker ausgeschaltet ist
      color_alarm = ILI9341_RED;                                 // Farbe des Alarm-Symbols
      symbol = 3;                                                // Symbol-ID für das durchgestrichene Glocken-Symbol
      sprintf(txt, "AUS");                                       // Text für ausgeschalteten Wecker
    }

    tft.drawBitmap(0, 0, symbole[symbol], 17, 17, color_alarm, COLOR_BG);         // Alarm-Symbol zeichnen
    textInBox(17, 0, 80, 17, txt, ALIGNCENTER, false, color_alarm, COLOR_BG, 1);  // Alarmtext anzeigen
  }
}

// Kann verwendet werden, um drei Ganzzahlen in der unteren Zeile des Displays anzuzeigen
void showDebugInfo(int16_t v1, int16_t v2, int16_t v3) {
  char txt[100] = "";                                                                      // Textpuffer für die Anzeige
  if (clockmode) {                                                                         // Nur anzeigen, wenn der Clockmodus aktiv ist
    sprintf(txt, "Info: v1 = %i, v2 = %i, v3 = %i", v1, v2, v3);                           // Formatierte Debug-Informationen erstellen
    textInBox(0, 220, 320, 20, txt, ALIGNCENTER, false, ILI9341_WHITE, ILI9341_BLACK, 1);  // Debug-Informationen anzeigen
  }
}

// Zeichne den Uhrbildschirm neu
void showClock() {
  start_conf = 0;            // Setze den Konfigurationsstartzeitpunkt zurück
  setBGLight(bright);        // Setze die Hintergrundbeleuchtung auf die aktuelle Helligkeit
  tft.fillScreen(COLOR_BG);  // Fülle den gesamten Bildschirm mit dem Hintergrundfarbwert
  updateTime(true);          // Aktualisiere die Uhrzeit und zeige sie an
  if (radio) showRadio();    // Zeige den Radiobereich, wenn der Radio-Modus aktiv ist
  // showNextAlarm wurde zu updateTime verschoben (nur jede Minute nötig)
  showGainMain();  // Zeige die aktuelle Lautstärke im Hauptbereich an
}


// Eine Abdeckfunktion für Text in der Box, die von anderen Unterfunktionen verwendet werden kann
void displayMessage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const char* text, uint8_t align, boolean big, uint16_t fc, uint16_t bg, uint8_t lines) {
  textInBox(x, y, w, h, text, align, big, fc, bg, lines);  // Rufe die Funktion zum Zeichnen des Texts in der Box auf
}

// Erstelle Favoriten-Buttons auf der Radioliste zwischen y44 und y132 und wechsle direkt zu ihnen
void FavoriteButtons() {
  int y = 44;           // Start-Y-Position für die Buttons
  bool active = false;  // Statusvariable, ob der Button aktiv ist
  int loopCount = 0;    // Zähler für die Schleife

  for (int i = 0; i < STATIONS && loopCount < 10; i++) {
    if (stationlist[i].enabled) {  // Prüfe, ob der Station-Button aktiviert ist
      active = (actStation == i);  // Aktiviere Button für die aktuelle Station
      tft.drawBitmap((loopCount % 5) * 64, y, num_64_44[loopCount], 64, 44,
                     active ? COLOR_FAV_BUTTONS_AKTIV : COLOR_FAV_BUTTONS, COLOR_FAV_BUTTONS_BG);
      loopCount++;               // Inkrementiere Schleifenzähler
      if (loopCount % 5 == 0) {  // Wechsle zur nächsten Zeile alle 5 Buttons
        y = 88;                  // Setze Y-Position für die nächste Reihe
      }
    }
  }
}
