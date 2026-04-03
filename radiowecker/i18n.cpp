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
    "Wecklautstärke",
    "Weckton",
    "Radio",
    "Piep",
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
    "Alarm volume",
    "Wake sound",
    "Radio",
    "Beep",
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
    "Volume réveil",
    "Son réveil",
    "Radio",
    "Bip",
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

/* Russisch: UTF-8 im Quelltext (TFT: Cyrillic-Fallback); Zeilenumbruch weiterhin \n */
static const char *const S_RU[] = {
    "Отключение",
    "Точка доступа откл.",
    "Режим WLAN",
    "Подключение к ",
    "...",
    "Нет связи!\nТочка доступа.",
    "IP: ",
    "Переподключено",
    "Нет связи",
    "Подключитесь к SSID ",
    "Откройте IP: ",
    "OK",
    "Сейчас",
    "Сегодня",
    "Завтра",
    "Темп.:",
    "Ощущ.:",
    "Мин",
    "Макс",
    "Громкость",
    "Яркость",
    "Таймер сна",
    "Повтор будильника",
    "Громкость будильника",
    "Звук будильника",
    "Радио",
    "Пищалка",
    "Язык",
    "ВЫКЛ.",
    "выкл.",
    "мин",
    "HTTP обновление ПО",
    "Загрузка...",
    "Подключение Wi-Fi...",
    "Загрузка ПО...",
    "Обновление отменено",
    "Неверный тег",
    "Ошибка обновления",
    "Нет Wi-Fi",
    "URL слишком длинный",
    "Перезагрузка...",
    "Код",
    "Нет активной станции.",
};

static const char *const S_ES[] = {
    "Desconectar",
    "Punto de acceso desconectado",
    "Modo WLAN",
    "Conectando a ",
    " …",
    "No hay conexión!\nIniciando punto de acceso.",
    "Dirección IP = ",
    "Reconectado",
    "No conectado",
    "Conéctese al SSID ",
    "Ajustes en IP: ",
    "OK",
    "Ahora",
    "Hoy",
    "Mañana",
    "Temp.:",
    "sensación:",
    "Mín",
    "Máx",
    "Volumen",
    "Brillo",
    "Temporizador de sueño",
    "Repetición alarma",
    "Volumen alarma",
    "Tono alarma",
    "Radio",
    "Pitido",
    "Idioma",
    "APAG",
    "apag",
    "min",
    "Actualización firmware HTTP",
    "Cargando …",
    "Conectando Wi-Fi …",
    "Descargando firmware …",
    "Actualización cancelada",
    "Etiqueta no válida",
    "Fallo de actualización",
    "Sin Wi-Fi",
    "URL demasiado larga",
    "Reiniciando …",
    "Código",
    "No hay emisora activa.",
};

static const char *const S_NL[] = {
    "Verbreken",
    "Soft AP losgekoppeld",
    "WLAN-modus",
    "Verbinding maken met ",
    " …",
    "Geen verbinding!\nToegangspunt starten.",
    "IP-adres = ",
    "Opnieuw verbonden",
    "Niet verbonden",
    "Maak verbinding met SSID ",
    "Open instellingen op IP: ",
    "OK",
    "Nu",
    "Vandaag",
    "Morgen",
    "Temp.:",
    "gevoel:",
    "Min",
    "Max",
    "Volume",
    "Helderheid",
    "Slaaptimer",
    "Wekker-snooze",
    "Wekkervolume",
    "Wekgeluid",
    "Radio",
    "Piep",
    "Taal",
    "UIT",
    "uit",
    "min",
    "HTTP firmware-update",
    "Laden …",
    "Wi-Fi verbinden …",
    "Firmware downloaden …",
    "Update afgebroken",
    "Ongeldige tag",
    "Update mislukt",
    "Geen Wi-Fi",
    "URL te lang",
    "Herstarten …",
    "Code",
    "Geen actief station.",
};

static const char *const *const S_TAB[6] = {S_DE, S_EN, S_FR, S_RU, S_ES, S_NL};

static const char *const DSHORT_DE[7] = {"So", "Mo", "Di", "Mi", "Do", "Fr", "Sa"};
static const char *const DSHORT_EN[7] = {"Su", "Mo", "Tu", "We", "Th", "Fr", "Sa"};
static const char *const DSHORT_FR[7] = {"Di", "Lu", "Ma", "Me", "Je", "Ve", "Sa"};
static const char *const DSHORT_RU[7] = {"Вс", "Пн", "Вт", "Ср", "Чт", "Пт", "Сб"};
static const char *const DSHORT_ES[7] = {"Do", "Lu", "Ma", "Mi", "Ju", "Vi", "Sá"};
static const char *const DSHORT_NL[7] = {"Zo", "Ma", "Di", "Wo", "Do", "Vr", "Za"};

static const char *const *const DSHORT_TAB[6] = {DSHORT_DE, DSHORT_EN, DSHORT_FR, DSHORT_RU, DSHORT_ES, DSHORT_NL};

static const char *const DLONG_DE[7] = {"Sonntag", "Montag", "Dienstag", "Mittwoch", "Donnerstag", "Freitag", "Samstag"};
static const char *const DLONG_EN[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
static const char *const DLONG_FR[7] = {"dimanche", "lundi", "mardi", "mercredi", "jeudi", "vendredi", "samedi"};
static const char *const DLONG_RU[7] = {
    "Воскресенье",
    "Понедельник",
    "Вторник",
    "Среда",
    "Четверг",
    "Пятница",
    "Суббота"};
static const char *const DLONG_ES[7] = {"domingo", "lunes", "martes", "miércoles", "jueves", "viernes", "sábado"};
static const char *const DLONG_NL[7] = {"zondag", "maandag", "dinsdag", "woensdag", "donderdag", "vrijdag", "zaterdag"};

static const char *const *const DLONG_TAB[6] = {DLONG_DE, DLONG_EN, DLONG_FR, DLONG_RU, DLONG_ES, DLONG_NL};

static const char *const MO_DE[12] = {"Jan.", "Feb.", "März", "April", "Mai", "Juni", "Juli", "Aug.", "Sept.", "Okt.", "Nov.", "Dez."};
static const char *const MO_EN[12] = {"Jan.", "Feb.", "Mar.", "Apr.", "May", "Jun.", "Jul.", "Aug.", "Sep.", "Oct.", "Nov.", "Dec."};
static const char *const MO_FR[12] = {"janv.", "févr.", "mars", "avr.", "mai", "juin", "juil.", "août", "sept.", "oct.", "nov.", "déc."};
static const char *const MO_RU[12] = {
    "янв.", "февр.", "март", "апр.",
    "май", "июн.", "июл.", "авг.",
    "сен.", "окт.", "ноя.", "дек."};
static const char *const MO_ES[12] = {
    "ene.", "feb.", "mar.", "abr.", "may.", "jun.",
    "jul.", "ago.", "sep.", "oct.", "nov.", "dic."};
static const char *const MO_NL[12] = {
    "jan.", "feb.", "mrt.", "apr.", "mei", "jun.",
    "jul.", "aug.", "sep.", "okt.", "nov.", "dec."};

static const char *const *const MO_TAB[6] = {MO_DE, MO_EN, MO_FR, MO_RU, MO_ES, MO_NL};

static const char *const LANG_CODES[6] = {"de", "en", "fr", "ru", "es", "nl"};

void i18n_begin(void) {
  uint8_t v = pref.getUChar("uilang", 0);
  if (v >= 4) v = 0;
  app_ui_lang = v;
}

uint8_t i18n_get_lang(void) { return app_ui_lang; }

void i18n_set_lang(uint8_t lang) {
  if (lang >= 6) lang = 0;
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
