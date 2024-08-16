// Initialisiert das WiFi-Modul mit den angegebenen SSID und Passkey
boolean initWiFi(String ssid, String pkey) {
  boolean connected = false;  // Variable zur Verfolgung des Verbindungsstatus

  // Zuerst trennen alle bestehenden Verbindungen
  Serial.print(TXT_DISCONNECT);          // Debug-Ausgabe, dass die Verbindung getrennt wird
  WiFi.disconnect();                     // Trennt die aktuelle WiFi-Verbindung
  Serial.print(TXT_SOFT_AP_DISCONNECT);  // Debug-Ausgabe, dass der Soft-AP-Modus getrennt wird
  WiFi.softAPdisconnect(true);           // Trennt den Soft-AP-Modus, falls aktiv

  // Setzt den WiFi-Modus auf Station (Client) und konfiguriert den Hostnamen
  Serial.print(TXT_SET_WLAN_MODE);  // Debug-Ausgabe, dass der WLAN-Modus gesetzt wird
  WiFi.setHostname(NETWORK_NAME);   // Setzt den Hostnamen für das Gerät
  WiFi.mode(WIFI_STA);              // Setzt den WiFi-Modus auf Station (Client)

  Serial.print(TXT_CONNECTING_TO);           // Debug-Ausgabe, dass eine Verbindung zu einer SSID aufgebaut wird
  Serial.print(ssid);                        // Gibt die SSID auf der Konsole aus
  Serial.println(TXT_CONNECTING_ESTABLISH);  // Debug-Ausgabe, dass die Verbindung aufgebaut wird
  Serial.println(pkey);                      // Gibt den Passkey auf der Konsole aus (Hinweis: Passkey nicht sicher in der Konsole ausgeben)

  // Wenn eine SSID angegeben wurde, versuche eine Verbindung herzustellen
  if (ssid != "") {
    WiFi.begin(ssid.c_str(), pkey.c_str());  // Beginnt den Verbindungsversuch zur angegebenen SSID mit dem Passkey
    uint8_t cnt = 0;                         // Zähler für die Anzahl der Versuche
    // Versuche bis zu 20 Mal, sich zu verbinden
    while ((WiFi.status() != WL_CONNECTED) && (cnt < 20)) {
      delay(500);         // Warte 500 Millisekunden
      Serial.print(".");  // Zeigt Fortschritt auf der Konsole an
      cnt++;              // Erhöhe den Zähler
    }
    Serial.println();  // Neue Zeile für saubere Konsolenausgabe
    // Wenn die Verbindung erfolgreich hergestellt wurde
    if (WiFi.status() == WL_CONNECTED) {
      Serial.print(TXT_IP_ADDRESS);    // Debug-Ausgabe der IP-Adresse
      Serial.println(WiFi.localIP());  // Gibt die lokale IP-Adresse auf der Konsole aus
      connected = true;                // Setzt den Verbindungsstatus auf true
    }
  }

  // Wenn keine Verbindung hergestellt werden konnte
  if (!connected) {
    Serial.println(TXT_NO_CONNECTION);  // Debug-Ausgabe, dass keine Verbindung hergestellt werden konnte
    WiFi.mode(WIFI_AP);                 // Setzt den WiFi-Modus auf Access Point (AP)
    WiFi.softAP(AP_NAME, "", 1);        // Startet den Access Point mit dem angegebenen Namen und ohne Passwort
  }

  return connected;  // Gibt den Verbindungsstatus zurück (true, wenn verbunden, sonst false)
}