#include "i18n.h"

// Initialisiert das WiFi-Modul mit den angegebenen SSID und Passkey (C-Strings)
boolean initWiFi(const char *wifiSsid, const char *wifiPkey) {
  boolean connected = false;  // Variable zur Verfolgung des Verbindungsstatus

  // Zuerst trennen alle bestehenden Verbindungen
  RADIO_SERIAL(Serial.print(i18n_str(I18N_DISCONNECT)));          // Debug-Ausgabe, dass die Verbindung getrennt wird
  WiFi.disconnect();                     // Trennt die aktuelle WiFi-Verbindung
  RADIO_SERIAL(Serial.print(i18n_str(I18N_SOFT_AP_DISCONNECT)));  // Debug-Ausgabe, dass der Soft-AP-Modus getrennt wird
  WiFi.softAPdisconnect(true);           // Trennt den Soft-AP-Modus, falls aktiv

  // Setzt den WiFi-Modus auf Station (Client) und konfiguriert den Hostnamen
  RADIO_SERIAL(Serial.print(i18n_str(I18N_SET_WLAN_MODE)));  // Debug-Ausgabe, dass der WLAN-Modus gesetzt wird
  WiFi.setHostname(NETWORK_NAME);   // Setzt den Hostnamen für das Gerät
  WiFi.mode(WIFI_STA);              // Setzt den WiFi-Modus auf Station (Client)

  RADIO_SERIAL({
    Serial.print(i18n_str(I18N_CONNECTING_TO));           // Debug-Ausgabe, dass eine Verbindung zu einer SSID aufgebaut wird
    Serial.print(wifiSsid);                    // Gibt die SSID auf der Konsole aus
    Serial.println(i18n_str(I18N_CONNECTING_ESTABLISH));  // Debug-Ausgabe, dass die Verbindung aufgebaut wird
    Serial.println(wifiPkey);                  // Gibt den Passkey auf der Konsole aus (Hinweis: Passkey nicht sicher in der Konsole ausgeben)
  });

  // Wenn eine SSID angegeben wurde, versuche eine Verbindung herzustellen
  if (wifiSsid && wifiSsid[0] != '\0') {
    WiFi.begin(wifiSsid, wifiPkey ? wifiPkey : "");
    uint8_t cnt = 0;                         // Zähler für die Anzahl der Versuche
    // Versuche bis zu 20 Mal, sich zu verbinden
    while ((WiFi.status() != WL_CONNECTED) && (cnt < 20)) {
      delay(500);         // Warte 500 Millisekunden
      RADIO_SERIAL(Serial.print("."));  // Zeigt Fortschritt auf der Konsole an
      cnt++;              // Erhöhe den Zähler
    }
    RADIO_SERIAL(Serial.println());  // Neue Zeile für saubere Konsolenausgabe
    // Wenn die Verbindung erfolgreich hergestellt wurde
    if (WiFi.status() == WL_CONNECTED) {
      RADIO_SERIAL(Serial.print(i18n_str(I18N_IP_ADDRESS)));    // Debug-Ausgabe der IP-Adresse
      RADIO_SERIAL(Serial.println(WiFi.localIP()));  // Gibt die lokale IP-Adresse auf der Konsole aus
      connected = true;                // Setzt den Verbindungsstatus auf true
      // WiFi-Stromsparen kann I2S-/Stream-Timing stören (stille oder ruckelige Wiedergabe).
      WiFi.setSleep(false);
    }
  }

  // Wenn keine Verbindung hergestellt werden konnte
  if (!connected) {
    RADIO_SERIAL(Serial.println(i18n_str(I18N_NO_CONNECTION)));  // Debug-Ausgabe, dass keine Verbindung hergestellt werden konnte
    WiFi.mode(WIFI_AP);                 // Setzt den WiFi-Modus auf Access Point (AP)
    WiFi.softAP(AP_NAME, "", 1);        // Startet den Access Point mit dem angegebenen Namen und ohne Passwort
  }

  return connected;  // Gibt den Verbindungsstatus zurück (true, wenn verbunden, sonst false)
}