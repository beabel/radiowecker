#ifndef TEXTE_H
#define TEXTE_H

#define TXT_DISCONNECT "Disconnect"
#define TXT_SOFT_AP_DISCONNECT "Soft AP disconnect"
#define TXT_SET_WLAN_MODE "Set WLAN Mode"
#define TXT_CONNECTING_TO "Verbindung zu "
#define TXT_CONNECTING_ESTABLISH " herstellen"
#define TXT_NO_CONNECTION "Keine Verbindung! \nStarte Access-Point."
#define TXT_IP_ADDRESS "IP-Adresse = "

#define TXT_RECONNECTED "Reconnected"
#define TXT_NOT_CONNECTED "Nicht verbunden"
#define TXT_CONNECT_TO_AP "Verbinde dich mit SSID "
#define TXT_CONFIG_IP "Einstellung auf IP: "

#define TXT_OK "OK"

//day and month names
const char* const PROGMEM days[] = { "Sonntag", "Montag", "Dienstag", "Mittwoch", "Donnerstag", "Freitag", "Samstag" };
const char* const PROGMEM days_short[] = { "So", "Mo", "Di", "Mi", "Do", "Fr", "Sa" };
const char* PROGMEM months[] = { "Jan.", "Feb.", "März", "April", "Mai", "Juni", "Juli", "Aug.", "Sept.", "Okt.", "Nov.", "Dez." };

#define TXT_NOW "Jetzt"
#define TXT_TODAY "Heute"
#define TXT_TOMMORROW "Morgen"

#define TXT_TEMP "Temp:"
#define TXT_FEELS "wie:"
#define TXT_MIN "Min"
#define TXT_MAX "Max"
#endif  // TEXTE_H