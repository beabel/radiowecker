// Variable zur Verfolgung des Fortschritts der OTA-Aktualisierung
uint32_t oldprc;

// Funktion zur Vorbereitung der OTA-Aktualisierung
void setup_ota() {
  // Setze den Hostnamen und das Passwort für OTA-Updates
  ArduinoOTA.setHostname(OTA_HOSTNAME);  // Setzt den Hostnamen für OTA
  ArduinoOTA.setPassword(OTA_PASSWORD);  // Setzt das Passwort für OTA

  // Registriere Callback-Routinen für OTA-Ereignisse
  ArduinoOTA.onStart(ota_onStart);        // Callback, wenn die OTA-Aktualisierung beginnt
  ArduinoOTA.onEnd(ota_onEnd);            // Callback, wenn die OTA-Aktualisierung endet
  ArduinoOTA.onProgress(ota_onProgress);  // Callback zur Anzeige des Fortschritts
  ArduinoOTA.onError(ota_onError);        // Callback bei einem Fehler während der OTA-Aktualisierung

  // Starte die OTA-Verarbeitung
  ArduinoOTA.begin();  // Beginnt die OTA-Aktualisierung
}

// Callback-Funktion, die aufgerufen wird, wenn die OTA-Aktualisierung beginnt
void ota_onStart() {
  displayClear();   // Löscht das Display
  setBGLight(100);  // Setzt die Hintergrundbeleuchtung auf 100% Helligkeit
  oldprc = 0;       // Setzt den alten Fortschrittswert zurück
  String type;
  // Bestimme den Typ der Aktualisierung (Sketch oder Dateisystem)
  if (ArduinoOTA.getCommand() == U_FLASH)
    type = "sketch";      // Programm-Aktualisierung
  else                    // U_SPIFFS
    type = "filesystem";  // Dateisystem-Aktualisierung

  // Zeige eine Nachricht auf dem Display an, dass das Sketch aktualisiert wird
  displayMessage(5, 10, 310, 30, "Updating Sketch", ALIGNCENTER, true, ILI9341_YELLOW, ILI9341_BLACK, 1);
  showProgress(0);  // Zeigt den Fortschritt (beginnend bei 0%)
}

// Callback-Funktion, die aufgerufen wird, wenn die OTA-Aktualisierung endet
void ota_onEnd() {
  // Zeigt eine Nachricht auf dem Display an, dass die Aktualisierung abgeschlossen ist
  displayMessage(5, 120, 310, 30, "Done", ALIGNLEFT, true, ILI9341_GREEN, ILI9341_BLACK, 1);
}

// Callback-Funktion, die aufgerufen wird, um den Fortschritt der OTA-Aktualisierung anzuzeigen
void ota_onProgress(unsigned int progress, unsigned int total) {
  uint32_t prc = progress / (total / 100);  // Berechne den Fortschritt in Prozent
  char tmp[20];
  // Wenn der Fortschritt gestiegen ist, aktualisiere die Fortschrittsanzeige
  if (prc > oldprc) {
    showProgress(prc);  // Zeige den Fortschritt an
    oldprc = prc;       // Speichere den aktuellen Fortschritt für die nächste Überprüfung
  }
}

// Callback-Funktion, die bei einem Fehler während der OTA-Aktualisierung aufgerufen wird
void ota_onError(ota_error_t error) {
  char err[80];
  // Formatierte Fehlermeldung
  sprintf(err, "Error[%u]: ", error);
  // Bestimme die spezifische Fehlermeldung basierend auf dem Fehlercode
  if (error == OTA_AUTH_ERROR) sprintf(err, "Error[%u]: Auth Failed", error);
  else if (error == OTA_BEGIN_ERROR) sprintf(err, "Error[%u] Begin Failed", error);
  else if (error == OTA_CONNECT_ERROR) sprintf(err, "Error[%u] Connect Failed", error);
  else if (error == OTA_RECEIVE_ERROR) sprintf(err, "Error[%u] Receive Failed", error);
  else if (error == OTA_END_ERROR) sprintf(err, "Error[%u] End Failed", error);

  // Zeige die Fehlermeldung auf dem Display an
  displayMessage(5, 200, 310, 30, err, ALIGNLEFT, false, ILI9341_RED);
}

// Funktion zum Vergleichen von semantischen Versionen
int compareVersions(const String& v1, const String& v2) {
  // Entferne das führende "v" und teile die Versionsnummern in ihre Bestandteile
  int v1Parts[3] = {0, 0, 0};  // Arrays zur Speicherung der Haupt-, Neben- und Patch-Versionen
  int v2Parts[3] = {0, 0, 0};

  // Konvertiere die Versionsstrings in Arrays von Ganzzahlen
  sscanf(v1.c_str(), "v%d.%d.%d", &v1Parts[0], &v1Parts[1], &v1Parts[2]);
  sscanf(v2.c_str(), "v%d.%d.%d", &v2Parts[0], &v2Parts[1], &v2Parts[2]);

  // Vergleiche die einzelnen Teile der Versionsnummern
  for (int i = 0; i < 3; i++) {
    if (v1Parts[i] > v2Parts[i]) return 1;  // Erste Version ist größer
    if (v1Parts[i] < v2Parts[i]) return -1; // Zweite Version ist größer
  }
  return 0; // Beide Versionen sind gleich
}

// Funktion zum Parsen der GitHub API und Extrahieren der neuesten Version
String getLatestVersionAndBinURL(String &binURL) {
  HTTPClient http;
  http.begin(GITHUB);
  int httpCode = http.GET();
  
  String latestVersion = "";
  
  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();
    StaticJsonDocument<1024> doc;
    deserializeJson(doc, payload);
    
    latestVersion = doc["tag_name"].as<String>();  // Die neueste Version
    JsonArray assets = doc["assets"];
    for (JsonObject asset : assets) {
      String assetName = asset["name"].as<String>();
      if (assetName.endsWith(".bin")) {
        binURL = asset["browser_download_url"].as<String>();  // Die URL zur .bin-Datei
        break;
      }
    }
  } else {
    Serial.printf("HTTP-Error: %d\n", httpCode);
  }
  
  http.end();
  return latestVersion;
}

void downloadAndUpdate(const char* url) {
  HTTPClient http;

  ota_onStart();  // Ruft die bestehende Funktion auf, wenn das Update startet

  http.begin(url);
  int httpCode = http.GET();  // Anfrage senden

  if (httpCode == HTTP_CODE_OK) {
    int contentLength = http.getSize();

    if (contentLength > 0 && Update.begin(contentLength)) {
      WiFiClient* stream = http.getStreamPtr();
      size_t written = 0;

      while (written < contentLength) {
        size_t availableBytes = stream->available();
        if (availableBytes > 0) {
          uint8_t buffer[128];
          size_t readBytes = stream->readBytes(buffer, availableBytes > sizeof(buffer) ? sizeof(buffer) : availableBytes);
          size_t writtenBytes = Update.write(buffer, readBytes);
          written += writtenBytes;

          ota_onProgress(written, contentLength);  // Ruft die bestehende Funktion auf, um den Fortschritt anzuzeigen
        }
      }

      if (Update.end(true)) {
        ota_onEnd();  // Ruft die bestehende Funktion auf, wenn das Update endet
        ESP.restart();  // Neustart nach erfolgreichem Update
      } else {
        ota_onError((ota_error_t)Update.getError());  // Ruft die bestehende Fehlerfunktion auf
      }
    } else {
      ota_onError(OTA_BEGIN_ERROR);  // Ruft die bestehende Fehlerfunktion auf, wenn das Update nicht beginnt
    }
  } else {
    ota_onError(OTA_CONNECT_ERROR);  // Ruft die bestehende Fehlerfunktion auf, wenn die HTTP-Verbindung fehlschlägt
  }

  http.end();
}

void handleGithubUpdate(){
  String latestVersion = getLatestVersionAndBinURL(GITHUB);

  Serial.println("Aktuelle Version: " RADIOVERSION);
  Serial.println("Neueste Version: " + latestVersion);


  if (latestVersion != "" && compareVersions(RADIOVERSION, latestVersion) < 0) {
    Serial.println("Neue Version verfügbar! Starte Update...");
    // Schritt 1: "v" entfernen
    latestVersion.replace("v", "");  // Entfernt das "v" aus der Versionsnummer
    // Schritt 2: Bereinigte Versionsnummer in die URL einfügen
    String nextbinURL = "http://ne-xt.de/esp_flasher/firmware/x.x.x/radiowecker.ino.bin";
    nextbinURL.replace("x.x.x", latestVersion);  // Ersetzt x.x.x durch die bereinigte Versionsnummer
    Serial.println("URL zur .bin-Datei: " + nextbinURL);    
    // Schritt 3: Überprüfen, ob die Datei existiert
    HTTPClient http;
    http.begin(nextbinURL);
    int httpCode = http.sendRequest("HEAD");  // Nur Header überprüfen

    if (httpCode == HTTP_CODE_OK) {
      Serial.println("Datei existiert: " + nextbinURL);
      
      // Wenn die Datei existiert, kann hier der Download und das Update stattfinden.
      downloadAndUpdate(nextbinURL.c_str());
    } else {
      Serial.println("Datei nicht gefunden oder Verbindung fehlgeschlagen: " + String(httpCode));
    }    

  } else {
    Serial.println("Keine neuere Version verfügbar.");
  }  
}