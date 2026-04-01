// Instanzen der Preferences-Klasse für die Speicherung von Einstellungen
Preferences pref;    // Allgemeine Instanz für die Nutzung der Preferences
Preferences sender;  // Zusätzliche Instanz, möglicherweise für spezielle Konfigurationen

// Struktur zur Speicherung der Stationsdaten
typedef struct {
  char url[150];    // URL des Streams der Station
  char name[32];    // Name der Station
  uint8_t enabled;  // Flag zur Aktivierung der Station (1 = aktiv, 0 = inaktiv)
} Station;

// Konstanten
#define STATIONS 30           // Anzahl der Stationen in der Liste
#define MINUTES_PER_DAY 1440  // Anzahl der Minuten in einem Tag (24 Stunden * 60 Minuten)

/* ESP8266Audio SetGain 0..4 — effektiver Maximalwert bei Slider 100 % (z. B. 0.35–0.5) */
#ifndef AUDIO_GAIN_MAX
#define AUDIO_GAIN_MAX 0.42f
#endif

/**
 * Serielle Diagnose für den gesamten Sketch (inkl. Wetter-URL/JSON).
 * 0: kein Serial.begin(), alle RADIO_SERIAL(...) werden wegpräpariert — weniger Blockierzeit an der UART,
 *    weniger Flash durch Debug-Strings; USB-Upload zum Flashen bleibt unabhängig davon möglich.
 * 1: bisheriges Verhalten. Build: -DRADIO_DEBUG_SERIAL=0
 */
#ifndef RADIO_DEBUG_SERIAL
#define RADIO_DEBUG_SERIAL 0
#endif
#if RADIO_DEBUG_SERIAL
#define RADIO_SERIAL(x) \
  do {                    \
    x;                    \
  } while (0)
#else
#define RADIO_SERIAL(x) ((void)0)
#endif

// Globale Variablen
Station stationlist[STATIONS];  // Liste der verfügbaren Stationen

#include "ui_start_colors.h" /* Startseite scr_clock: RGB565, Web + NVS (usc_*); Definition von ui_start_cols in tft_display.ino */

/* Konfiguration als char[] statt Arduino-String: kein Heap für SSID/NTP/TZ, weniger Fragmentierung. */
#define CFG_SSID_MAX 33         /* 802.11: SSID max. 32 Zeichen + NUL */
#define CFG_WPA_PSK_MAX 65      /* WPA-PSK: max. 63 ASCII + NUL */
#define CFG_NTP_HOST_MAX 96     /* Hostname (weit unter DNS-253) */
#define CFG_TZ_IANA_MAX 64      /* z. B. America/Argentina/ComodRivadavia */

// Variablen zur Speicherung von Konfigurationsdaten
char ssid[CFG_SSID_MAX] = "";
char pkey[CFG_WPA_PSK_MAX] = "";
char ntp[CFG_NTP_HOST_MAX] = "de.pool.ntp.org";
uint8_t curStation = 0;          // Index der aktuell ausgewählten Station in der stationlist
uint8_t curGain = 50;            // Aktuelle Lautstärke 0–100 (Slider)
uint8_t snoozeTime = 30;         // Einschlafzeit (Sleep-Timer) in Minuten
uint8_t alarmSnoozeMin = 0;      // Wecker-Schlummer 0–10 min (0 = Funktion aus)
uint16_t alarm1 = 390;           // Erste Alarmzeit (6:30 Uhr in Minuten nach Mitternacht)
uint8_t alarmday1 = 0B00111110;  // Gültige Wochentage für den ersten Alarm (Beispiel: 00111110 = Montag bis Freitag)
uint16_t alarm2 = 480;           // Zweite Alarmzeit (8:00 Uhr in Minuten nach Mitternacht)
uint8_t alarmday2 = 0B01000001;  // Gültige Wochentage für den zweiten Alarm (Beispiel: 01000001 = Sonntag und Samstag)
uint8_t actStation = 0;          // Index der aktuellen Station in der Liste, verwendet für das Streaming
uint8_t bright = 25;             // Helligkeit in Prozent. 0 bedeutet, dass der LDR die Helligkeit regelt

// Weitere globale Variablen
uint32_t lastchange = 0;       // Zeitpunkt der letzten Auswahländerung
uint32_t snoozeTimeEnd = 0;    // Einschlafzeit (Sleep-Timer): Ende → Radio aus
uint32_t alarmSnoozeUntil = 0; // Wecker-Schlummer: millis() bis Radio wieder an (0 = inaktiv)
bool alarmActionsVisible = false; // Stop/Schlummer-Buttons: true bei Weckstart und nach Schlummer-Ende (Radio wieder an)
uint16_t alarmtime = 0;        // Nächste relevante Alarmzeit
uint8_t alarmday = 8;          // Wochentag für den nächsten relevanten Alarm oder 8 bedeutet Alarm deaktiviert
char title[128];               // Stream-Titel (UTF-8 nach Normalisierung aus ICY-Metadaten)
bool newTitle = false;         // Flag zum Signalisieren eines neuen Titels
uint32_t tick = 0;             // Letzter Wert des Tick-Counters zur Auslösung zeitgesteuerter Ereignisse
uint32_t discon = 0;           // Tick-Counter-Wert zur Berechnung der Zeit der Trennung
uint16_t minutes;              // Aktuelle Anzahl der Minuten seit Mitternacht
uint8_t weekday;               // Aktueller Wochentag
struct tm ti;                  // Zeitstruktur mit der aktuellen Zeit
int16_t lastldr;               // Letzter Wert vom LDR-Sensor zur Erkennung von Änderungen
uint32_t start_conf;           // Zeitpunkt des Wechsels auf Favoriten-, Einstellungs- oder Alarmseite (Timeout → Startseite)
boolean connected;             // Flag zum Signalisieren einer aktiven Verbindung
boolean reconnected = false;   // Flag zum Signalisieren einer erfolgreichen Wiederverbindung
boolean radio = false;         // Flag zum Signalisieren der Radiowiedergabe
boolean startpage = true;       // Startseite mit Uhr/Wetter/Radio-Widget (scr_clock)
boolean favoritenpage = false;  // Favoriten / Senderliste (scr_config)
boolean settingspage = false;   // Einstellungen (scr_radio: u. a. Lautstärke)
boolean alarmpage = false;      // Alarm-Einstellungen (scr_alarm)
unsigned long lastUpdate = 0;  // Variable, um die letzte Anzeigeaktualisierung zu verfolgen
uint16_t lastLedb = 0;         // Globale Variable, um den zuletzt eingestellten Helligkeitswert zu speichern
bool alarmTriggered = false;
// Definitionen für die Textausrichtung
#define ALIGNLEFT 0    // Textausrichtung: linksbündig
#define ALIGNCENTER 1  // Textausrichtung: zentriert
#define ALIGNRIGHT 2   // Textausrichtung: rechtsbündig

// WLAN-Konfiguration
#define NETWORK_NAME "radioweckerlanname"  // Definiert den Hostnamen
#define AP_NAME "radioweckerconf"          // Definiert den SSID-Namen des Soft-AP

// Zeitzonen-Definition für Mitteleuropa (Deutschland, Frankreich, Italien, Spanien, etc.)
#define TIME_ZONE "CET-1CEST,M3.5.0/02,M10.5.0/03"  // Zeitzone für Mitteleuropa mit Sommerzeitregelung

// OTA-Konfiguration
#define OTA_HOSTNAME "radiowecker"   // Definiert den Hostnamen für OTA Update
#define OTA_PASSWORD "weckerupdate"  // Definiert das Passwort für OTA Update

// HTTP-OTA (Weboberfläche): Release-Asset muss exakt diesen Dateinamen haben
#define HTTP_OTA_GITHUB_OWNER "beabel"
#define HTTP_OTA_GITHUB_REPO "radiowecker"
#define HTTP_OTA_FIRMWARE_FILENAME "radiowecker-firmware.bin"
/** Mindestens freier Platz im inaktiven OTA-Slot (Bytes), sonst Abbruch vor Download */
#define HTTP_OTA_MIN_FREE_BYTES 524288u
/** NVS (Namespace „radio“): HTTP-OTA nach Neustart — Key-Namen ≤15 Zeichen */
#define PREF_HTTP_OTA_PEND "http_otapend"
#define PREF_HTTP_OTA_TAG "http_otatag"
//############## WETTER ##############################################################
// Globale Variable, um den Zeitpunkt des letzten Wetterupdates zu speichern
unsigned long lastWeatherUpdate = 0;
const unsigned long weatherUpdateInterval = 60000;  // 60 Sekunden
// Definiere Latitude, Longitude und Zeitzone
float LATITUDE = 52.520645;
float LONGITUDE = 13.409779;
char TIME_ZONE_IANA[CFG_TZ_IANA_MAX] = "Europe/Berlin";