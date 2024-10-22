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

// Globale Variablen
Station stationlist[STATIONS];  // Liste der verfügbaren Stationen

// Variablen zur Speicherung von Konfigurationsdaten
String ssid = "";                // SSID für die WLAN-Verbindung
String pkey = "";                // Passwort für die WLAN-Verbindung
String ntp = "de.pool.ntp.org";  // URL des NTP-Servers zur Zeitabgleich
uint8_t curStation = 0;          // Index der aktuell ausgewählten Station in der stationlist
uint8_t curGain = 200;           // Aktuelle Lautstärke (Gain)
uint8_t snoozeTime = 30;         // Schlummerzeit in Minuten
uint16_t alarm1 = 390;           // Erste Alarmzeit (6:30 Uhr in Minuten nach Mitternacht)
uint8_t alarmday1 = 0B00111110;  // Gültige Wochentage für den ersten Alarm (Beispiel: 00111110 = Montag bis Freitag)
uint16_t alarm2 = 480;           // Zweite Alarmzeit (8:00 Uhr in Minuten nach Mitternacht)
uint8_t alarmday2 = 0B01000001;  // Gültige Wochentage für den zweiten Alarm (Beispiel: 01000001 = Sonntag und Samstag)
uint8_t actStation = 0;          // Index der aktuellen Station in der Liste, verwendet für das Streaming
uint8_t bright = 25;             // Helligkeit in Prozent. 0 bedeutet, dass der LDR die Helligkeit regelt

// Weitere globale Variablen
uint32_t lastchange = 0;       // Zeitpunkt der letzten Auswahländerung
uint32_t snoozeTimeEnd = 0;    // Zeitpunkt zum Beenden des Schlummermodus
uint16_t alarmtime = 0;        // Nächste relevante Alarmzeit
uint8_t alarmday = 8;          // Wochentag für den nächsten relevanten Alarm oder 8 bedeutet Alarm deaktiviert
char title[64];                // Zeichenarray zur Speicherung der Metadaten-Nachricht
bool newTitle = false;         // Flag zum Signalisieren eines neuen Titels
uint32_t tick = 0;             // Letzter Wert des Tick-Counters zur Auslösung zeitgesteuerter Ereignisse
uint32_t discon = 0;           // Tick-Counter-Wert zur Berechnung der Zeit der Trennung
uint16_t minutes;              // Aktuelle Anzahl der Minuten seit Mitternacht
uint8_t weekday;               // Aktueller Wochentag
struct tm ti;                  // Zeitstruktur mit der aktuellen Zeit
int16_t lastldr;               // Letzter Wert vom LDR-Sensor zur Erkennung von Änderungen
uint32_t start_conf;           // Zeitpunkt des Betretens des Konfigurationsbildschirms
boolean connected;             // Flag zum Signalisieren einer aktiven Verbindung
boolean radio = false;         // Flag zum Signalisieren der Radiowiedergabe
boolean clockmode = true;      // Flag zum Signalisieren, dass die Uhrzeit auf dem Bildschirm angezeigt wird
boolean configpage = false;    // Flag zum Signalisieren, dass die Konfigurationsseite angezeigt wird
boolean radiopage = false;     // Flag zum Signalisieren, dass die Radioauswahlseite angezeigt wird
boolean alarmpage = false;     // Flag zum Signalisieren, dass die Alarm-Einstellseite angezeigt wird
unsigned long lastUpdate = 0;  // Variable, um die letzte Anzeigeaktualisierung zu verfolgen
uint16_t lastLedb = 0;         // Globale Variable, um den zuletzt eingestellten Helligkeitswert zu speichern
// Definitionen für die Textausrichtung
#define ALIGNLEFT 0    // Textausrichtung: linksbündig
#define ALIGNCENTER 1  // Textausrichtung: zentriert
#define ALIGNRIGHT 2   // Textausrichtung: rechtsbündig

// Definitionen für Schriftarten
#define FNT9 &AT_Standard9pt7b  // Schriftart 9pt
#define FNT12 &AT_Bold12pt7b    // Schriftart 12pt
#define LS 23                   // Zeilenabstand

// WLAN-Konfiguration
#define NETWORK_NAME "radioweckerlanname"  // Definiert den Hostnamen
#define AP_NAME "radioweckerconf"          // Definiert den SSID-Namen des Soft-AP

// Zeitzonen-Definition für Mitteleuropa (Deutschland, Frankreich, Italien, Spanien, etc.)
#define TIME_ZONE "CET-1CEST,M3.5.0/02,M10.5.0/03"  // Zeitzone für Mitteleuropa mit Sommerzeitregelung

// OTA-Konfiguration
#define OTA_HOSTNAME "radiowecker"   // Definiert den Hostnamen für OTA Update
#define OTA_PASSWORD "weckerupdate"  // Definiert das Passwort für OTA Update