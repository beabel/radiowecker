// Bibliotheken für WiFi und Over-the-Air (OTA) Updates
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPUpdate.h>
#include <ArduinoOTA.h>

// TFT, Touch & LVGL 9.x — lv_conf.h zuerst laden (setzt LV_CONF_H), dann lvgl.h
#define LV_CONF_INCLUDE_SIMPLE
#include "lv_conf.h"
#include <lvgl.h>
#include <Adafruit_ILI9341.h>
#include <XPT2046_Touchscreen.h>

// ESP32 Bibliothek zum Speichern von Einstellungen im Flash-Speicher
#include <Preferences.h>

// Bibliothek für den eingebauten Webserver
#include <WebServer.h>

// Bibliotheken für Audio-Funktionen
// Enthält Klassen für die Audiowiedergabe und -verarbeitung
#include <AudioFileSourceICYStream.h>  // Klasse für den Eingabestream von ICY (Internet Radio) Streams
#include <AudioFileSourceBuffer.h>     // Klasse für das Puffern von Audio-Daten
#include <AudioGeneratorMP3.h>         // MP3-Decoder Klasse
#include <AudioOutputI2S.h>            // Klasse für die Audioausgabe über I2S (Inter-IC Sound) Schnittstelle