#define RADIOVERSION "v5.0.1"
#include "00_librarys.h"      //Lade alle benötigten Bibliotheken
#include "00_pin_settings.h"  //Einstellungen der genutzten Pins
#include "00_settings.h"      //einstellungen
#include "00_texte.h"         //Strings
#include "weather.h"
/* Kein #include <esp_bt.h>: spart Include-Kette + #pragma-Warnungen; Linker bindet die Funktion trotzdem. */
#if defined(ARDUINO_ARCH_ESP32) && defined(CONFIG_IDF_TARGET_ESP32) && CONFIG_IDF_TARGET_ESP32
extern "C" int esp_bt_controller_mem_release(int mode);
#ifndef ESP_BT_MODE_BTDM
#define ESP_BT_MODE_BTDM 3
#endif
#endif
// Vor tft_display.ino (Arduino fügt die .ino alphabetisch danach ein)
extern AudioGenerator *decoder;  // Definition in audio.ino
void showStartPage(void);
void toggleRadio(boolean off);
// predefined function from modul tft_display.ino
void displayMessage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const char* text, uint8_t align = ALIGNLEFT, boolean big = false, uint16_t fc = ILI9341_WHITE, uint16_t bg = ILI9341_BLACK, uint8_t lines = 1);

// Liest die aktuelle Uhrzeit und berechnet weekday/minutes
void updateCurrentTime() {
  if (getLocalTime(&ti)) {
    weekday = ti.tm_wday;                    // 0=Sonntag ... 6=Samstag
    minutes = ti.tm_hour * 60 + ti.tm_min;   // Minuten seit Mitternacht
  }
}

// Sucht den nächsten gültigen Alarm
void findNextAlarm() {
  Serial.println("Search next alarm time");

  if (!getLocalTime(&ti)) return; // keine Zeit verfügbar

  // aktuelle Zeitvariablen aktualisieren
  updateCurrentTime();

  alarmday = 8; // kein Alarm
  alarmtime = MINUTES_PER_DAY + 1;

  // Hilfsfunktion: Prüft eine Weckzeit
  auto checkAlarm = [&](uint8_t daysMask, int alarmMinutes, int currentMinutes, int day) {
    if (alarmMinutes <= 0) return; // nicht gesetzt
    if (daysMask & (1 << day)) {   // Tag ist aktiv
      if (day == weekday) {
        // Heute nur, wenn Uhrzeit noch in Zukunft
        if (alarmMinutes > currentMinutes && alarmMinutes < alarmtime) {
          alarmtime = alarmMinutes;
          alarmday = day;
        }
      } else {
        // Kommende Tage → übernehmen, falls kleiner als bisher
        if (alarmMinutes < alarmtime) {
          alarmtime = alarmMinutes;
          alarmday = day;
        }
      }
    }
  };

  // 1️⃣ Heute prüfen
  checkAlarm(alarmday1, alarm1, minutes, weekday);
  checkAlarm(alarmday2, alarm2, minutes, weekday);

  // 2️⃣ Falls nichts gefunden → nächste 7 Tage durchsuchen
  if (alarmday == 8) {
    for (int i = 1; i <= 7; i++) {
      int nextDay = (weekday + i) % 7;
      alarmtime = MINUTES_PER_DAY + 1; // Reset für Tag
      checkAlarm(alarmday1, alarm1, -1, nextDay);
      checkAlarm(alarmday2, alarm2, -1, nextDay);
      if (alarmday != 8) break; // gefunden
    }
  }

  // Ausgabe
  if (alarmday != 8) {
    Serial.printf(
      "Next alarm: %02d:%02d on %s\n",
      alarmtime / 60, alarmtime % 60,
      days_short[alarmday]
    );
  } else {
    Serial.println("No alarm found");
  }
}

// Initialisierung des Systems
void setup() {
  Serial.begin(115200);
#if defined(ARDUINO_ARCH_ESP32) && defined(CONFIG_IDF_TARGET_ESP32) && CONFIG_IDF_TARGET_ESP32
  esp_bt_controller_mem_release(ESP_BT_MODE_BTDM);  /* klassischer ESP32: BT-RAM für App freigeben */
#endif
  Serial.println("Load preferences");  // Debug-Ausgabe: "Lade Einstellungen"

  title[0] = 0;  // Setze das erste Zeichen des Titels auf 0 (leerer Titel)

  // Lade die gespeicherten Präferenzen aus dem EEPROM des ESP32
  // Die Präferenzen werden in zwei Themen gespeichert: "radio" und "senderliste"
  // Beide Themen werden hier geladen
  pref.begin("radio", false);
  sender.begin("senderlist", false);

  // Werte aus den gespeicherten Präferenzen abrufen, falls vorhanden
  if (pref.isKey("ssid")) ssid = pref.getString("ssid");  // SSID für WLAN-Verbindung
  if (pref.isKey("pkey")) pkey = pref.getString("pkey");  // Passkey für WLAN-Verbindung
  if (pref.isKey("ntp")) ntp = pref.getString("ntp");     // NTP-Server-URL für die Zeitabgleich
  if (pref.isKey("TIME_ZONE_IANA")) TIME_ZONE_IANA = pref.getString("TIME_ZONE_IANA");  // Zeitzone abrufen
  if (pref.isKey("LATITUDE")) LATITUDE = pref.getFloat("LATITUDE");   // Latitude für die Wetterdaten
  if (pref.isKey("LONGITUDE")) LONGITUDE = pref.getFloat("LONGITUDE");  // Longitude für die Wetterdaten


  curGain = 50;                                              // Standardwert für Lautstärke
  if (pref.isKey("gain")) curGain = (uint8_t)constrain((int)pref.getUShort("gain"), 0, 100);

  snoozeTime = 30;                                                  // Standardwert für Schlummerzeit in Minuten
  if (pref.isKey("snooze")) snoozeTime = pref.getUShort("snooze");  // Abrufen der Schlummerzeit

  bright = 80;                                                  // Standardwert für Helligkeit in Prozent
  if (pref.isKey("bright")) bright = pref.getUShort("bright");  // Abrufen des Helligkeitswerts

  alarm1 = 390;                                               // Standardwert für die erste Alarmzeit (6:30)
  if (pref.isKey("alarm1")) alarm1 = pref.getUInt("alarm1");  // Abrufen der ersten Alarmzeit

  alarmday1 = 0B00111110;                                                // Standardwert für die Tage (Mo-Fr)
  if (pref.isKey("alarmday1")) alarmday1 = pref.getUShort("alarmday1");  // Abrufen der Wochentage für den ersten Alarm

  alarm2 = 480;                                               // Standardwert für die zweite Alarmzeit (8:00)
  if (pref.isKey("alarm2")) alarm2 = pref.getUInt("alarm2");  // Abrufen der zweiten Alarmzeit

  alarmday2 = 0B01000001;                                                // Standardwert für die Tage (So, Sa)
  if (pref.isKey("alarmday2")) alarmday2 = pref.getUShort("alarmday2");  // Abrufen der Wochentage für den zweiten Alarm

  alarmtime = 0;  // Startwert für die nächste Alarmzeit
  alarmday = 8;   // Startwert für den nächsten Alarmtag (8 bedeutet, Alarm ist deaktiviert)

  curStation = 0;  // Standardwert für die aktuelle Station
  // Setze die aktuelle Station auf den gespeicherten Wert, falls vorhanden
  if (pref.isKey("station")) curStation = pref.getUShort("station");
  // Überprüfe, ob die aktuelle Station innerhalb des gültigen Bereichs liegt
  if (curStation >= STATIONS) curStation = 0;

  actStation = curStation;  // Setze die aktive Station auf die aktuelle Station

  // Debug-Ausgabe der aktuellen Station, Lautstärke, SSID und NTP-Server
  Serial.printf("station %i, gain %i, ssid %s, ntp %s\n", curStation, curGain, ssid.c_str(), ntp.c_str());
  Serial.printf_P(PSTR("heap vor setup_audio: %u\n"), (unsigned)ESP.getFreeHeap());

  // Führe die Setup-Funktionen für Audio, Display und Senderliste aus
  setup_audio();       // Initialisiere die Audio-Streams
  Serial.printf_P(PSTR("heap nach setup_audio: %u\n"), (unsigned)ESP.getFreeHeap());
  Serial.println(F("setup_display…"));
  setup_display();     // Initialisiere die Display-Schnittstelle
  Serial.printf_P(PSTR("heap nach setup_display: %u\n"), (unsigned)ESP.getFreeHeap());
  Serial.println(F("setup_display OK"));
  /* Audio-Puffer erst bei erstem startUrl: sonst ~70 KiB weniger Heap → LwIP/Wetter/TCP-Timeouts. */
  setup_senderList();  // Lade die Senderliste aus den Präferenzen
  setGain(curGain);    // gespeicherte Lautstärke auf I2S (0 = dauerhaft stumm bis zum nächsten Slider-Zug)

  // Versuche, eine WLAN-Verbindung herzustellen und zeige den Fortschritt auf dem Display an
  displayClear();                                                                                          // Bildschirm löschen
  displayMessage(5, 10, 310, 30, TXT_CONNECTING_TO, ALIGNCENTER, true, ILI9341_YELLOW, ILI9341_BLACK, 1);  // Zeige "Verbinden zu" auf dem Display an
  displayMessage(5, 50, 310, 30, ssid.c_str(), ALIGNCENTER, true, ILI9341_GREEN, ILI9341_BLACK, 1);        // Zeige die SSID auf dem Display an
  Serial.println("Connect WiFi");                                                                          // Debug-Ausgabe: "Verbinde zu WiFi"

  // Versuche, sich mit dem WLAN zu verbinden
  connected = initWiFi(ssid, pkey);

  // Wenn die Verbindung erfolgreich war
  if (connected) {
    // Konfiguriere die Echtzeituhr mit der Zeitzone und dem NTP-Server
    configTzTime(TIME_ZONE, ntp.c_str());

    // Zeige Datum und Uhrzeit sowie den Namen der Station an
    delay(500);  // Kurze Verzögerung für die Anzeige
    // Fülle die Zeitstruktur ti, Minuten und Wochentag mit der aktuellen Zeit
    getLocalTime(&ti);
    minutes = ti.tm_hour * 60 + ti.tm_min;  // Berechne Minuten seit Mitternacht
    weekday = ti.tm_wday;                   // Wochentag (0 = Sonntag, 1 = Montag, usw.)

    Serial.println("Start");  // Debug-Ausgabe: "Start"

    // Wenn der Alarm aktiviert ist, berechne das Datum und die Uhrzeit für den nächsten Alarm
    if (pref.isKey("alarmon") && pref.getBool("alarmon")) findNextAlarm();
  } else {  // Wenn die Verbindung nicht erfolgreich war
    // Es konnte keine Verbindung hergestellt werden. Eine entsprechende Nachricht wird auf dem Display angezeigt
    displayClear();                                                                                         // Bildschirm löschen
    displayMessage(5, 10, 310, 30, TXT_NOT_CONNECTED, ALIGNCENTER, true, ILI9341_RED, ILI9341_BLACK, 1);    // Zeige "Nicht verbunden" auf dem Display an
    displayMessage(5, 50, 310, 30, TXT_CONNECT_TO_AP, ALIGNCENTER, true, ILI9341_WHITE, ILI9341_BLACK, 1);  // Zeige "Verbinden zu AP" auf dem Display an
    displayMessage(5, 100, 310, 30, AP_NAME, ALIGNCENTER, true, ILI9341_GREEN, ILI9341_BLACK, 1);           // Zeige "Ap Name" auf dem Display an
    displayMessage(5, 150, 310, 30, TXT_CONFIG_IP, ALIGNCENTER, true, ILI9341_WHITE, ILI9341_BLACK, 1);     // Zeige "IP konfigurieren" auf dem Display an
    displayMessage(5, 200, 310, 30, "192.168.4.1", ALIGNCENTER, true, ILI9341_GREEN, ILI9341_BLACK, 1);     // Zeige "IP " auf dem Display an

    // Merke den aktuellen Zeitstempel, um später nach 5 Minuten einen Retry zu ermöglichen
    discon = millis();
  }

  /* Webserver vor Startseite (showStartPage): weniger Heap-Fragmentierung vor WebServer::begin(). */
  Serial.println("Start webserver");  // Debug-Ausgabe: "Starte Webserver"
  setup_webserver();
  setup_ota();

  if (connected) {
    showStartPage();
  }

  start_conf = 0;  // Setze den Startwert für die Konfiguration
}

void loop() {
  // Überprüfe und verarbeite mögliche OTA-Updates
  ArduinoOTA.handle();

  /* Audio vor Webserver/LVGL: weniger Pufferunderruns bei gleichzeitigem WLAN-Traffic und UI. */
  if (connected && (radio || decoder)) {
    audio_loop();
  }

  // Verarbeite HTTP-Anfragen
  webserver_loop();

  // Überprüfe Touch-Ereignisse
  touch_loop();

  /* Nach LVGL: nochmal füttern — Screen-Wechsel kann lv_timer_handler länger blocken (Underruns/Knackser). */
  if (connected && (radio || decoder)) {
    audio_loop();
  }

  // Nach 10 Sekunden nicht auf der Startseite → zurück (Favoriten/Einstellungen/Alarm)
  if (!startpage && ((millis() - start_conf) > 10000)) {
    showStartPage(); /* setzt u. a. startpage = true */
  }

  // Streamtitel nur auf der Startseite ins Widget übernehmen
  if (newTitle && radio && startpage) {
    showTitle();       // Zeige den Titel des aktuellen Radiosenders an
    newTitle = false;  // Setze das Flag zurück
  }

  // Erkenne einen Verbindungsabbruch
  if (connected && (WiFi.status() != WL_CONNECTED)) {
    connected = false;  // Setze die Verbindung auf false
    discon = millis();  // Merke den aktuellen Zeitstempel für einen automatischen Retry
    displayClear();     // Bildschirm löschen
    // Zeige eine Nachricht an, dass die Verbindung verloren gegangen ist
    displayMessage(5, 10, 310, 30, TXT_NOT_CONNECTED, ALIGNCENTER, true, ILI9341_RED, ILI9341_BLACK, 1);
  }

  // Erkenne eine Wiederverbindung
  if (!connected && (WiFi.status() == WL_CONNECTED)) {
    connected = true;  // Setze die Verbindung auf true
    lastWeatherUpdate = 0;  // Wetter nach Reconnect zeitnah neu holen (nicht bis Intervall warten)
    displayClear();    // Bildschirm löschen
    // Zeige eine Nachricht an, dass die Verbindung wiederhergestellt wurde
    displayMessage(5, 10, 310, 30, TXT_RECONNECTED, ALIGNCENTER, true, ILI9341_GREEN, ILI9341_BLACK, 1);
    reconnected = true;
  }

  // Wenn die Helligkeit auf 0 gesetzt ist, wird die Helligkeit durch das Umgebungslicht gesteuert
  if (bright == 0) {
    int16_t tmp = analogRead(LDR);  // Lese den Wert vom Lichtsensor (LDR)
    int16_t diff = lastldr - tmp;   // Berechne die Differenz zum vorherigen Wert
    diff = abs(diff);               // Nehme den absoluten Wert der Differenz

    // LDR-Helligkeit nur auf der Startseite anwenden
    if (startpage && (diff > 50)) {
      setBGLight(bright);                                  // Setze die Hintergrundbeleuchtung
      Serial.printf("ldr %i letzter %i\n", tmp, lastldr);  // Debug-Ausgabe der aktuellen und letzten LDR-Werte
      lastldr = tmp;                                       // Merke den aktuellen LDR-Wert für die nächste Messung
    }
  }

  // Wetter nur bei echter WLAN-Verbindung (ohne STA z. B. nur AP → kein Internet → kein HTTP)
  if (millis() - lastWeatherUpdate > weatherUpdateInterval || lastWeatherUpdate == 0) {
    if (connected && !radio && startpage) {
      String weatherData = getWeatherData(LATITUDE, LONGITUDE, TIME_ZONE_IANA);
      displayWeather(weatherData);
    }
    lastWeatherUpdate = millis();
  }

  // Zeitgesteuertes Ereignis: Update der Anzeige alle 1 Sekunde
  if ((millis() - lastUpdate) > 1000) {
    lastUpdate = millis();  // Aktualisiere den Zeitstempel

    // Regelmäßige Prüfung der Schlummerzeit
    if (millis() > snoozeTimeEnd && snoozeTimeEnd > 0) {
      snoozeTimeEnd = 0;  // Setze den Schlummermodus zurück
      toggleRadio(true);  // Schalte das Radio aus
      showRadio();        // Zeige Radio-Status an
    }

    // Hole das Datum und die Uhrzeit
    if (connected && getLocalTime(&ti)) {
      minutes = ti.tm_hour * 60 + ti.tm_min;  // Berechne Minuten seit Mitternacht
      weekday = ti.tm_wday;                   // Wochentag (0 = Sonntag, 1 = Montag, usw.)
    }
    // Datum/Uhrzeit-Update nur auf der Startseite
    if (connected && startpage) {

      if(!reconnected){
        setBGLight(bright);  // Setze die Hintergrundbeleuchtung
        displayDateTime();   // Zeige Datum und Uhrzeit an ohne komplettes Display neu zu zeichnen
      }else{
        showStartPage();   // Zeige Datum und Uhrzeit mit komplett neu zu zeichnen
        reconnected = false;
      }

    }

    // Wenn ein Alarm aktiviert ist, überprüfe den Tag und die Zeit
    if ((alarmday < 8) && getLocalTime(&ti)) {
      // Wenn der Alarmtag und die Zeit erreicht sind, schalte das Radio ein und berechne die Werte für den nächsten erwarteten Alarm
      if ((alarmday == weekday) && (minutes == alarmtime) && !alarmTriggered) {
        // Test Beeper#####################
        alarmTriggered = true;  // verhindert mehrfaches Auslösen
        // Test Beeper#####################
        toggleRadio(false);  // Schalte das Radio an
        showRadio();         // Zeige Radio-Informationen an
        findNextAlarm();     // Berechne den nächsten Alarm
        showNextAlarm();     // Zeige den nächsten Alarm an
      }
    }
  }

  // Reset-Flag, wenn Zeit weiterläuft und wir nicht mehr im Alarm-Minutenbereich sind
if (minutes != alarmtime) {
    alarmTriggered = false;
}

  // Starte einen Neustart, wenn das Gerät mehr als 5 Minuten getrennt war
  if (!connected && ((millis() - discon) > 300000)) ESP.restart();
}
