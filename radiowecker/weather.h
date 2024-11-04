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

// Funktion zur Anzeige der Wetterdaten (zunächst auf Serial, später TFT)
void displayWeather(String weatherData) {
  if (weatherData == "") {
    Serial.println("Keine Wetterdaten empfangen.");
    return;
  }

  // JSON-Daten parsen
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, weatherData);

  if (error) {
    Serial.print("Fehler beim Parsen von JSON: ");
    Serial.println(error.c_str());
    return;
  }

  // Temperaturen und Wettercode für heute, morgen und übermorgen extrahieren
  float temp_min_today = doc["daily"]["temperature_2m_min"][0];
  float temp_max_today = doc["daily"]["temperature_2m_max"][0];
  int weather_code_today = doc["daily"]["weathercode"][0];

  float temp_min_tomorrow = doc["daily"]["temperature_2m_min"][1];
  float temp_max_tomorrow = doc["daily"]["temperature_2m_max"][1];
  int weather_code_tomorrow = doc["daily"]["weathercode"][1];

  float temp_min_day_after = doc["daily"]["temperature_2m_min"][2];
  float temp_max_day_after = doc["daily"]["temperature_2m_max"][2];
  int weather_code_day_after = doc["daily"]["weathercode"][2];

  // Temperaturen und Wettercode aktual Werte extrahieren
  float temp_current_temp = doc["current"]["temperature_2m"];
  float temp_feels_like_temp = doc["current"]["apparent_temperature"];
  int temp_current_weather_code = doc["current"]["weather_code"];

  // Ausgabe auf die serielle Schnittstelle
  Serial.println("aktuelle Wetterdaten:");
  Serial.print("aktuelle Temperatur: ");
  Serial.println(temp_current_temp);
  Serial.print("gefühlte Temperatur: ");
  Serial.println(temp_feels_like_temp);
  Serial.print("aktuelle Wettercode: ");
  Serial.println(temp_current_weather_code);

  Serial.println("Wetterdaten für heute:");
  Serial.print("Minimale Temperatur: ");
  Serial.println(temp_min_today);
  Serial.print("Maximale Temperatur: ");
  Serial.println(temp_max_today);
  Serial.print("Wettercode: ");
  Serial.println(weather_code_today);

  Serial.println("\nWetterdaten für morgen:");
  Serial.print("Minimale Temperatur: ");
  Serial.println(temp_min_tomorrow);
  Serial.print("Maximale Temperatur: ");
  Serial.println(temp_max_tomorrow);
  Serial.print("Wettercode: ");
  Serial.println(weather_code_tomorrow);

  Serial.println("\nWetterdaten für übermorgen:");
  Serial.print("Minimale Temperatur: ");
  Serial.println(temp_min_day_after);
  Serial.print("Maximale Temperatur: ");
  Serial.println(temp_max_day_after);
  Serial.print("Wettercode: ");
  Serial.println(weather_code_day_after);

  // Hier später die Daten auf dem TFT anzeigen
}