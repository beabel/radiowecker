#define RADIOVERSION "v4.0.1";
#include "00_librarys.h"      //Lade alle benötigten Bibliotheken
#include "00_pin_settings.h"  //Einstellungen der genutzten Pins
#include "00_settings.h"      //einstellungen
#include "00_texte.h"         //Strings
#include "weather.h"
//predefined function from modul tft_display.ino
void displayMessage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const char* text, uint8_t align = ALIGNLEFT, boolean big = false, uint16_t fc = ILI9341_WHITE, uint16_t bg = ILI9341_BLACK, uint8_t lines = 1);

// Berechnet das Datum und die Uhrzeit für den nächsten erwarteten Alarm
// Setzt `alarmtime` auf die Zeit des Tages und `alarmday` auf den Wochentag für den Alarm
// oder auf 8, wenn kein Alarm vorhanden ist
void findNextAlarm() {
  Serial.println("Search next alarm time");
  int wd;        // Variable für den aktuellen Wochentag
  uint8_t mask;  // Maske zum Filtern von Wochentagen

  if (getLocalTime(&ti)) {            // Hole das aktuelle Datum und die Uhrzeit
    wd = weekday;                     // Setze wd auf den aktuellen Wochentag
    alarmday = 8;                     // Setze alarmday auf 8, was bedeutet, dass kein Alarm vorhanden ist
    alarmtime = MINUTES_PER_DAY + 1;  // Setze alarmtime auf einen Wert, der höher ist als der maximal mögliche Wert für Minuten pro Tag

    mask = 1 << wd;  // Erstelle eine Maske für den aktuellen Wochentag

    // Überprüfe, ob die Alarmzeit 1 übereinstimmt und ob der Alarm vor der aktuellen Zeit liegt
    if (((alarmday1 & mask) != 0) && (alarm1 > minutes)) {
      alarmtime = alarm1;  // Setze alarmtime auf alarm1
      alarmday = wd;       // Setze alarmday auf den aktuellen Wochentag
    }

    // Überprüfe, ob die Alarmzeit 2 übereinstimmt und ob der Alarm vor alarm1 liegt
    if (((alarmday2 & mask) != 0) && (alarm2 > minutes) && (alarmtime > alarm2)) {
      alarmtime = alarm2;  // Setze alarmtime auf alarm2
      alarmday = wd;       // Setze alarmday auf den aktuellen Wochentag
    }

    // Wenn kein Alarm gefunden wurde, fahre fort mit der Suche nach dem nächsten Alarm in der kommenden Woche
    if (alarmday == 8) {  // Wenn alarmday immer noch 8 ist (kein Alarm gefunden)
      do {
        wd++;                // Gehe zum nächsten Wochentag
        if (wd > 7) wd = 0;  // Wenn der Wochentag über 7 hinausgeht, setze ihn auf 0 (Sonntag)

        mask = 1 << wd;  // Erstelle eine neue Maske für den aktuellen Wochentag

        // Überprüfe erneut Alarmzeit 1
        if ((alarmday1 & mask) != 0) {
          alarmtime = alarm1;  // Setze alarmtime auf alarm1
          alarmday = wd;       // Setze alarmday auf den aktuellen Wochentag
        }

        // Überprüfe erneut Alarmzeit 2
        if (((alarmday2 & mask) != 0) && (alarmtime > alarm2)) {
          alarmtime = alarm2;  // Setze alarmtime auf alarm2
          alarmday = wd;       // Setze alarmday auf den aktuellen Wochentag
        }

      } while ((alarmday == 8) && (wd != weekday));  // Wiederhole, bis ein gültiger Alarm gefunden wird oder eine Woche vorbei ist
    }
    Serial.printf("Next alarm %i on %i\n", alarmtime, alarmday);  // Gib die nächste Alarmzeit und den Wochentag aus
  }
}

// Initialisierung des Systems
void setup() {
  // Starte die serielle Kommunikation mit einer Baudrate von 115200
  Serial.begin(115200);
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
  if (pref.isKey("gain")) curGain = pref.getUShort("gain");  // Abrufen des Lautstärkewerts

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

  // Führe die Setup-Funktionen für Audio, Display und Senderliste aus
  setup_audio();       // Initialisiere die Audio-Streams
  setup_display();     // Initialisiere die Display-Schnittstelle
  setup_senderList();  // Lade die Senderliste aus den Präferenzen
  setGain(0);          // Setze die aktuelle Lautstärke

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

    // Zeige Uhrzeit und nächsten Alarm auf dem Display an
    showClock();
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

  Serial.println("Start webserver");  // Debug-Ausgabe: "Starte Webserver"

  // Initialisiere den Webserver und das OTA-Update
  setup_webserver();  // Initialisiere den Webserver
  setup_ota();        // Initialisiere das OTA-Update

  start_conf = 0;  // Setze den Startwert für die Konfiguration
}

void loop() {
  long rssi = WiFi.RSSI();  // Hole das Signalstärke- (RSSI) Level des WiFi

  // Überprüfe und verarbeite mögliche OTA-Updates
  ArduinoOTA.handle();

  // Verarbeite HTTP-Anfragen
  webserver_loop();

  // Überprüfe Touch-Ereignisse
  touch_loop();

  // Nach 10 Sekunden im Konfigurationsmodus, zurück zum Uhrmodus wechseln
  if (!clockmode && ((millis() - start_conf) > 10000)) {
    showClock();       // Zeige die Uhrzeit an
    clockmode = true;  // Setze den Modus auf Uhr
  }

  // Zeige Metadaten an, wenn das Radio aktiv ist und wir im Uhrmodus sind
  if (newTitle && radio && clockmode) {
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
    displayClear();    // Bildschirm löschen
    // Zeige eine Nachricht an, dass die Verbindung wiederhergestellt wurde
    displayMessage(5, 10, 310, 30, TXT_RECONNECTED, ALIGNCENTER, true, ILI9341_GREEN, ILI9341_BLACK, 1);
    reconnected = true;
  }

  // Wenn das Radio eingeschaltet ist, hole neue Stream-Daten
  if (connected && radio) {
    audio_loop();  // Verarbeite Audiodaten
  }

  // Wenn die Helligkeit auf 0 gesetzt ist, wird die Helligkeit durch das Umgebungslicht gesteuert
  if (bright == 0) {
    int16_t tmp = analogRead(LDR);  // Lese den Wert vom Lichtsensor (LDR)
    int16_t diff = lastldr - tmp;   // Berechne die Differenz zum vorherigen Wert
    diff = abs(diff);               // Nehme den absoluten Wert der Differenz

    // Setze die Hintergrundbeleuchtung, wenn die Uhr angezeigt wird und eine signifikante Änderung des Umgebungslichts erkannt wurde
    if (clockmode && (diff > 50)) {
      setBGLight(bright);                                  // Setze die Hintergrundbeleuchtung
      Serial.printf("ldr %i letzter %i\n", tmp, lastldr);  // Debug-Ausgabe der aktuellen und letzten LDR-Werte
      lastldr = tmp;                                       // Merke den aktuellen LDR-Wert für die nächste Messung
    }
  }

  // Wetterdaten beim ersten Durchlauf sofort abrufen (wenn lastWeatherUpdate == 0) oder alle 60 Sekunden
  if (millis() - lastWeatherUpdate > weatherUpdateInterval || lastWeatherUpdate == 0) {
    if (!radio && clockmode) {
      String weatherData = getWeatherData(LATITUDE, LONGITUDE, TIME_ZONE_IANA);
      displayWeather(weatherData);
    }
    lastWeatherUpdate = millis();  // Zeitstempel für das nächste Update
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
    // Setze die Hintergrundbeleuchtung, wenn die Uhr angezeigt wird
    if (connected && clockmode) {

      if(!reconnected){
        setBGLight(bright);  // Setze die Hintergrundbeleuchtung
        displayDateTime();   // Zeige Datum und Uhrzeit an ohne komplettes Display neu zu zeichnen
      }else{
        showClock();   // Zeige Datum und Uhrzeit mit komplett neu zu zeichnen
        reconnected = false;
      }

    }

    // Wenn ein Alarm aktiviert ist, überprüfe den Tag und die Zeit
    if ((alarmday < 8) && getLocalTime(&ti)) {
      // Wenn der Alarmtag und die Zeit erreicht sind, schalte das Radio ein und berechne die Werte für den nächsten erwarteten Alarm
      if ((alarmday == weekday) && ((minutes == alarmtime) || (minutes == (alarmtime + 1)))) {
        // Test Beeper#####################

        // Test Beeper#####################
        toggleRadio(false);  // Schalte das Radio an
        showRadio();         // Zeige Radio-Informationen an
        findNextAlarm();     // Berechne den nächsten Alarm
        showNextAlarm();     // Zeige den nächsten Alarm an
      }
    }
  }

  // Starte einen Neustart, wenn das Gerät mehr als 5 Minuten getrennt war
  if (!connected && ((millis() - discon) > 300000)) ESP.restart();
}
