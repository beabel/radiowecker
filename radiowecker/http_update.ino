/* HTTP-OTA: Binary aus GitHub-Release, ausgelöst per POST /cmd/startHttpUpdate (JSON {"tag":"v…"}).
 * Läuft im loop() nach HTTP-Antwort; Anzeige wie ArduinoOTA (scr_ota / showProgress). */

#include "ArduinoJson.h"

void showProgress(uint32_t prc);
void ota_ui_begin(const char *title);
void ota_ui_set_sub(const char *sub);
void setBGLight(uint8_t prct);
void stopPlaying(void);

extern WebServer server;

volatile bool g_httpOtaPending = false;
volatile bool g_httpOtaBusy = false;
char g_httpOtaTagBuf[40];

static bool http_ota_tag_ok(const char *t) {
  if (!t || !*t) return false;
  size_t n = strlen(t);
  if (n >= sizeof(g_httpOtaTagBuf)) return false;
  for (size_t i = 0; i < n; i++) {
    unsigned char c = (unsigned char)t[i];
    if (c <= 32 || c >= 127) return false;
  }
  return true;
}

static void http_ota_quiet_stop(void) {
  stopPlaying();
  snoozeTimeEnd = 0;
  radio = false;
}

void httpOtaTryConsume(void) {
  if (!g_httpOtaPending) return;
  g_httpOtaPending = false;
  g_httpOtaBusy = true;

  char tag[sizeof(g_httpOtaTagBuf)];
  memcpy(tag, g_httpOtaTagBuf, sizeof(tag));

  http_ota_quiet_stop();
  delay(80);
  yield();

  setBGLight(100);
  ota_ui_begin("HTTP Firmware-Update");
  ota_ui_set_sub("Lade …");
  showProgress(0);

  String url = String("https://github.com/") + HTTP_OTA_GITHUB_OWNER + "/" + HTTP_OTA_GITHUB_REPO + "/releases/download/" + tag + "/" +
               HTTP_OTA_FIRMWARE_FILENAME;

  WiFiClientSecure client;
  client.setInsecure();
  client.setTimeout(300);

  /* Eigene Instanz: längeres HTTP-Timeout; Redirects nötig (GitHub → CDN) */
  HTTPUpdate httpOta(180000);
  httpOta.rebootOnUpdate(true);
  httpOta.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);

  httpOta.onProgress([](size_t cur, size_t total) {
    if (total == 0) return;
    uint32_t prc = (uint32_t)((100UL * (unsigned long)cur) / (unsigned long)total);
    if (prc > 100u) prc = 100u;
    showProgress(prc);
    yield();
  });

  Serial.printf_P(PSTR("HTTP OTA: %s\n"), url.c_str());
  t_httpUpdate_return ret = httpOta.update(client, url);

  /* Bei Erfolg ruft die Bibliothek üblicherweise ESP.restart() auf — kein Return */
  g_httpOtaBusy = false;
  if (ret == HTTP_UPDATE_OK) {
    ota_ui_set_sub("Neustart …");
    delay(500);
    ESP.restart();
    return;
  }

  String detail = httpOta.getLastErrorString();
  Serial.printf_P(PSTR("HTTP OTA Fehler ret=%d %s\n"), (int)ret, detail.c_str());
  ota_ui_begin("Update fehlgeschlagen");
  char line[100];
  snprintf(line, sizeof(line), "Code %d", (int)ret);
  ota_ui_set_sub(line);
  delay(50);
  if (detail.length() > 0 && detail.length() < 70) {
    delay(2500);
    ota_ui_set_sub(detail.c_str());
  }
}

void handleStartHttpUpdate(void) {
  if (server.method() != HTTP_POST) {
    server.send(405, "application/json; charset=utf-8", "{\"error\":\"method\"}");
    return;
  }
  if (!connected || WiFi.status() != WL_CONNECTED) {
    server.send(503, "application/json; charset=utf-8", "{\"error\":\"no_wifi\"}");
    return;
  }
  if (g_httpOtaPending || g_httpOtaBusy) {
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
  strlcpy(g_httpOtaTagBuf, tag, sizeof(g_httpOtaTagBuf));

  size_t freeSpc = ESP.getFreeSketchSpace();
  if (freeSpc < HTTP_OTA_MIN_FREE_BYTES) {
    server.send(500, "application/json; charset=utf-8", "{\"error\":\"no_space\"}");
    return;
  }

  g_httpOtaPending = true;
  server.send(200, "application/json; charset=utf-8", "{\"ok\":true}");
}
