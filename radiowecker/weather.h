#include "ArduinoJson.h"

// Funktion zur Abfrage der Wetterdaten
String getWeatherData(float latitude, float longitude, String timezone) {
  WiFiClient client;
  const char* host = "api.open-meteo.com";
  const int httpPort = 80;

  String url = "/v1/forecast?latitude=" + String(latitude, 6) + 
               "&longitude=" + String(longitude, 6) +
               "&current=temperature_2m,apparent_temperature,weather_code" + 
               "&daily=temperature_2m_min,temperature_2m_max,weathercode&timezone=" + timezone;

  String fullUrl = "http://" + String(host) + url;
  Serial.println("Überprüfe die folgende URL im Browser:");
  Serial.println(fullUrl);

  if (!client.connect(host, httpPort)) {
    Serial.println("Verbindung zum Server fehlgeschlagen");
    return "";
  }

  Serial.println("Verbindung zum Server hergestellt.");

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");

  Serial.println("HTTP-Request gesendet.");

  String jsonResponse;
  bool isBody = false; // Flag, um anzuzeigen, ob wir im Response-Body sind

  // Warte auf die Antwort vom Server
  unsigned long timeout = millis() + 5000; // 5 Sekunden Timeout
  while (!client.available()) {
    if (millis() > timeout) {
      Serial.println("Timeout beim Warten auf die Antwort vom Server.");
      return "";
    }
    delay(50);
  }

  Serial.println("Antwort vom Server verfügbar.");

  // Header und Body verarbeiten
  while (client.available()) {
    char c = client.read();

    // Erkenne den Beginn des Response-Bodys (Überspringe die Header)
    if (c == '{' || c == '[') {
      isBody = true; // Wir befinden uns jetzt im Body
    }

    // Füge das Zeichen zur JSON-Antwort hinzu, wenn wir im Body sind
    if (isBody) {
      jsonResponse += c;
    }
  }

  if (jsonResponse.length() > 0) {
    Serial.println("Wetterdaten empfangen.");
    Serial.println(jsonResponse); // Gibt die empfangene JSON-Antwort aus
  } else {
    Serial.println("Keine Wetterdaten empfangen.");
  }

  return jsonResponse;  // JSON-Daten als String zurückgeben
}

// Funktion zur Rückgabe des passenden Icon-Indexes basierend auf dem Wettercode
int getWeatherIcon(int weather_code) {
    // Überprüfe die Wettercodes und gebe den entsprechenden Icon-Index zurück
    if (weather_code == 0) return 0;
    else if (weather_code == 45 || weather_code == 48) return 1;
    else if (weather_code == 1) return 2;
    else if (weather_code == 2) return 3;
    else if (weather_code == 3) return 4;
    else if (weather_code == 51 || weather_code == 53 || weather_code == 55 || weather_code == 80) return 5;
    else if (weather_code == 56 || weather_code == 57) return 6;
    else if (weather_code == 66 || weather_code == 67) return 7;
    else if (weather_code == 81 || weather_code == 82 || weather_code == 61 || weather_code == 63 || weather_code == 65) return 8;
    else if (weather_code == 95) return 9;
    else if (weather_code == 77 || weather_code == 85 || weather_code == 86 || weather_code == 71 || weather_code == 73 || weather_code == 75) return 10;
    else if (weather_code == 96 || weather_code == 99) return 11;
    else return -1; // Falls der Wettercode ungültig ist, -1 zurückgeben
}