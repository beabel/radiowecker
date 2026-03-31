//home page and templatefor options
#include <string.h>
#include "index.h"
#include "ArduinoJson.h"
#include "i18n.h"

void handleStartHttpUpdate(void);
void getLang(void);
void setLang(void);

WebServer server(80);

// Konfiguriert den Webserver und richtet die Routen für verschiedene HTTP-Anfragen ein
void setup_webserver() {
  // Optional: Aktiviert CORS (Cross-Origin Resource Sharing) für den Server
  // server.enableCORS();

  // Definiert die Route für die Root-URL, die auf die Funktion `handleRoot` verweist
  server.on("/", handleRoot);

  // Definiert die Routen für verschiedene AJAX-Befehle und verknüpft sie mit den entsprechenden Funktionen
  server.on("/cmd/stations", sendStations);            // Ruft die Liste der verfügbaren Stationen ab
  server.on("/cmd/restorestations", restoreStations);  // Stellt die gespeicherten Stationen wieder her
  server.on("/cmd/restart", restart);                  // Startet das System neu
  server.on("/cmd/setaccess", setAccessData);          // Setzt die Zugangsdaten
  server.on("/cmd/getaccess", getAccessData);          // Ruft die Zugangsdaten ab
  server.on("/cmd/getalarms", getAlarms);              // Ruft die Alarmkonfiguration ab
  server.on("/cmd/setalarms", setAlarms);              // Setzt die Alarmkonfiguration
  server.on("/cmd/getstation", getStationData);        // Ruft die Daten der aktuellen Station ab
  server.on("/cmd/setstation", setStationData);        // Setzt die Daten der aktuellen Station
  server.on("/cmd/teststation", testStation);          // Testet die Verbindung zur aktuellen Station
  server.on("/cmd/endtest", endTest);                  // Beendet den Testmodus

  // Player-Tab: Definiert Routen für Steuerbefehle des Players
  server.on("/cmd/startPlay", startPlay);                // Startet die Wiedergabe
  server.on("/cmd/stopPlay", stopPlay);                  // Stoppt die Wiedergabe
  server.on("/cmd/GainSlider", GainSlider);              // Setzt den Wert des Lautstärkereglers
  server.on("/cmd/btnAlarm", btnAlarm);                  // Schaltet den Alarm ein oder aus
  server.on("/cmd/startSleep", startSleep);              // Startet den Schlafmodus
  server.on("/cmd/beforeStation", beforeStation);        // Wechselt zur vorherigen Station
  server.on("/cmd/nextStation", nextStation);            // Wechselt zur nächsten Station
  server.on("/cmd/getCurrentStatus", getCurrentStatus);  // Ruft den aktuellen Status ab

  // Info-Tab: Definiert die Route für das Abrufen von Systeminformationen
  server.on("/cmd/getInfo", getInfo);  // Ruft allgemeine Systeminformationen ab
  server.on("/cmd/getlang", getLang);
  server.on("/cmd/setlang", setLang);
  /* Kein Upload-Callback: sonst nimmt WebServer 3.x den RAW-Pfad, arg("plain") bleibt leer (JSON-OTA schlägt fehl). */
  server.on("/cmd/startHttpUpdate", HTTP_POST, handleStartHttpUpdate, nullptr);

  // Startet den Webserver
  server.begin();
}

// Wird aus der Hauptschleife aufgerufen, um eingehende HTTP-Anfragen zu verarbeiten
void webserver_loop() {
  server.handleClient();  // Bearbeitet alle an den Webserver gesendeten Anfragen
}

// Bearbeitet Anfragen an die Wurzel-URL ("/") des Webservers
void handleRoot() {
  if (connected) {
    /* PROGMEM-HTML per send_P: send() baut ein RAM-String aus dem ganzen Dokument
       (Arduino-ESP32 WebServer) — bei großer index.h + wenig Heap: leere/kaputte Seite. */
    server.send_P(200, PSTR("text/html"), MAIN_page);
  } else {
    // Wenn nicht verbunden, sende die Konfigurationsseite
    // Parameter werden als Argumente im HTML-Request übergeben
    RADIO_SERIAL({
      Serial.println("Got config:");
      uint8_t a = server.args();  // Anzahl der Argumente im Request
      Serial.print(a);
      Serial.println(" Arguments");
      for (uint8_t i = 0; i < a; i++) Serial.println(server.arg(i));  // Ausgabe aller Argumente
    });

    // Speichern der SSID in den Einstellungen, falls vorhanden
    if (server.hasArg("conf_ssid")) {
      strlcpy(ssid, server.arg("conf_ssid").c_str(), sizeof(ssid));
      pref.putString("ssid", ssid);
      RADIO_SERIAL(Serial.println(ssid));
    }

    // Speichern des PKEY in den Einstellungen, falls vorhanden
    if (server.hasArg("conf_pkey")) {
      strlcpy(pkey, server.arg("conf_pkey").c_str(), sizeof(pkey));
      pref.putString("pkey", pkey);
      RADIO_SERIAL(Serial.println(pkey));
    }

    // Falls das Reset-Argument vorhanden ist, starte das System neu
    if (server.hasArg("reset")) {
      RADIO_SERIAL(Serial.println("Restart!"));
      ESP.restart();
    }

    server.send_P(200, PSTR("text/html"), CONFIG_page);
  }
}

// Bearbeitet AJAX-Befehle für "/cmd/stations"
// Sendet eine Liste der verfügbaren Stationen als HTML-Optionen
void sendStations() {
  char* s;        // Variable für das Symbol vor dem Stationsnamen
  char* sel;      // Variable für das 'selected'-Attribut der aktuellen Station
  char buf[100];  // Buffer für die HTML-Ausgabe

  // Bereite die HTML-Antwort vor
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, "text/html", "");

  // Sende alle Stationen als Optionsliste
  for (uint8_t i = 0; i < STATIONS; i++) {
    // Füge das 'selected'-Attribut hinzu, wenn die Station aktuell ist
    sel = "";
    if (i == actStation) sel = "selected";

    // Setze ein schwarzes Punkt-Symbol vor dem Stationsnamen, wenn aktiviert
    s = "&#x2002;";
    if (stationlist[i].enabled) s = "&#x25cf;";

    // Erstelle den Eintrag für die Option mit der Vorlage
    sprintf(buf, OPTION_entry, i, sel, s, stationlist[i].name);
    server.sendContent(buf);  // Sende den Inhalt der Option
  }
}

// Bearbeitet AJAX-Befehle für "/cmd/setaccess"
// Speichert Zugangsdaten in den Einstellungen
void setAccessData() {
  // Der Befehl erwartet drei Argumente
  // Die Zugangsdaten werden in den Einstellungen gespeichert

  // Speichere die SSID, wenn vorhanden
  if (server.hasArg("ssid")) {
    strlcpy(ssid, server.arg("ssid").c_str(), sizeof(ssid));
    pref.putString("ssid", ssid);
  }

  // Speichere den Pre-Shared Key (PKEY), wenn vorhanden
  if (server.hasArg("pkey")) {
    strlcpy(pkey, server.arg("pkey").c_str(), sizeof(pkey));
    pref.putString("pkey", pkey);
  }

  // Speichere die NTP-Server-Adresse, wenn vorhanden
  if (server.hasArg("ntp")) {
    strlcpy(ntp, server.arg("ntp").c_str(), sizeof(ntp));
    pref.putString("ntp", ntp);
  }

  // Speichere die Timezone, wenn vorhanden
  if (server.hasArg("TIME_ZONE_IANA")) {
    strlcpy(TIME_ZONE_IANA, server.arg("TIME_ZONE_IANA").c_str(), sizeof(TIME_ZONE_IANA));
    pref.putString("TIME_ZONE_IANA", TIME_ZONE_IANA);
  }

// Speichere die Latitude, wenn sie vorhanden ist
if (server.hasArg("LATITUDE")) {
  LATITUDE = server.arg("LATITUDE").toFloat();
  pref.putFloat("LATITUDE", LATITUDE);
}

// Speichere die Longitude, wenn sie vorhanden ist
if (server.hasArg("LONGITUDE")) {
  LONGITUDE = server.arg("LONGITUDE").toFloat();
  pref.putFloat("LONGITUDE", LONGITUDE);
}

  // Antworte mit "OK"
  server.send(200, "text/plain", "OK");
}

// Bearbeitet AJAX-Befehle für "/cmd/getaccess"
// Sendet Zugangsdaten als Textnachricht
void getAccessData() {
  char msg[384];
  snprintf(msg, sizeof(msg), "%s\n%s\n%s\n%s\n%.6f\n%.6f\n", ssid, pkey, ntp, TIME_ZONE_IANA, (double)LATITUDE,
           (double)LONGITUDE);
  server.send(200, "text/plain", msg);
}


// Bearbeitet AJAX-Befehle für "/cmd/getalarms"
// Sendet die Alarmzeiten und die Wochentage für zwei Alarme als Textnachricht
void getAlarms() {
  char buf[45];
  uint8_t h, m, mask, i;

  // Konvertiere die Alarmzeiten in Stunden und Minuten und formatiere sie
  h = alarm1 / 60;
  m = alarm1 % 60;
  sprintf(buf, "%02i:%02i\n", h, m);

  h = alarm2 / 60;
  m = alarm2 % 60;
  sprintf(&buf[20], "%02i:%02i\n", h, m);

  // Füge die Wochentage für beide Alarme hinzu
  for (i = 0; i < 7; i++) {
    mask = 1 << i;
    buf[6 + i * 2] = ((alarmday1 & mask) != 0) ? '1' : '0';
    buf[26 + i * 2] = ((alarmday2 & mask) != 0) ? '1' : '0';
    buf[7 + i * 2] = '\n';
    buf[27 + i * 2] = '\n';
  }

  buf[40] = 0;  // Null-Terminierung des Strings

  // Antwort mit den Alarmdaten als Textnachricht (kein String(buf): spart Heap-Kopie)
  server.send(200, "text/plain", buf);
}

// Berechnet die Gesamtanzahl der Minuten aus "HH:MM" ohne Arduino-String/substring-Allokationen
static uint16_t stringToMinutes(const char *val) {
  int h, m;
  if (!val || sscanf(val, "%d:%d", &h, &m) != 2) return 0;
  if (h < 0 || h > 23 || m < 0 || m > 59) return 0;
  return (uint16_t)(h * 60 + m);
}

// Verarbeitet den AJAX-Befehl /cmd/setalarms zum Setzen der Alarmzeiten
void setAlarms() {
  char txt[10];

  // Debug-Ausgabe: Beginn des Setzens der Alarme
  RADIO_SERIAL(Serial.println("Set alarms start"));

  // Überprüfe, ob der Parameter "al0" vorhanden ist und setze alarm1
  if (server.hasArg("al0")) {
    alarm1 = stringToMinutes(server.arg("al0").c_str());  // Konvertiere die Zeit in Minuten
    RADIO_SERIAL(Serial.print(server.arg("al0")));              // Debug-Ausgabe der übergebenen Zeit
    RADIO_SERIAL(Serial.printf(" = %i\n", alarm1));             // Debug-Ausgabe des berechneten Alarmwerts
    pref.putUInt("alarm1", alarm1);               // Speichere den Alarmwert in den Einstellungen
  }

  // Überprüfe, ob der Parameter "al8" vorhanden ist und setze alarm2
  if (server.hasArg("al8")) {
    alarm2 = stringToMinutes(server.arg("al8").c_str());  // Konvertiere die Zeit in Minuten
    RADIO_SERIAL(Serial.print(server.arg("al8")));              // Debug-Ausgabe der übergebenen Zeit
    RADIO_SERIAL(Serial.printf(" = %i\n", alarm2));             // Debug-Ausgabe des berechneten Alarmwerts
    pref.putUInt("alarm2", alarm2);               // Speichere den Alarmwert in den Einstellungen
  }

  // Initialisiere die Wochentags-Flags für beide Alarme
  alarmday1 = 0;
  alarmday2 = 0;

  // Überprüfe die Wochentags-Parameter für alarmday1 und alarmday2
  for (uint8_t i = 0; i < 7; i++) {
    // Erstelle den Parameter-Namen für alarmday1
    sprintf(txt, "al%i", i + 1);
    if (server.hasArg(txt)) {
      if (server.arg(txt) == "1") alarmday1 = alarmday1 | (1 << i);  // Setze das entsprechende Bit für alarmday1
    }

    // Erstelle den Parameter-Namen für alarmday2
    sprintf(txt, "al%i", i + 9);
    if (server.hasArg(txt)) {
      if (server.arg(txt) == "1") alarmday2 = alarmday2 | (1 << i);  // Setze das entsprechende Bit für alarmday2
    }
  }

  // Speichere die Wochentags-Flags in den Einstellungen
  pref.putUShort("alarmday1", alarmday1);
  pref.putUShort("alarmday2", alarmday2);

  // Debug-Ausgabe der Wochentags-Flags
  RADIO_SERIAL(Serial.printf("days1 %x days2 %x\n", alarmday1, alarmday2));

  // Nächster Alarm berechnen; Kopfzeile nur aktualisieren, wenn Startseite sichtbar ist
  findNextAlarm();
  if (startpage) showNextAlarm();

  // Sende eine Bestätigung als Antwort
  server.send(200, "text/plain", "OK");
}

// Verarbeitet den AJAX-Befehl /cmd/getstation, um Daten einer spezifischen Station abzurufen
void getStationData() {
  // Überprüft, ob der Parameter "stationid" vorhanden ist
  if (server.hasArg("stationid")) {
    uint8_t i = server.arg("stationid").toInt();  // Liest die Station-ID aus der Anfrage

    // Wenn die Station-ID außerhalb des gültigen Bereichs liegt, setze sie auf die letzte Station
    if (i >= STATIONS) i = STATIONS - 1;

    /* Festpuffer statt String-Ketten (4× weniger dynamische Allokation pro Request) */
    char msg[32 + 150 + 16];
    snprintf(msg, sizeof(msg), "%s\n%s\n%u\n%u", stationlist[i].name, stationlist[i].url, (unsigned)stationlist[i].enabled,
             (unsigned)(i + 1));

    server.send(200, "text/plain", msg);
  } else {
    // Wenn der Parameter fehlt oder ungültig ist, sende eine Fehlermeldung
    server.send(200, "text/plain", "Invalid command");
  }
}

// Verarbeitet den AJAX-Befehl /cmd/setstation, um Daten einer bestimmten Station zu setzen
void setStationData() {
  // Erwartet eine gültige Station-ID als Argument
  // Daten werden ebenfalls als Argumente übermittelt:
  // name = Name der Station
  // url = URL der Station
  // enabled = Aktivierungsstatus
  // position = neue Position in der Liste
  char key[5];

  // Überprüft, ob die Station-ID vorhanden ist und innerhalb des gültigen Bereichs liegt
  if ((server.hasArg("stationid")) && (server.arg("stationid").toInt() < STATIONS)) {
    uint8_t i = server.arg("stationid").toInt();

    // Speichert die Werte aus den Argumenten in der Stationenliste
    if (server.hasArg("name")) {
      sprintf(key, "n%i", i);
      sender.putString(key, server.arg("name"));
      strlcpy(stationlist[i].name, server.arg("name").c_str(), 32);
    }
    if (server.hasArg("url")) {
      sprintf(key, "u%i", i);
      sender.putString(key, server.arg("url"));
      strlcpy(stationlist[i].url, server.arg("url").c_str(), 150);
    }
    if (server.hasArg("enabled")) {
      sprintf(key, "f%i", i);
      sender.putUChar(key, server.arg("enabled").toInt());
      stationlist[i].enabled = server.arg("enabled").toInt();
    }
    if (server.hasArg("position")) {
      int16_t newpos = server.arg("position").toInt();
      newpos--;
      RADIO_SERIAL(Serial.printf("Move %i to position %i\n", i + 1, newpos + 1));
      if ((i != newpos) && (newpos >= 0) && (newpos < STATIONS)) {
        reorder(i, newpos);  // Ändert die Position der Station in der Liste
        saveList();          // Speichert die aktualisierte Liste
      }
    }

    // Sendet eine Bestätigung der erfolgreichen Verarbeitung zurück
    server.send(200, "text/plain", "OK");
  } else {
    // Sendet eine Fehlermeldung zurück, wenn die Station-ID ungültig ist
    server.send(200, "text/plain", "Invalid station ID");
  }
}

// Verarbeitet den AJAX-Befehl /cmd/teststation, um eine URL zu testen
void testStation() {
  // Erwartet ein Argument mit der zu testenden URL
  bool ret = true;

  // Überprüft, ob das URL-Argument vorhanden ist
  if (server.hasArg("url")) {
    // Versucht, die URL zu starten und speichert das Ergebnis
    ret = startUrl(server.arg("url").c_str());
  }

  // Sendet eine Bestätigung bei Erfolg oder eine Fehlermeldung bei Misserfolg
  if (ret) {
    // Bei Erfolg: Antwort "OK" senden
    server.send(200, "text/plain", "OK");
  } else {
    // Bei Misserfolg: Wechselt zurück zur aktuellen Station und sendet "ERROR"
    startUrl(stationlist[actStation].url);
    server.send(300, "text/plain", "ERROR");
  }
}

// Verarbeitet den AJAX-Befehl /cmd/endtest, um den Test zu beenden
void endTest() {
  // Wechselt zurück zur aktuellen Station, um den Test zu beenden
  startUrl(stationlist[actStation].url);

  // Sendet eine Bestätigung, dass der Test beendet wurde
  server.send(200, "text/plain", "OK");
}

// Verarbeitet den AJAX-Befehl /cmd/restorestations, um die Stationen wiederherzustellen
void restoreStations() {
  restore();  // Stellt die Stationen aus dem Speicher wieder her

  // Sendet eine Bestätigung, dass die Wiederherstellung erfolgreich war
  server.send(200, "text/plain", "OK");
}

// Verarbeitet den AJAX-Befehl /cmd/restart, um das Gerät neu zu starten
void restart() {
  ESP.restart();  // Startet das Gerät neu
}

// Player Tab ######################
// Verarbeitet den AJAX-Befehl /cmd/startPlay, um das Radio zu starten
void startPlay() {
  toggleRadio(false);                    // Schaltet das Radio ein
  server.send(200, "text/plain", "OK");  // Antwortet mit "OK"
}

// Verarbeitet den AJAX-Befehl /cmd/stopPlay, um das Radio zu stoppen
void stopPlay() {
  toggleRadio(true);                     // Schaltet das Radio aus
  server.send(200, "text/plain", "OK");  // Antwortet mit "OK"
}

// Verarbeitet den AJAX-Befehl /cmd/GainSlider, um den Lautstärkewert zu setzen
void GainSlider() {
  // Überprüft, ob der Parameter "GainValue" in der Anfrage vorhanden ist
  if (server.hasArg("GainValue")) {
    // Konvertiert den Wert des Parameters in einen float
    float floatWert = atof(server.arg("GainValue").c_str());
    // Setzt den Lautstärkewert
    setGainValue(floatWert, "WEBSITE");
    // Antwortet mit "OK"
    server.send(200, "text/plain", "OK");

    // Gibt den Wert für Debugging-Zwecke aus
    RADIO_SERIAL({
      Serial.print("GainValue: ");
      Serial.println(floatWert);
    });
  } else {
    // Antwortet mit einem Fehlercode, wenn der Parameter fehlt
    server.send(400, "text/plain", "ERROR");
  }
}

// Verarbeitet den AJAX-Befehl /cmd/btnAlarm, um den Alarm ein- oder auszuschalten
void btnAlarm() {
  // Schaltet den Alarm um
  toggleAlarm();
  // Antwortet mit "OK"
  server.send(200, "text/plain", "OK");
}

// Verarbeitet den AJAX-Befehl /cmd/startSleep, um die Schlummerfunktion zu starten
void startSleep() {
  // Startet die Schlummerfunktion
  startSnooze();
  // Antwortet mit "OK"
  server.send(200, "text/plain", "OK");
}

// Verarbeitet den AJAX-Befehl /cmd/beforeStation, um zur vorherigen "aktiven" Station zu wechseln
void beforeStation() {
  if (station_navigate_prev())
    server.send(200, "text/plain", "OK");
  else
    server.send(200, "text/plain", i18n_str(I18N_ERR_NO_STATION));
}

// Verarbeitet den AJAX-Befehl /cmd/nextStation, um zur nächsten "aktiven" Station zu wechseln
void nextStation() {
  if (station_navigate_next())
    server.send(200, "text/plain", "OK");
  else
    server.send(200, "text/plain", i18n_str(I18N_ERR_NO_STATION));
}

// AJAX-Befehl /cmd/getCurrentStatus
// Diese Funktion wird aufgerufen, um den aktuellen Status des Systems zu ermitteln und als JSON-Daten zurückzugeben.
// Die JSON-Antwort enthält Informationen über:
// - Die Lautstärke des Radios
// - Den Alarmstatus und die Alarmzeit
// - Den aktuellen Radiosender und Titel (wenn das Radio aktiv ist)
// - Datum und Uhrzeit
// - Die WLAN-Signalstärke
// - Den Status des Sleeptimers (ob aktiv oder nicht)
void getCurrentStatus() {
  // Variablen zur Zeitberechnung
  uint8_t h, m;
  char txt[50] = "";

  /* Größer: lange UTF-8-Strings (Sender + Titel) */
  StaticJsonDocument<512> jsonDoc;

  // Lautstärke des Radios
  jsonDoc["gain"] = curGain;

  // Status des Alarms
  if (alarmday < 8) {  // Alarm ist aktiv
    h = alarmtime / 60;
    m = alarmtime % 60;
    sprintf(txt, "%s %02i:%02i", i18n_day_short((int)alarmday), h, m);
    jsonDoc["alarm"] = "1";
  } else {  // Alarm ist ausgeschaltet
    snprintf(txt, sizeof(txt), "%s", i18n_str(I18N_ALARM_OFF));
    jsonDoc["alarm"] = "0";
  }
  jsonDoc["alarmtime"] = txt;

  // Radio-Status
  if (radio) {
    jsonDoc["radioStation"] = stationlist[actStation].name;
    jsonDoc["radioTitle"] = title;
  } else {
    jsonDoc["radioStation"] = "";
    jsonDoc["radioTitle"] = "";
  }

  // Aktuelles Datum und Uhrzeit
  char tim[40];
  if (getLocalTime(&ti)) {
    sprintf(tim, "%s %i. %s %i", i18n_day_long(ti.tm_wday), ti.tm_mday, i18n_month_short(ti.tm_mon), ti.tm_year + 1900);
    jsonDoc["Date"] = tim;
    strftime(tim, sizeof(tim), "%H:%M", &ti);
    jsonDoc["Time"] = tim;
  }

  // WLAN-Signalstärke
  jsonDoc["Rssi"] = WiFi.RSSI();

  // Status des Sleeptimers
  jsonDoc["Sleep"] = snoozeTimeEnd != 0 ? 1 : 0;

  String response;
  serializeJson(jsonDoc, response);

  server.send(200, "application/json; charset=utf-8", response);
}

//################ Info Tab
// AJAX-Befehl /cmd/getInfo
// Diese Funktion wird aufgerufen, um detaillierte Systeminformationen als JSON-Daten zurückzugeben.
// Die JSON-Antwort enthält:
// - Die installierte Version des Radios
// - Informationen zum aktuellen ESP8266/ESP32-Chip, einschließlich:
//   - Heap-Größe und freier Heap-Speicher
//   - Sketch-Größe und freier Platz in der App-Partition (Summe = Partitionsgröße)
//   - Chip-Modell
void getLang() {
  char buf[48];
  snprintf(buf, sizeof(buf), "{\"lang\":\"%s\"}", i18n_lang_code());
  server.send(200, "application/json; charset=utf-8", buf);
}

void setLang() {
  if (!server.hasArg("l")) {
    server.send(400, "text/plain", "missing l");
    return;
  }
  String v = server.arg("l");
  uint8_t l = 255;
  if (v == "de")
    l = 0;
  else if (v == "en")
    l = 1;
  else if (v == "fr")
    l = 2;
  else if (v == "ru")
    l = 3;
  else if (v == "es")
    l = 4;
  else if (v == "nl")
    l = 5;
  else {
    int n = v.toInt();
    if (n >= 0 && n <= 5) l = (uint8_t)n;
  }
  if (l > 5) {
    server.send(400, "text/plain", "bad lang");
    return;
  }
  i18n_set_lang(l);
  server.send(200, "text/plain", "OK");
}

void getInfo() {
  // Erstellen eines JSON-Dokuments für die Antwort
  StaticJsonDocument<512> jsonDoc;

  // Installierte Radio-Version
  jsonDoc["radioversion"] = RADIOVERSION;
  jsonDoc["uiLang"] = i18n_lang_code();
  jsonDoc["httpOtaAsset"] = HTTP_OTA_FIRMWARE_FILENAME;
  jsonDoc["httpOtaFreeBytes"] = ESP.getFreeSketchSpace();

  // Erstellen eines Unterobjekts für ESP-Board-Informationen
  JsonObject ESP_INFO = jsonDoc.createNestedObject("ESP_INFO");

  // HEAP-Informationen
  JsonObject HEAP = ESP_INFO.createNestedObject("HEAP");
  HEAP["getHeapSize"] = ESP.getHeapSize();  // Gesamte Heap-Größe
  HEAP["getFreeHeap"] = ESP.getFreeHeap();  // Freier Heap-Speicher
#if defined(ESP32)
  HEAP["getMaxAllocHeap"] = ESP.getMaxAllocHeap(); /* größter zusammenhängender Block (malloc/TCP) */
#endif

  // SKETCH-Informationen
  JsonObject SKETCH = ESP_INFO.createNestedObject("SKETCH");
  SKETCH["getSketchSize"] = ESP.getSketchSize();            // Gesamte Sketch-Größe
  SKETCH["getFreeSketchSpace"] = ESP.getFreeSketchSpace();  // Freier Sketch-Speicher

  // CHIP-Informationen
  JsonObject CHIP = ESP_INFO.createNestedObject("CHIP");
  CHIP["getChipModel"] = ESP.getChipModel();  // Modell des verwendeten Chips

  // Konvertieren des JSON-Dokuments in einen String
  String response;
  serializeJson(jsonDoc, response);

  // Senden der JSON-Antwort an den Client
  server.send(200, "application/json", response);
}