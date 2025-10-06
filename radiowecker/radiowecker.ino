#define RADIOVERSION "v4.0.3";
#include "00_librarys.h"      //Lade alle benötigten Bibliotheken
#include "00_pin_settings.h"  //Einstellungen der genutzten Pins
#include "00_settings.h"      //einstellungen
#include "00_texte.h"         //Strings
#include "weather.h"

// ************************************************************
// ERWEITERUNG: GLOBALE VARIABLEN FÜR TASTER-LOGIK (DYNAMISCH)
// ************************************************************

// SNOOZE-LOGIK-VARIABLE
uint16_t snoozeEndTime = 0; 

// VARIABLEN FÜR ALARM-STOP TASTER (GPIO5)
unsigned long lastStopButtonPressTime = 0;
int stopButtonState = HIGH;      
int lastStopButtonState = HIGH; 

// VARIABLEN FÜR SNOOZE TASTER (GPIO6)
unsigned long lastSnoozeButtonPressTime = 0;
int snoozeButtonState = HIGH;
int lastSnoozeButtonState = HIGH; 

// DEKLARATIONEN (Muss vor loop() stehen)
void handleStopButton();
void handleSnoozeButton();
void snoozeAlarm();
// ************************************************************
#define CONFIG_ADC_DRIVER_INCLUDED 



//predefined function from modul tft_display.ino
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
  // Starte die serielle Kommunikation mit einer Baudrate von 115200
  Serial.begin(115200);
  Serial.println("Load preferences");  // Debug-Ausgabe: "Lade Einstellungen"

  pinMode(ALARM_STOP_BUTTON_PIN, INPUT_PULLUP);
  pinMode(SNOOZE_BUTTON_PIN, INPUT_PULLUP);

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

  snoozeTime = 10;                                                  // Standardwert für Schlummerzeit in Minuten
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


// ************************************************************
// NEUE FUNKTIONEN: SNOOZE UND TASTER-HANDLER
// ************************************************************

void snoozeAlarm() {
  Serial.println("Snooze-Funktion aktiviert (10 Min).");
  
  // 1. Radio ausschalten
  toggleRadio(true);
  
  // 2. Snooze-Endzeit berechnen: Aktuelle Minuten + 10 Minuten
  // Die Konstante MINUTES_PER_DAY (1440) wird für den Modulo-Operator verwendet.
  uint16_t currentMinutes = minutes; 
  snoozeEndTime = (currentMinutes + snoozeTime) % 1440;
  
  // 3. Alarm-Flag zurücksetzen
  alarmTriggered = false; 
  
  Serial.print("Snooze endet um Minute: ");
  Serial.println(snoozeEndTime);
}

void handleStopButton() {
  // Entprell-Logik für Taster GPIO5 (Stop)
  int reading = digitalRead(ALARM_STOP_BUTTON_PIN);

  if (reading != lastStopButtonState) {
    lastStopButtonPressTime = millis();
  }

  if (reading != lastStopButtonState) {
     Serial.printf("Stop Button: Reading is now %s\n", reading == HIGH ? "HIGH" : "LOW");
  }

  if ((millis() - lastStopButtonPressTime) > debounceDelay) {
    if (reading != stopButtonState) {
      stopButtonState = reading; 

      if (stopButtonState == LOW) { // Taster gedrückt
        Serial.println("Stop-Taster gedrückt. Beende Alarm/Snooze.");
        
        // Stoppt den Alarm, wenn er läuft (alarmTriggered) oder gesnoozed wird (snoozeEndTime > 0)
        if (alarmTriggered || snoozeEndTime > 0) { 
          
          toggleRadio(true);      // Radio stoppen
          alarmTriggered = false; // Alarm-Flag zurücksetzen
          snoozeEndTime = 0;      // Snooze-Zeit zurücksetzen
          
          showClock(); // Display-Aktualisierung
        }
      }
    }
  }
  lastStopButtonState = reading;
}

void handleSnoozeButton() {
  // Entprell-Logik für Taster GPIO6 (Snooze)
  int reading = digitalRead(SNOOZE_BUTTON_PIN);

  if (reading != lastSnoozeButtonState) {
    lastSnoozeButtonPressTime = millis();
  }

  if ((millis() - lastSnoozeButtonPressTime) > debounceDelay) {
    if (reading != snoozeButtonState) {
      snoozeButtonState = reading;

      if (snoozeButtonState == LOW) { // Taster gedrückt
        Serial.println("Snooze-Taster gedrückt.");
        if (alarmTriggered || snoozeEndTime > 0) { 
            snoozeAlarm(); // Snooze aktivieren (rechnet neue 10 Minuten hinzu)
        }        
        // Snooze nur, wenn der Alarm gerade läuft (alarmTriggered)
        //if (alarmTriggered) { 
        //    snoozeAlarm();
        //} 
      }
    }
  }
  lastSnoozeButtonState = reading;
}
// ************************************************************


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

  // Taster-Logik ausführen (NEU)
  handleStopButton(); 
  handleSnoozeButton();

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
      
      // ************************************************************
      // NEU: SNOOZE-ENDE-PRÜFUNG (Muss VOR dem Haupt-Alarm-Trigger stehen)
      // ************************************************************
      // Prüfe, ob die Snooze-Zeit abgelaufen ist und der Alarm manuell neu gestartet werden muss
      if (snoozeEndTime > 0 && minutes == snoozeEndTime) {
        Serial.println("Snooze-Zeit abgelaufen. Starte Radio neu.");
        snoozeEndTime = 0; // Snooze beendet
        
        toggleRadio(false); // Radio an
        alarmTriggered = true; // Alarm-Flag setzen
        showRadio();
      }
      // ************************************************************

      // Wenn der Alarmtag und die Zeit erreicht sind, schalte das Radio ein und berechne die Werte für den nächsten erwarteten Alarm
      // ERWEITERT: Starte den Alarm NUR, wenn wir NICHT snoozen (snoozeEndTime == 0).
      if ((alarmday == weekday) && (minutes == alarmtime) && !alarmTriggered && snoozeEndTime == 0) {
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
//if (minutes != alarmtime && snoozeEndTime == 0) { 
//    alarmTriggered = false;
//}

  // Starte einen Neustart, wenn das Gerät mehr als 5 Minuten getrennt war
  if (!connected && ((millis() - discon) > 300000)) ESP.restart();
}
