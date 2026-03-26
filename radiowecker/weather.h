#ifndef RADIOWECKER_WEATHER_H
#define RADIOWECKER_WEATHER_H

#include <HTTPClient.h>
#include <string.h>
#include "ArduinoJson.h"

#ifndef RADIO_SERIAL
#ifndef RADIO_DEBUG_SERIAL
#define RADIO_DEBUG_SERIAL 1
#endif
#if RADIO_DEBUG_SERIAL
#define RADIO_SERIAL(x) \
  do {                    \
    x;                    \
  } while (0)
#else
#define RADIO_SERIAL(x) ((void)0)
#endif
#endif

/**
 * Nur die sechs Werte, die die UI aus Open-Meteo braucht (vgl. API-Antwort).
 * Kein großer JSON-Puffer in .bss — Parsing mit Filter → kleines JsonDocument.
 * Serial-Diagnose: RADIO_DEBUG_SERIAL in 00_settings.h (alle Wetter-Logs nutzen RADIO_SERIAL).
 */
struct OpenMeteoTemps {
  float current_c;
  float feels_c;
  float today_min, today_max;
  float tomorrow_min, tomorrow_max;
};

/* Implementierung in tft_display.ino */
void displayWeather(const OpenMeteoTemps *temps);

/* IANA-Zeitzone für die Query (z. B. Europe/Berlin): '/' → %2F */
inline size_t weather_encode_timezone(const char *tz, char *out, size_t outCap) {
  if (!out || outCap < 2) return 0;
  if (!tz || !*tz) tz = "Europe/Berlin";
  size_t o = 0;
  for (const unsigned char *p = (const unsigned char *)tz; *p && o + 4 < outCap; ++p) {
    if (*p == '/') {
      out[o++] = '%';
      out[o++] = '2';
      out[o++] = 'F';
    } else if (*p == ' ') {
      out[o++] = '%';
      out[o++] = '2';
      out[o++] = '0';
    } else
      out[o++] = (char)*p;
  }
  out[o] = '\0';
  return o;
}

/**
 * HTTP GET + Parse. Open-Meteo liefert trotz schlanker Query noch Metadaten
 * (latitude, units, …); mit DeserializationOption::Filter bleiben nur die genutzten Felder
 * im JsonDocument (siehe https://api.open-meteo.com/v1/forecast …).
 *
 * Filter: Bei Arrays reiner Zahlen muss pro Index true stehen — nicht nur der Key,
 * sonst verwirft der Filter alle Elemente (früherer Bug mit lauter 0 °C).
 */
inline bool fetchOpenMeteoWeather(OpenMeteoTemps *out, float latitude, float longitude, const char *timezone) {
  if (!out) return false;

  char tzenc[80];
  weather_encode_timezone(timezone, tzenc, sizeof(tzenc));

  char url[288];
  int n = snprintf(
      url, sizeof(url),
      "http://api.open-meteo.com/v1/forecast?latitude=%.6f&longitude=%.6f"
      "&current=temperature_2m,apparent_temperature"
      "&daily=temperature_2m_min,temperature_2m_max"
      "&forecast_days=2"
      "&timezone=%s",
      (double)latitude, (double)longitude, tzenc);
  if (n <= 0 || (size_t)n >= sizeof(url)) return false;

  RADIO_SERIAL({
    Serial.println(F("Wetter-URL (Browser):"));
    Serial.println(url);
  });

  HTTPClient http;
  http.setTimeout(20000);
  http.setReuse(false);
  if (!http.begin(url)) {
    RADIO_SERIAL(Serial.println(F("Wetter: http.begin fehlgeschlagen")));
    return false;
  }

  http.addHeader("Accept-Encoding", "identity");

  int code = http.GET();
  if (code != HTTP_CODE_OK) {
    RADIO_SERIAL(Serial.printf_P(PSTR("Wetter: HTTP-Code %d\n"), code));
    http.end();
    return false;
  }

  String body = http.getString();
  http.end();

  if (body.length() == 0) {
    RADIO_SERIAL(Serial.println(F("Keine Wetterdaten empfangen.")));
    return false;
  }

  if (body.length() >= 2 && (unsigned char)body[0] == 0x1f && (unsigned char)body[1] == 0x8b) {
    RADIO_SERIAL(Serial.println(F("Wetter: Antwort gzip-komprimiert.")));
    return false;
  }

  JsonDocument filterSpec;
  filterSpec["current"]["temperature_2m"] = true;
  filterSpec["current"]["apparent_temperature"] = true;
  filterSpec["daily"]["temperature_2m_min"][0] = true;
  filterSpec["daily"]["temperature_2m_min"][1] = true;
  filterSpec["daily"]["temperature_2m_max"][0] = true;
  filterSpec["daily"]["temperature_2m_max"][1] = true;

  JsonDocument doc;
  DeserializationError err = deserializeJson(doc, body, DeserializationOption::Filter(filterSpec));
  if (err) {
    RADIO_SERIAL({
      Serial.print(F("Fehler beim Parsen von JSON: "));
      Serial.println(err.c_str());
      Serial.print(F("Wetter JSON-Anfang (max. 160 Zeichen): "));
      for (size_t i = 0; i < 160 && (size_t)i < (size_t)body.length(); i++) {
        char c = body[i];
        if (c >= 32 && c < 127)
          Serial.print(c);
        else
          Serial.printf(" \\x%02X", (unsigned char)c);
      }
      Serial.println();
    });
    return false;
  }
  if (doc.overflowed()) {
    RADIO_SERIAL(Serial.println(F("Wetter: JsonDocument overflow nach Filter.")));
    return false;
  }

  out->current_c = doc["current"]["temperature_2m"].as<float>();
  out->feels_c = doc["current"]["apparent_temperature"].as<float>();
  out->today_min = doc["daily"]["temperature_2m_min"][0].as<float>();
  out->today_max = doc["daily"]["temperature_2m_max"][0].as<float>();
  out->tomorrow_min = doc["daily"]["temperature_2m_min"][1].as<float>();
  out->tomorrow_max = doc["daily"]["temperature_2m_max"][1].as<float>();

  RADIO_SERIAL(Serial.printf_P(PSTR("Wetter: %u B empfangen, 6 Werte übernommen\n"), (unsigned)body.length()));
  return true;
}

#endif
