#include "i18n.h"
#include <Arduino.h>
#include <Preferences.h>

extern Preferences pref;

static uint8_t app_ui_lang = 0;
volatile bool i18n_ui_dirty = false;

static const char *const S_DE[] = {
    "Disconnect",
    "Soft AP disconnect",
    "Set WLAN Mode",
    "Verbindung zu ",
    " herstellen",
    "Keine Verbindung!\nStarte Access-Point.",
    "IP-Adresse = ",
    "Reconnected",
    "Nicht verbunden",
    "Verbinde dich mit SSID ",
    "Einstellung auf IP: ",
    "OK",
    "Jetzt",
    "Heute",
    "Morgen",
    "Temp:",
    "wie:",
    "Min",
    "Max",
    "Lautstärke",
    "Helligkeit",
    "Einschlafzeit",
    "Wecker-Schlummer",
    "Sprache",
    "AUS",
    "aus",
    "min",
    "HTTP Firmware-Update",
    "Lade …",
    "WLAN verbinden …",
    "Lade Firmware …",
    "Update abgebrochen",
    "Ungültiger Tag",
    "Update fehlgeschlagen",
    "Kein WLAN",
    "URL zu lang",
    "Neustart …",
    "Code",
    "Keine aktive Station verfügbar.",
};

static const char *const S_EN[] = {
    "Disconnect",
    "Soft AP disconnect",
    "Set WLAN Mode",
    "Connecting to ",
    " …",
    "No connection!\nStarting access point.",
    "IP address = ",
    "Reconnected",
    "Not connected",
    "Connect to SSID ",
    "Open settings at IP: ",
    "OK",
    "Now",
    "Today",
    "Tomorrow",
    "Temp:",
    "feels:",
    "Min",
    "Max",
    "Volume",
    "Brightness",
    "Sleep timer",
    "Alarm snooze",
    "Language",
    "OFF",
    "off",
    "min",
    "HTTP firmware update",
    "Loading …",
    "Connecting Wi-Fi …",
    "Downloading firmware …",
    "Update aborted",
    "Invalid tag",
    "Update failed",
    "No Wi-Fi",
    "URL too long",
    "Restarting …",
    "Code",
    "No active station available.",
};

static const char *const S_FR[] = {
    "Déconnexion",
    "Soft AP déconnecté",
    "Mode WLAN",
    "Connexion à ",
    " …",
    "Pas de connexion !\nPoint d'accès démarré.",
    "Adresse IP = ",
    "Reconnecté",
    "Non connecté",
    "Connectez-vous au SSID ",
    "Réglage sur IP: ",
    "OK",
    "Maintenant",
    "Aujourd'hui",
    "Demain",
    "Temp.:",
    "ressenti:",
    "Min",
    "Max",
    "Volume",
    "Luminosité",
    "Sommeil",
    "Répétition alarme",
    "Langue",
    "ARRÊT",
    "off",
    "min",
    "Mise à jour firmware HTTP",
    "Chargement …",
    "Connexion Wi-Fi …",
    "Téléchargement …",
    "Mise à jour annulée",
    "Étiquette invalide",
    "Échec mise à jour",
    "Pas de Wi-Fi",
    "URL trop longue",
    "Redémarrage …",
    "Code",
    "Aucune station active.",
};

/* TFT: lateinische Umschrift (ASCII) — spart kyrillische Bitmap-Schriften */
static const char *const S_RU[] = {
    "Disconnect",
    "Soft AP disconnect",
    "Set WLAN Mode",
    "Podklyuchenie k ",
    " …",
    "Net svyazi!\nTochka dostupa.",
    "IP = ",
    "Reconnected",
    "Net svyazi",
    "Podklyuchites' k SSID ",
    "Nastrojka IP: ",
    "OK",
    "Seychas",
    "Segodnya",
    "Zavtra",
    "Temp.:",
    "Oshch.:",
    "Min",
    "Max",
    "Gromkost'",
    "Yarkost'",
    "Taymer sna",
    "Povtor budil'nika",
    "Yazyk",
    "VYKL.",
    "vykl.",
    "min",
    "HTTP obnovlenie PO",
    "Zagruzka …",
    "WLAN …",
    "Zagruzka PO …",
    "Obnovlenie otm.",
    "Nevernyj teg",
    "Obnovlenie oshibka",
    "Net WLAN",
    "URL dlinnyj",
    "Perezagruzka …",
    "Kod",
    "Net aktivnoj stancii.",
};

static const char *const *const S_TAB[4] = {S_DE, S_EN, S_FR, S_RU};

static const char *const DSHORT_DE[7] = {"So", "Mo", "Di", "Mi", "Do", "Fr", "Sa"};
static const char *const DSHORT_EN[7] = {"Su", "Mo", "Tu", "We", "Th", "Fr", "Sa"};
static const char *const DSHORT_FR[7] = {"Di", "Lu", "Ma", "Me", "Je", "Ve", "Sa"};
static const char *const DSHORT_RU[7] = {"Vs", "Pn", "Vt", "Sr", "Cht", "Pt", "Sb"};

static const char *const *const DSHORT_TAB[4] = {DSHORT_DE, DSHORT_EN, DSHORT_FR, DSHORT_RU};

static const char *const DLONG_DE[7] = {"Sonntag", "Montag", "Dienstag", "Mittwoch", "Donnerstag", "Freitag", "Samstag"};
static const char *const DLONG_EN[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
static const char *const DLONG_FR[7] = {"dimanche", "lundi", "mardi", "mercredi", "jeudi", "vendredi", "samedi"};
static const char *const DLONG_RU[7] = {"Voskresen'e", "Ponedel'nik", "Vtornik", "Sreda", "Chetver", "Pyatnitsa", "Subbota"};

static const char *const *const DLONG_TAB[4] = {DLONG_DE, DLONG_EN, DLONG_FR, DLONG_RU};

static const char *const MO_DE[12] = {"Jan.", "Feb.", "M\xC3\xA4rz", "April", "Mai", "Juni", "Juli", "Aug.", "Sept.", "Okt.", "Nov.", "Dez."};
static const char *const MO_EN[12] = {"Jan.", "Feb.", "Mar.", "Apr.", "May", "Jun.", "Jul.", "Aug.", "Sep.", "Oct.", "Nov.", "Dec."};
static const char *const MO_FR[12] = {"janv.", "f\xC3\xA9vr.", "mars", "avr.", "mai", "juin", "juil.", "ao\xC3\xBBt", "sept.", "oct.", "nov.", "d\xC3\xA9c."};
static const char *const MO_RU[12] = {"yanv.", "fevr.", "mart", "apr.", "maj", "ijun'", "ijul'", "avg.", "sent.", "okt.", "noj.", "dek."};

static const char *const *const MO_TAB[4] = {MO_DE, MO_EN, MO_FR, MO_RU};

static const char *const LANG_CODES[4] = {"de", "en", "fr", "ru"};

void i18n_begin(void) {
  uint8_t v = pref.getUChar("uilang", 0);
  if (v >= 4) v = 0;
  app_ui_lang = v;
}

uint8_t i18n_get_lang(void) { return app_ui_lang; }

void i18n_set_lang(uint8_t lang) {
  if (lang >= 4) lang = 0;
  app_ui_lang = lang;
  pref.putUChar("uilang", app_ui_lang);
  i18n_ui_dirty = true;
}

bool i18n_take_ui_dirty(void) {
  if (!i18n_ui_dirty) return false;
  i18n_ui_dirty = false;
  return true;
}

const char *i18n_str(uint16_t id) {
  if (id >= I18N_STR_COUNT) return "";
  return S_TAB[app_ui_lang][id];
}

const char *i18n_day_long(int wday) {
  int i = wday % 7;
  if (i < 0) i += 7;
  return DLONG_TAB[app_ui_lang][(unsigned)i];
}

const char *i18n_day_short(int wday) {
  int i = wday % 7;
  if (i < 0) i += 7;
  return DSHORT_TAB[app_ui_lang][(unsigned)i];
}

const char *i18n_month_short(int mon) {
  if (mon < 0 || mon > 11) mon = 0;
  return MO_TAB[app_ui_lang][(unsigned)mon];
}

const char *i18n_lang_code(void) { return LANG_CODES[app_ui_lang]; }
