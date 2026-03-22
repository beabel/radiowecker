#include "ArduinoJson.h"
#include <HTTPClient.h>

// Funktion zur Abfrage der Wetterdaten (HTTPClient: zuverlässiger als roher WiFiClient bei knappem Heap)
String getWeatherData(float latitude, float longitude, String timezone) {
  String path = "/v1/forecast?latitude=" + String(latitude, 6) +
                "&longitude=" + String(longitude, 6) +
                "&current=temperature_2m,apparent_temperature,weather_code" +
                "&daily=temperature_2m_min,temperature_2m_max,weathercode&timezone=" + timezone;

  String fullUrl = "http://api.open-meteo.com" + path;
  Serial.println(F("Wetter-URL (wie im Browser):"));
  Serial.println(fullUrl);

  HTTPClient http;
  http.setTimeout(20000);
  http.setReuse(false);
  if (!http.begin(fullUrl)) {
    Serial.println(F("Wetter: http.begin fehlgeschlagen"));
    return "";
  }

  int code = http.GET();
  if (code != HTTP_CODE_OK) {
    Serial.printf_P(PSTR("Wetter: HTTP-Code %d\n"), code);
    http.end();
    return "";
  }

  String jsonResponse = http.getString();
  http.end();

  if (jsonResponse.length() > 0) {
    Serial.println(F("Wetterdaten empfangen."));
    Serial.println(jsonResponse);
  } else {
    Serial.println(F("Keine Wetterdaten empfangen."));
  }

  return jsonResponse;
}
