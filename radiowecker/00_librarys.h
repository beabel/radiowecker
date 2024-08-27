// Bibliotheken für WiFi und Over-the-Air (OTA) Updates
#include <WiFi.h>
#include <ArduinoOTA.h>

// TFT & Touchscreen Bibliotheken
#include <Adafruit_GFX.h>         // Basisbibliothek für Grafikanzeige
#include <Adafruit_ILI9341.h>     // Treiberbibliothek für das ILI9341 TFT-Display
#include <XPT2046_Touchscreen.h>  // Treiberbibliothek für das XPT2046 Touchscreen
#include <TouchEvent.h>           // Bibliothek zur Analyse von Touchscreen-Ereignissen

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