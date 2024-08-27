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