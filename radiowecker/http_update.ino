/* HTTP-OTA: GitHub-Release-Binary, ausgelöst per POST /cmd/startHttpUpdate (JSON {"tag":"v…"} im Body).
 * Route in webserver.ino ohne Upload-Handler registrieren, sonst bleibt POST-Body unter ESP32 WebServer 3.x leer.
 * Ablauf: Tag + Pending-Flag in NVS, sofort ESP.restart(); nächster Boot nach setup_display():
 * gleicher OTA-Screen (ota_ui_*, showProgress) wie zuvor, dann WLAN + TLS-Download.
 * setup_senderList usw. entfallen bis zum nächsten vollständigen Boot. */

#include <string.h>
#include <WiFiClientSecure.h>
#include <HTTPUpdate.h>
#include "ArduinoJson.h"
#include "i18n.h"

boolean initWiFi(const char *wifiSsid, const char *wifiPkey);
void setBGLight(uint8_t prct);
void ota_ui_begin(const char *title);
void ota_ui_set_sub(const char *sub);
void showProgress(uint32_t prc);

static bool http_ota_tag_ok(const char *t) {
  if (!t || !*t) return false;
  size_t n = strlen(t);
  if (n >= 40) return false;
  for (size_t i = 0; i < n; i++) {
    unsigned char c = (unsigned char)t[i];
    if (c <= 32 || c >= 127) return false;
  }
  return true;
}

/* Schlanker Boot: nur WiFi + GitHub-OTA. Keine Aufrufe von Display/Audio/Web. */
void httpOtaRunDeferredBoot(void) {
  char tag[40];
  if (pref.getString(PREF_HTTP_OTA_TAG, tag, sizeof(tag)) == 0) tag[0] = '\0';

  pref.putUChar(PREF_HTTP_OTA_PEND, 0);

  RADIO_SERIAL(Serial.println(F("HTTP-OTA: Boot-Modus (Display + WLAN + Download)")));
  RADIO_SERIAL(Serial.printf_P(PSTR("heap zu OTA-Start: %u\n"), (unsigned)ESP.getFreeHeap()));

  setBGLight(100);
  ota_ui_begin(i18n_str(I18N_OTA_HTTP_FW));
  ota_ui_set_sub(i18n_str(I18N_OTA_LOADING));
  showProgress(0);

  if (!http_ota_tag_ok(tag)) {
    RADIO_SERIAL(Serial.println(F("HTTP-OTA: ungültiger/leerer Tag in NVS — Neustart normal")));
    ota_ui_begin(i18n_str(I18N_OTA_ABORTED));
    ota_ui_set_sub(i18n_str(I18N_OTA_INV_TAG));
    delay(2500);
    ESP.restart();
    return;
  }

  RADIO_SERIAL(Serial.printf_P(PSTR("HTTP-OTA: Tag %s\n"), tag));

  ota_ui_set_sub(i18n_str(I18N_OTA_CONN_WLAN));
  if (!initWiFi(ssid, pkey)) {
    RADIO_SERIAL(Serial.println(F("HTTP-OTA: WLAN fehlgeschlagen — Neustart normal")));
    ota_ui_begin(i18n_str(I18N_OTA_FAILED));
    ota_ui_set_sub(i18n_str(I18N_OTA_NO_WLAN));
    delay(3000);
    ESP.restart();
    return;
  }

  ota_ui_set_sub(i18n_str(I18N_OTA_DL_FW));
  showProgress(0);

  char url[256];
  int urllen = snprintf(url, sizeof(url), "https://github.com/%s/%s/releases/download/%s/%s", HTTP_OTA_GITHUB_OWNER,
                        HTTP_OTA_GITHUB_REPO, tag, HTTP_OTA_FIRMWARE_FILENAME);
  if (urllen <= 0 || (size_t)urllen >= sizeof(url)) {
    RADIO_SERIAL(Serial.println(F("HTTP-OTA: URL-Puffer zu klein")));
    ota_ui_begin(i18n_str(I18N_OTA_FAILED));
    ota_ui_set_sub(i18n_str(I18N_OTA_URL_LONG));
    delay(3000);
    ESP.restart();
    return;
  }

  WiFiClientSecure client;
  client.setInsecure();
  client.setTimeout(300);

  HTTPUpdate httpOta(180000);
  httpOta.rebootOnUpdate(true);
  httpOta.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);

  httpOta.onProgress([](size_t cur, size_t total) {
    if (total == 0) return;
    uint32_t prc = (uint32_t)((100UL * (unsigned long)cur) / (unsigned long)total);
    if (prc > 100u) prc = 100u;
    showProgress(prc);
    static uint32_t s_last = 999;
    if (prc / 5u != s_last / 5u) {
      s_last = prc;
      RADIO_SERIAL(Serial.printf_P(PSTR("HTTP-OTA: %u%%\n"), (unsigned)prc));
    }
    yield();
  });

  RADIO_SERIAL(Serial.printf_P(PSTR("HTTP OTA: %s\n"), url));
  t_httpUpdate_return ret = httpOta.update(client, url);

  /* Bibliothek rebootet oft selbst; falls nicht, nach OK nachziehen */
  if (ret == HTTP_UPDATE_OK) {
    showProgress(100);
    ota_ui_set_sub(i18n_str(I18N_OTA_RESTART));
    delay(500);
    ESP.restart();
    return;
  }

  String detail = httpOta.getLastErrorString();
  RADIO_SERIAL(Serial.printf_P(PSTR("HTTP OTA Fehler ret=%d %s\n"), (int)ret, detail.c_str()));
  ota_ui_begin(i18n_str(I18N_OTA_FAILED));
  {
    char line[48];
    snprintf(line, sizeof(line), "%s %d", i18n_str(I18N_OTA_CODE), (int)ret);
    ota_ui_set_sub(line);
  }
  delay(400);
  if (detail.length() > 0 && detail.length() < 70) {
    delay(2200);
    ota_ui_set_sub(detail.c_str());
  }
  delay(3000);
  ESP.restart();
}

extern WebServer server;

void handleStartHttpUpdate(void) {
  if (server.method() != HTTP_POST) {
    server.send(405, "application/json; charset=utf-8", "{\"error\":\"method\"}");
    return;
  }
  if (!connected || WiFi.status() != WL_CONNECTED) {
    server.send(503, "application/json; charset=utf-8", "{\"error\":\"no_wifi\"}");
    return;
  }
  if (pref.getUChar(PREF_HTTP_OTA_PEND, 0) != 0) {
    server.send(409, "application/json; charset=utf-8", "{\"error\":\"busy\"}");
    return;
  }

  String plain = server.arg("plain");
  StaticJsonDocument<192> doc;
  DeserializationError e = deserializeJson(doc, plain);
  if (e) {
    server.send(400, "application/json; charset=utf-8", "{\"error\":\"json\"}");
    return;
  }
  const char *tag = doc["tag"];
  if (!tag || !http_ota_tag_ok(tag)) {
    server.send(400, "application/json; charset=utf-8", "{\"error\":\"tag\"}");
    return;
  }

  size_t freeSpc = ESP.getFreeSketchSpace();
  if (freeSpc < HTTP_OTA_MIN_FREE_BYTES) {
    server.send(500, "application/json; charset=utf-8", "{\"error\":\"no_space\"}");
    return;
  }

  pref.putString(PREF_HTTP_OTA_TAG, tag);
  pref.putUChar(PREF_HTTP_OTA_PEND, 1);

  server.send(200, "application/json; charset=utf-8", "{\"ok\":true}");
  delay(200);
  ESP.restart();
}
