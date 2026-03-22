void ota_ui_begin(const char *title);
void ota_ui_set_sub(const char *sub);

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
  displayClear();
  setBGLight(100);
  oldprc = 0;
  ota_ui_begin("Updating Sketch");
  showProgress(0);
}

// Callback-Funktion, die aufgerufen wird, wenn die OTA-Aktualisierung endet
void ota_onEnd() {
  ota_ui_set_sub("Done");
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

  ota_ui_begin("OTA Fehler");
  ota_ui_set_sub(err);
}