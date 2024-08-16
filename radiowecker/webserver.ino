//home page and templatefor options
#include "index.h"
#include "ArduinoJson.h"

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
    // Wenn verbunden, sende die Hauptseite
    server.send(200, "text/html", MAIN_page);
  } else {
    // Wenn nicht verbunden, sende die Konfigurationsseite
    // Parameter werden als Argumente im HTML-Request übergeben
    Serial.println("Got config:");
    uint8_t a = server.args();  // Anzahl der Argumente im Request
    Serial.print(a);
    Serial.println(" Arguments");
    for (uint8_t i = 0; i < a; i++) Serial.println(server.arg(i));  // Ausgabe aller Argumente

    // Speichern der SSID in den Einstellungen, falls vorhanden
    if (server.hasArg("conf_ssid")) {
      pref.putString("ssid", server.arg("conf_ssid"));
      Serial.println(server.arg("conf_ssid"));
    }

    // Speichern des PKEY in den Einstellungen, falls vorhanden
    if (server.hasArg("conf_pkey")) {
      pref.putString("pkey", server.arg("conf_pkey"));
      Serial.println(server.arg("conf_pkey"));
    }

    // Falls das Reset-Argument vorhanden ist, starte das System neu
    if (server.hasArg("reset")) {
      Serial.println("Restart!");
      ESP.restart();
    }

    // Sende die Konfigurationsseite an den Client
    server.send(200, "text/html", CONFIG_page);
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
    ssid = server.arg("ssid");
    pref.putString("ssid", ssid);
  }

  // Speichere den Pre-Shared Key (PKEY), wenn vorhanden
  if (server.hasArg("pkey")) {
    pkey = server.arg("pkey");
    pref.putString("pkey", pkey);
  }

  // Speichere die NTP-Server-Adresse, wenn vorhanden
  if (server.hasArg("ntp")) {
    ntp = server.arg("ntp");
    pref.putString("ntp", ntp);
  }

  // Antworte mit "OK"
  server.send(200, "text/plain", "OK");
}

// Bearbeitet AJAX-Befehle für "/cmd/getaccess"
// Sendet Zugangsdaten als Textnachricht
void getAccessData() {
  // Erstelle eine Nachricht mit den Zugangsdaten, getrennt durch Zeilenumbrüche
  String msg = String(ssid) + "\n" + String(pkey) + "\n" + String(ntp);

  // Antworte mit den Zugangsdaten
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

  // Antwort mit den Alarmdaten als Textnachricht
  server.send(200, "text/plain", String(buf));
}

// Berechnet die Gesamtanzahl der Minuten aus einer Stunden- und Minutenangabe im Format "HH:MM"
// Gibt die Minuten als uint16_t zurück
uint16_t stringToMinutes(String val) {
  uint8_t h, m;
  // Extrahiere die Stunden aus den ersten zwei Zeichen des Strings und konvertiere sie in eine Ganzzahl
  h = val.substring(0, 2).toInt();
  // Extrahiere die Minuten aus den Zeichen nach dem Doppelpunkt und konvertiere sie in eine Ganzzahl
  m = val.substring(3).toInt();
  // Berechne die Gesamtanzahl der Minuten und gebe sie zurück
  return h * 60 + m;
}

// Verarbeitet den AJAX-Befehl /cmd/setalarms zum Setzen der Alarmzeiten
void setAlarms() {
  char txt[10];
  uint8_t b;

  // Debug-Ausgabe: Beginn des Setzens der Alarme
  Serial.println("Set alarms start");

  // Überprüfe, ob der Parameter "al0" vorhanden ist und setze alarm1
  if (server.hasArg("al0")) {
    alarm1 = stringToMinutes(server.arg("al0"));  // Konvertiere die Zeit in Minuten
    Serial.print(server.arg("al0"));              // Debug-Ausgabe der übergebenen Zeit
    Serial.printf(" = %i\n", alarm1);             // Debug-Ausgabe des berechneten Alarmwerts
    pref.putUInt("alarm1", alarm1);               // Speichere den Alarmwert in den Einstellungen
  }

  // Überprüfe, ob der Parameter "al8" vorhanden ist und setze alarm2
  if (server.hasArg("al8")) {
    alarm2 = stringToMinutes(server.arg("al8"));  // Konvertiere die Zeit in Minuten
    Serial.print(server.arg("al8"));              // Debug-Ausgabe der übergebenen Zeit
    Serial.printf(" = %i\n", alarm2);             // Debug-Ausgabe des berechneten Alarmwerts
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
  Serial.printf("days1 %x days2 %x\n", alarmday1, alarmday2);

  // Finde den nächsten Alarm und zeige ihn an, falls der Clock-Modus aktiv ist
  findNextAlarm();
  if (clockmode) showNextAlarm();

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

    // Erstelle eine Nachricht mit dem Namen, der URL und dem Aktivierungsstatus der Station, getrennt durch neue Zeilen
    String msg = String(stationlist[i].name) + "\n" + String(stationlist[i].url) + "\n" + String(stationlist[i].enabled) + "\n" + String(i + 1);

    // Sende die Nachricht als Antwort
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
      Serial.printf("Move %i to position %i\n", i + 1, newpos + 1);
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
    ret = startUrl(server.arg("url"));
  }

  // Sendet eine Bestätigung bei Erfolg oder eine Fehlermeldung bei Misserfolg
  if (ret) {
    // Bei Erfolg: Antwort "OK" senden
    server.send(200, "text/plain", "OK");
  } else {
    // Bei Misserfolg: Wechselt zurück zur aktuellen Station und sendet "ERROR"
    startUrl(String(stationlist[actStation].url));
    server.send(300, "text/plain", "ERROR");
  }
}

// Verarbeitet den AJAX-Befehl /cmd/endtest, um den Test zu beenden
void endTest() {
  // Wechselt zurück zur aktuellen Station, um den Test zu beenden
  startUrl(String(stationlist[actStation].url));

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
    Serial.print("GainValue: ");
    Serial.println(floatWert);
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

// Verarbeitet den AJAX-Befehl /cmd/beforeStation, um zur vorherigen Station zu wechseln
void beforeStation() {
  // Verringert die aktuelle Station um eins
  curStation -= 1;
  // Wenn die aktuelle Station kleiner als 0 wird, setze sie auf die letzte Station
  if (curStation >= STATIONS) curStation = STATIONS - 1;
  // Ändert die Station und zeigt die neue Station an
  changeStation();
  // Debug-Ausgabe der aktuellen Station
  Serial.println(curStation);
  // Antwortet mit "OK"
  server.send(200, "text/plain", "OK");
}

// Verarbeitet den AJAX-Befehl /cmd/nextStation, um zur nächsten Station zu wechseln
void nextStation() {
  // Erhöht die aktuelle Station um eins
  curStation += 1;
  // Wenn die aktuelle Station den Maximalwert überschreitet, setze sie auf die erste Station
  if (curStation >= STATIONS) curStation = 0;
  // Ändert die Station und zeigt die neue Station an
  changeStation();
  // Debug-Ausgabe der aktuellen Station
  Serial.println(curStation);
  // Antwortet mit "OK"
  server.send(200, "text/plain", "OK");
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

  // Erstellen eines JSON-Dokuments für die Antwort
  StaticJsonDocument<200> jsonDoc;

  // Lautstärke des Radios
  jsonDoc["gain"] = curGain;

  // Status des Alarms
  if (alarmday < 8) {  // Alarm ist aktiv
    h = alarmtime / 60;
    m = alarmtime % 60;
    sprintf(txt, "%s %02i:%02i", days_short[alarmday], h, m);
    jsonDoc["alarm"] = "1";
  } else {  // Alarm ist ausgeschaltet
    sprintf(txt, "AUS");
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
    sprintf(tim, "%s %i. %s %i", days[ti.tm_wday], ti.tm_mday, months[ti.tm_mon], ti.tm_year + 1900);
    jsonDoc["Date"] = tim;
    strftime(tim, sizeof(tim), "%H:%M", &ti);
    jsonDoc["Time"] = tim;
  }

  // WLAN-Signalstärke
  jsonDoc["Rssi"] = WiFi.RSSI();

  // Status des Sleeptimers
  jsonDoc["Sleep"] = snoozeWait != 0 ? 1 : 0;

  // Konvertieren des JSON-Dokuments in einen String
  String response;
  serializeJson(jsonDoc, response);

  // Senden der JSON-Antwort an den Client
  server.send(200, "application/json", response);
}

//################ Info Tab
// AJAX-Befehl /cmd/getInfo
// Diese Funktion wird aufgerufen, um detaillierte Systeminformationen als JSON-Daten zurückzugeben.
// Die JSON-Antwort enthält:
// - Die installierte Version des Radios
// - Informationen zum aktuellen ESP8266/ESP32-Chip, einschließlich:
//   - Heap-Größe und freier Heap-Speicher
//   - Sketch-Größe und freier Sketch-Speicher
//   - Chip-Modell
void getInfo() {
  // Erstellen eines JSON-Dokuments für die Antwort
  StaticJsonDocument<300> jsonDoc;

  // Installierte Radio-Version
  jsonDoc["radioversion"] = RADIOVERSION;

  // Erstellen eines Unterobjekts für ESP-Board-Informationen
  JsonObject ESP_INFO = jsonDoc.createNestedObject("ESP_INFO");

  // HEAP-Informationen
  JsonObject HEAP = ESP_INFO.createNestedObject("HEAP");
  HEAP["getHeapSize"] = ESP.getHeapSize();  // Gesamte Heap-Größe
  HEAP["getFreeHeap"] = ESP.getFreeHeap();  // Freier Heap-Speicher

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