//html for web pages
const char OPTION_entry[] PROGMEM = 
"<option value='%i' %s>%s %s</option>";

const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
<meta http-equiv='content-type' content='text/html; charset=UTF-8'>
<meta name='viewport' content='width=320' />
<script src="https://code.jquery.com/jquery-3.7.1.min.js"></script>
<link rel="stylesheet" href="https://code.jquery.com/ui/1.13.2/themes/base/jquery-ui.min.css">
<script src="https://code.jquery.com/ui/1.13.2/jquery-ui.min.js"></script>
<link rel="stylesheet" href="//use.fontawesome.com/releases/v6.6.0/css/all.css">
<script>
var CURRENT_UI_LANG = "de";
var I18N = {
 de: {lang:"Sprache",title:"Web Radiowecker",w1:"Weckzeit 1:",w2:"Weckzeit 2:",save:"Speichern",cancel:"Rückgängig",list:"Liste:",name:"Name:",url:"URL:",use:"Verwenden:",pos:"Position:",btest:"Testen",bchange:"Ändern",acc_wlan:"Wlan",acc_ntp:"NTP",acc_wx:"Wetter",bsave:"Speichern",brestart:"Neustart",brestore:"Senderliste Reset",lbl_ver:"Version:",lbl_heap:"HEAP:",lbl_sk:"SKETCH:",lbl_chip:"ChipModel:",fh:"Fehler beim Abrufen der Daten.",fe:"Fehler beim Laden der Daten.",gh_err:"Fehler beim Überprüfen auf Updates.",dow_mo:"Mo",dow_di:"Di",dow_mi:"Mi",dow_do:"Do",dow_fr:"Fr",dow_sa:"Sa",dow_so:"So",testend:"Mit OK beenden Sie den Test",ota_ask:"Firmware auf",ota_rel:"Im GitHub-Release muss die Datei angehängt sein:",ota_free:"Freier OTA-Slot (ungefähr):",ota_stop:"Das Radio wird gestoppt;",ota_iprog:"Der Fortschritt erscheint auf dem Display.",ota_reboot:"Anschließend startet das Gerät neu.",ota_web:"Diese Webseite verliert danach die Verbindung zum Gerät (erwartet) — es erscheint kein weiteres Meldungsfenster.",ota_go:"Fortfahren?",gh_new:"Neue Version:",gh_inst:"Per Web installieren…",gh_rel:"Release-Seite",gh_newer:"Du verwendest eine neuere Version",gh_than:"als die neueste offizielle Version",gh_okv:"Du verwendest bereits die neueste Version",heap_u:"Belegt:",heap_f:"Frei:",heap_t:"Heap gesamt:",heap_m:"max. Block:",fw_l:"Firmware:",fw_v:"von",fw_u:"frei für nächstes Update:",fu:"Letztes Update:",fl:"Created with",sp:"Sponsor this Project:",fgh:"Fehler beim Abrufen der GitHub-Informationen.",tab_disp:"Anzeige",lbl_bright:"Helligkeit (%):",hint_bright:"0 % = automatisch per Umgebungslicht (LDR), wenn am Gerät verbaut.",lbl_sleep:"Einschlafzeit (Min.):",lbl_asnooze:"Wecker-Schlummer (Min.):",hint_asnooze:"0 = Schlummer aus.",lbl_alarmgain:"Wecklautstärke (%):",lbl_alarmwake:"Weckton:",wake_radio:"Radio",wake_beep:"Piep",colors_title:"Startbildschirm-Farben",colors_hint:"Sofort auf dem Gerät sichtbar. Werksfarben = Standard wie bei Erstinstallation.",colors_save:"Farben speichern",colors_reset:"Werksfarben",c_bg:"Hintergrund",c_ip:"IP-Zeile",c_sleep:"Schlaf-Symbol",c_alarm:"Wecker (aktiv)",c_sFill:"Slider Füllung",c_sBg:"Slider Hintergrund",c_sBd:"Slider Rahmen",c_date:"Datum",c_time:"Uhrzeit",c_boxBg:"Kästen (Wetter/Radio)",c_boxBd:"Kasten-Rahmen",c_stName:"Sendername",c_stTitle:"Titel & Wetter",help_ldr:"Mit Helligkeitssensor (LDR) am Gerät: Weitere Einstellungen stehen in der Datei 00_pin_settings.h im Projektordner radiowecker. Ohne diesen Sensor reicht der Regler hier.",help_snooze_hw:"Die Minuten gelten, wenn du Schlummer auf dem Display oder hier wählst. Zwei optionale Extra-Tasten (Stopp, Schlummer) kannst du in 00_pin_settings.h einrichten (welcher Anschluss, ein oder aus). Ohne solche Tasten brauchst du dort nichts zu ändern.",help_tip_aria:"Hinweis ein- oder ausblenden"},
 en: {lang:"Language",title:"Web clock radio",w1:"Alarm 1:",w2:"Alarm 2:",save:"Save",cancel:"Undo",list:"List:",name:"Name:",url:"URL:",use:"Use:",pos:"Position:",btest:"Test",bchange:"Apply",acc_wlan:"Wi‑Fi",acc_ntp:"NTP",acc_wx:"Weather",bsave:"Save",brestart:"Restart",brestore:"Reset station list",lbl_ver:"Version:",lbl_heap:"HEAP:",lbl_sk:"SKETCH:",lbl_chip:"Chip model:",fh:"Failed to fetch data.",fe:"Failed to load data.",gh_err:"Update check failed.",dow_mo:"Mo",dow_di:"Tu",dow_mi:"We",dow_do:"Th",dow_fr:"Fr",dow_sa:"Sa",dow_so:"Su",testend:"Press OK to end the test",ota_ask:"Install firmware",ota_rel:"The GitHub release must include file:",ota_free:"Approx. free OTA slot:",ota_stop:"Playback will stop;",ota_iprog:"progress is shown on the device.",ota_reboot:"Then the device reboots.",ota_web:"This page will lose connection (expected).",ota_go:"Continue?",gh_new:"New version:",gh_inst:"Install via web…",gh_rel:"Release page",gh_newer:"You are running a newer version",gh_than:"than the latest official release",gh_okv:"You are on the latest version",heap_u:"Used:",heap_f:"Free:",heap_t:"Heap total:",heap_m:"max block:",fw_l:"Firmware:",fw_v:"of",fw_u:"free for next update:",fu:"Last update:",fl:"Created with",sp:"Sponsor this project:",fgh:"Failed to fetch GitHub info.",tab_disp:"Display",lbl_bright:"Brightness (%):",hint_bright:"0% = ambient light (LDR) when fitted on the device.",lbl_sleep:"Sleep timer (min):",lbl_asnooze:"Alarm snooze (min):",hint_asnooze:"0 = snooze off.",lbl_alarmgain:"Alarm volume (%):",lbl_alarmwake:"Wake sound:",wake_radio:"Radio",wake_beep:"Beep",colors_title:"Home screen colors",colors_hint:"Applied on the device at once. Factory = default theme.",colors_save:"Save colors",colors_reset:"Factory colors",c_bg:"Background",c_ip:"IP line",c_sleep:"Sleep icon",c_alarm:"Alarm (on)",c_sFill:"Volume fill",c_sBg:"Volume track",c_sBd:"Volume border",c_date:"Date",c_time:"Time",c_boxBg:"Weather/radio boxes",c_boxBd:"Box border",c_stName:"Station name",c_stTitle:"Title & weather",help_ldr:"If your clock has a light sensor for the display: extra settings are in the file 00_pin_settings.h in the radiowecker project folder. If you do not have that sensor, this slider is enough.",help_snooze_hw:"These minutes are used when you tap snooze on the device or here. Optional extra buttons (stop, snooze): you can turn them on or off and set the pins in 00_pin_settings.h. If you have no extra buttons, you can ignore that file.",help_tip_aria:"Show or hide this hint"},
 fr: {lang:"Langue",title:"Radio-réveil web",w1:"Alarme 1 :",w2:"Alarme 2 :",save:"Enregistrer",cancel:"Annuler",list:"Liste :",name:"Nom :",url:"URL :",use:"Actif :",pos:"Position :",btest:"Tester",bchange:"Modifier",acc_wlan:"Wi‑Fi",acc_ntp:"NTP",acc_wx:"Météo",bsave:"Enregistrer",brestart:"Redémarrer",brestore:"Réinit. liste stations",lbl_ver:"Version :",lbl_heap:"TAS :",lbl_sk:"SKETCH :",lbl_chip:"Modèle puce :",fh:"Échec de lecture des données.",fe:"Échec du chargement.",gh_err:"Échec vérif. mises à jour.",dow_mo:"Lu",dow_di:"Ma",dow_mi:"Me",dow_do:"Je",dow_fr:"Ve",dow_sa:"Sa",dow_so:"Di",testend:"OK pour terminer le test",ota_ask:"Installer le firmware",ota_rel:"Le release GitHub doit contenir :",ota_free:"Espace OTA libre (approx.) :",ota_stop:"La radio s'arrête ;",ota_iprog:"la progression s'affiche sur l'appareil.",ota_reboot:"Puis redémarrage.",ota_web:"Cette page perdra la connexion (normal).",ota_go:"Continuer ?",gh_new:"Nouvelle version :",gh_inst:"Installer via le web…",gh_rel:"Page release",gh_newer:"Version plus récente que",gh_than:"la dernière version officielle",gh_okv:"Déjà la dernière version",heap_u:"Utilisé :",heap_f:"Libre :",heap_t:"Tas total :",heap_m:"bloc max :",fw_l:"Firmware :",fw_v:"sur",fw_u:"libre pour prochaine MAJ :",fu:"Dernière MAJ :",fl:"Créé avec",sp:"Soutenir le projet :",fgh:"Échec infos GitHub.",tab_disp:"Affichage",lbl_bright:"Luminosité (%) :",hint_bright:"0 % = lumière ambiante (LDR) si présent.",lbl_sleep:"Sommeil (min) :",lbl_asnooze:"Snooze alarme (min) :",hint_asnooze:"0 = snooze désactivé.",lbl_alarmgain:"Volume réveil (%) :",lbl_alarmwake:"Son réveil :",wake_radio:"Radio",wake_beep:"Bip",colors_title:"Couleurs écran d’accueil",colors_hint:"Appliquées tout de suite. Usine = défaut.",colors_save:"Enregistrer",colors_reset:"Couleurs d’usine",c_bg:"Fond",c_ip:"Ligne IP",c_sleep:"Icône sommeil",c_alarm:"Alarme (actif)",c_sFill:"Curseur (rempl.)",c_sBg:"Curseur (piste)",c_sBd:"Curseur (bord)",c_date:"Date",c_time:"Heure",c_boxBg:"Boîtes météo/radio",c_boxBd:"Bord des boîtes",c_stName:"Nom station",c_stTitle:"Titre & météo",help_ldr:"Avec capteur de lumière (LDR) : réglages supplémentaires dans le fichier 00_pin_settings.h du dossier radiowecker. Sans capteur, ce curseur suffit.",help_snooze_hw:"Ces minutes s’appliquent au snooze sur l’écran ou ici. Boutons supplémentaires (arrêt, snooze) : à configurer dans 00_pin_settings.h. Sans ces boutons, rien à changer dans ce fichier.",help_tip_aria:"Afficher ou masquer l’info"},
 ru: {lang:"Язык",title:"Веб‑радиобудильник",w1:"Будильник 1:",w2:"Будильник 2:",save:"Сохранить",cancel:"Отменить",list:"Список:",name:"Имя:",url:"URL:",use:"Вкл.:",pos:"Позиция:",btest:"Тест",bchange:"Изменить",acc_wlan:"Wi‑Fi",acc_ntp:"NTP",acc_wx:"Погода",bsave:"Сохранить",brestart:"Перезагрузка",brestore:"Сброс списка станций",lbl_ver:"Версия:",lbl_heap:"КУЧА:",lbl_sk:"ПРОШИВКА:",lbl_chip:"Чип:",fh:"Ошибка получения данных.",fe:"Ошибка загрузки.",gh_err:"Ошибка проверки обновлений.",dow_mo:"Пн",dow_di:"Вт",dow_mi:"Ср",dow_do:"Чт",dow_fr:"Пт",dow_sa:"Сб",dow_so:"Вс",testend:"Нажмите OK, чтобы завершить тест",ota_ask:"Установить прошивку",ota_rel:"В релизе GitHub должен быть файл:",ota_free:"Свободно под OTA (прибл.):",ota_stop:"Радио остановится;",ota_iprog:"прогресс на экране устройства.",ota_reboot:"Затем перезагрузка.",ota_web:"Страница потеряет связь (норма).",ota_go:"Продолжить?",gh_new:"Новая версия:",gh_inst:"Установить через веб…",gh_rel:"Страница релиза",gh_newer:"У вас новее, чем",gh_than:"последний официальный релиз",gh_okv:"Уже последняя версия",heap_u:"Занято:",heap_f:"Свободно:",heap_t:"Куча всего:",heap_m:"макс. блок:",fw_l:"Прошивка:",fw_v:"из",fw_u:"свободно для обновления:",fu:"Последнее обновление:",fl:"Создано с",sp:"Поддержать проект:",fgh:"Ошибка GitHub.",tab_disp:"Экран",lbl_bright:"Яркость (%):",hint_bright:"0 % = по датчику освещённости (LDR), если есть.",lbl_sleep:"Таймер сна (мин):",lbl_asnooze:"Отложить будильник (мин):",hint_asnooze:"0 = выкл.",lbl_alarmgain:"Громкость будильника (%):",lbl_alarmwake:"Звук будильника:",wake_radio:"Радио",wake_beep:"Пищалка",colors_title:"Цвета главного экрана",colors_hint:"Сразу на устройстве. Завод = по умолчанию.",colors_save:"Сохранить",colors_reset:"Сброс цветов",c_bg:"Фон",c_ip:"Строка IP",c_sleep:"Сон",c_alarm:"Будильник",c_sFill:"Громкость заливка",c_sBg:"Громкость фон",c_sBd:"Громкость рамка",c_date:"Дата",c_time:"Время",c_boxBg:"Блоки погоды/радио",c_boxBd:"Рамка блоков",c_stName:"Станция",c_stTitle:"Текст и погода",help_ldr:"Если есть датчик света (LDR): доп. настройки в файле 00_pin_settings.h в папке radiowecker. Без датчика достаточно ползунка.",help_snooze_hw:"Минуты для отложенного будильника на экране или здесь. Дополнительные кнопки (стоп, сон): включаются в 00_pin_settings.h. Без кнопок файл можно не трогать.",help_tip_aria:"Показать или скрыть подсказку"},
 es: {lang:"Idioma",title:"Radio despertador web",w1:"Alarma 1:",w2:"Alarma 2:",save:"Guardar",cancel:"Deshacer",list:"Lista:",name:"Nombre:",url:"URL:",use:"Usar:",pos:"Posición:",btest:"Probar",bchange:"Aplicar",acc_wlan:"Wi‑Fi",acc_ntp:"NTP",acc_wx:"Tiempo",bsave:"Guardar",brestart:"Reiniciar",brestore:"Restablecer lista de emisoras",lbl_ver:"Versión:",lbl_heap:"HEAP:",lbl_sk:"SKETCH:",lbl_chip:"Modelo de chip:",fh:"Error al obtener los datos.",fe:"Error al cargar los datos.",gh_err:"Error al comprobar actualizaciones.",dow_mo:"Lu",dow_di:"Ma",dow_mi:"Mi",dow_do:"Ju",dow_fr:"Vi",dow_sa:"Sá",dow_so:"Do",testend:"Pulse OK para finalizar la prueba",ota_ask:"Instalar firmware",ota_rel:"El release de GitHub debe incluir el archivo:",ota_free:"Espacio OTA libre (aprox.):",ota_stop:"Se detiene la reproducción;",ota_iprog:"el progreso se muestra en el dispositivo.",ota_reboot:"Luego el dispositivo se reinicia.",ota_web:"Esta página perderá la conexión (esperado).",ota_go:"¿Continuar?",gh_new:"Nueva versión:",gh_inst:"Instalar vía web…",gh_rel:"Página del release",gh_newer:"Tiene una versión más reciente",gh_than:"que el último release oficial",gh_okv:"Ya tiene la última versión",heap_u:"Usado:",heap_f:"Libre:",heap_t:"Montón total:",heap_m:"bloque máx.:",fw_l:"Firmware:",fw_v:"de",fw_u:"libre para el próximo update:",fu:"Última actualización:",fl:"Creado con",sp:"Patrocinar el proyecto:",fgh:"Error al obtener GitHub.",tab_disp:"Pantalla",lbl_bright:"Brillo (%):",hint_bright:"0 % = luz ambiente (LDR) si está instalado.",lbl_sleep:"Temporizador sueño (min):",lbl_asnooze:"Snooze alarma (min):",hint_asnooze:"0 = snooze desactivado.",lbl_alarmgain:"Volumen alarma (%):",lbl_alarmwake:"Tono alarma:",wake_radio:"Radio",wake_beep:"Pitido",colors_title:"Colores pantalla inicio",colors_hint:"Se aplican al instante. Fábrica = predeterminado.",colors_save:"Guardar",colors_reset:"Colores de fábrica",c_bg:"Fondo",c_ip:"Línea IP",c_sleep:"Icono sueño",c_alarm:"Alarma (on)",c_sFill:"Volumen relleno",c_sBg:"Volumen pista",c_sBd:"Volumen borde",c_date:"Fecha",c_time:"Hora",c_boxBg:"Cajas clima/radio",c_boxBd:"Borde caja",c_stName:"Emisora",c_stTitle:"Título y clima",help_ldr:"Con sensor de luz (LDR): más ajustes en el archivo 00_pin_settings.h de la carpeta radiowecker. Sin sensor, basta este control.",help_snooze_hw:"Los minutos valen para repetir alarma en pantalla o aquí. Botones extra (parar, repetir): configúralos en 00_pin_settings.h. Sin botones extra, no hace falta tocar ese archivo.",help_tip_aria:"Mostrar u ocultar la nota"},
 nl: {lang:"Taal",title:"Webklokradio",w1:"Wekker 1:",w2:"Wekker 2:",save:"Opslaan",cancel:"Ongedaan maken",list:"Lijst:",name:"Naam:",url:"URL:",use:"Gebruik:",pos:"Positie:",btest:"Test",bchange:"Toepassen",acc_wlan:"Wi‑Fi",acc_ntp:"NTP",acc_wx:"Weer",bsave:"Opslaan",brestart:"Herstarten",brestore:"Zenderlijst resetten",lbl_ver:"Versie:",lbl_heap:"HEAP:",lbl_sk:"SKETCH:",lbl_chip:"Chipmodel:",fh:"Gegevens ophalen mislukt.",fe:"Laden mislukt.",gh_err:"Updatecontrole mislukt.",dow_mo:"Ma",dow_di:"Di",dow_mi:"Wo",dow_do:"Do",dow_fr:"Vr",dow_sa:"Za",dow_so:"Zo",testend:"Druk op OK om de test te beëindigen",ota_ask:"Firmware installeren",ota_rel:"De GitHub-release moet het bestand bevatten:",ota_free:"Vrij OTA-blok (ca.):",ota_stop:"Afspelen stopt;",ota_iprog:"voortgang op het apparaat.",ota_reboot:"Daarna start het apparaat opnieuw.",ota_web:"Deze pagina verliest de verbinding (verwacht).",ota_go:"Doorgaan?",gh_new:"Nieuwe versie:",gh_inst:"Installeren via web…",gh_rel:"Releasepagina",gh_newer:"U heeft een nieuwere versie",gh_than:"dan de laatste officiële release",gh_okv:"U heeft al de nieuwste versie",heap_u:"Bezet:",heap_f:"Vrij:",heap_t:"Heap totaal:",heap_m:"max. blok:",fw_l:"Firmware:",fw_v:"van",fw_u:"vrij voor volgende update:",fu:"Laatste update:",fl:"Gemaakt met",sp:"Project steunen:",fgh:"GitHub-info ophalen mislukt.",tab_disp:"Scherm",lbl_bright:"Helderheid (%):",hint_bright:"0 % = omgevingslicht (LDR) indien aanwezig.",lbl_sleep:"Slaaptimer (min):",lbl_asnooze:"Alarm-snooze (min):",hint_asnooze:"0 = snooze uit.",lbl_alarmgain:"Wekkervolume (%) :",lbl_alarmwake:"Wekgeluid:",wake_radio:"Radio",wake_beep:"Piep",colors_title:"Kleuren startscherm",colors_hint:"Meteen op het apparaat. Fabriek = standaard.",colors_save:"Opslaan",colors_reset:"Fabriekskleuren",c_bg:"Achtergrond",c_ip:"IP-regel",c_sleep:"Slaap-icoon",c_alarm:"Wekker (aan)",c_sFill:"Volume vulling",c_sBg:"Volume spoor",c_sBd:"Volume rand",c_date:"Datum",c_time:"Tijd",c_boxBg:"Weer/radio-vakken",c_boxBd:"Vakrand",c_stName:"Zendernaam",c_stTitle:"Titel & weer",help_ldr:"Met lichtsensor (LDR): extra instellingen in het bestand 00_pin_settings.h in de map radiowecker. Zonder sensor volstaat deze schuifbalk.",help_snooze_hw:"De minuten gelden voor sluimeren op het scherm of hier. Optionele extra knoppen (stop, sluimeren): in 00_pin_settings.h in- of uitschakelen. Zonder extra knoppen hoef je dat bestand niet aan te passen.",help_tip_aria:"Tip tonen of verbergen"}
};
function tr(k) { var o = I18N[CURRENT_UI_LANG] || I18N.de; if (o[k] !== undefined) return o[k]; return (I18N.de[k] !== undefined) ? I18N.de[k] : k; }
function applyWebLang(code) {
  if (!I18N[code]) code = "de";
  CURRENT_UI_LANG = code;
  document.documentElement.lang = code;
  $("[data-i18n]").each(function() {
    var k = $(this).attr("data-i18n");
    if (k) $(this).text(tr(k));
  });
  $("[data-i18n-title]").each(function() {
    var k = $(this).attr("data-i18n-title");
    if (k) {
      var t = tr(k);
      $(this).attr("title", t);
      $(this).attr("aria-label", tr("help_tip_aria"));
      var $p = $(this).siblings(".help-tip-panel");
      if ($p.length) $p.text(t);
    }
  });
  /* Info-Tab: Heap-/Sketch-Zeilen, GitHub-Block und Footer nutzen tr() nur beim AJAX-Refresh */
  getInfo();
  updateGitHubInfo();
}
$(document).ready(function() {
  $("#tabs").tabs();
  $( "#accordion" ).accordion({
    heightStyle: "content"
  });
  //Standard Stuff
  getAll();
  $("#btn_save").click(saveSSID);
  $("#btn_reset").click(restartHost);
  $("#btn_test").click(testStation);
  $("#btn_updt").click(updateStation);
  $("#btn_restore").click(restoreStations);
  $("#btn_savealarm").click(setAlarms);
  $("#btn_cancelalarm").click(getAlarms);
  $("#ui_lang_sel").on("change", function() {
    var v = $(this).val();
    $.get("/cmd/setlang", { l: v }, function() { applyWebLang(v); });
  });
  $("#stationlist").change(getStation);
  // Player Tab ######################
  $("#btn_play").click(startPlay);// senden Play zum ESP
  $("#btn_stop").click(stopPlay);// senden Stop zum ESP
  $("#btn_alarm").click(btnAlarm);// toggle Alarm zum ESP
  $("#btn_sleep").click(startSleep);// toggle Sleep zum ESP 
  GainSlider();// senden des Sliders zum ESP
  setupDisplaySliders();
  function closeAllHelpTips() {
    $("#display .help-tip-wrap.is-open, #wecker .help-tip-wrap.is-open").removeClass("is-open");
    $("#display .help-tip, #wecker .help-tip").attr("aria-expanded", "false");
    $("#display .help-tip-panel, #wecker .help-tip-panel").attr("aria-hidden", "true");
  }
  $(document).on("click", "#display .help-tip-panel, #wecker .help-tip-panel", function(e) { e.stopPropagation(); });
  $(document).on("click", "#display .help-tip, #wecker .help-tip", function(e) {
    e.preventDefault();
    e.stopPropagation();
    var $w = $(this).closest(".help-tip-wrap");
    var wasOpen = $w.hasClass("is-open");
    closeAllHelpTips();
    if (!wasOpen) {
      $w.addClass("is-open");
      $(this).attr("aria-expanded", "true");
      $w.find(".help-tip-panel").attr("aria-hidden", "false");
    }
  });
  $(document).on("click", function() { closeAllHelpTips(); });
  $(document).on("keydown", function(e) {
    if (e.key === "Escape") closeAllHelpTips();
  });
  $("#btn_bwd").click(beforeStation);// toggle beforeStation zum ESP 
  $("#btn_fwd").click(nextStation);// toggle nextStation zum ESP 
  $("#switchStation").on("change", selectStationFromDropdown);
  $("#btn_colors_save").click(saveStartColors);
  $("#btn_colors_reset").click(resetStartColors);
  $("input[name='alm_wake']").on("change", function () {
    $.get("/cmd/setAlarmWake", { value: $(this).val() });
  });

  // Zeugs um die aktuellen Daten des ESP regelmäßig zu erhalten um halbwegs sinvolle Anzeigen zu sehen
  $("#tabs").tabs({
    activate: function(event, ui) {
      var href = ui.newTab.find("a").attr("href");
      var prevPl = ui.oldPanel.data("intervalId");
      if (prevPl) clearInterval(prevPl);
      ui.oldPanel.removeData("intervalId");
      var prevWk = ui.oldPanel.data("weckerPollId");
      if (prevWk) clearInterval(prevWk);
      ui.oldPanel.removeData("weckerPollId");
      var prevHeap = ui.oldPanel.data("heapPollId");
      if (prevHeap) clearInterval(prevHeap);
      ui.oldPanel.removeData("heapPollId");

      if (href === "#player") {
        var intervalId = setInterval(updateCurrentStatus, 5000);
        ui.newPanel.data("intervalId", intervalId);
      }
      if (href === "#display") {
        loadDisplaySettings();
        loadStartColors();
      }
      if (href === "#wecker") {
        loadDisplaySettings();
        var weckerPollId = setInterval(updateCurrentStatus, 3000);
        ui.newPanel.data("weckerPollId", weckerPollId);
        updateCurrentStatus();
      }
      if (href === "#info") {
        refreshHeapStats();
        var heapPollId = setInterval(refreshHeapStats, HEAP_POLL_INTERVAL_MS);
        ui.newPanel.data("heapPollId", heapPollId);
      }
    }
  });

  if ($("#tabs").tabs("option", "active") === 0) { // 0 entspricht dem Index von "#player"
    updateCurrentStatus();
    var intervalId = setInterval(updateCurrentStatus, 3000); // Starten des Intervall
    $("#player").data("intervalId", intervalId);
  }
  if ($("#tabs").tabs("option", "active") === 5) { // Info-Tab: Heap live
    refreshHeapStats();
    var heapPollId0 = setInterval(refreshHeapStats, HEAP_POLL_INTERVAL_MS);
    $("#info").data("heapPollId", heapPollId0);
  }
         
});

function getAll() {
    $.getJSON("/cmd/getlang", function(d) {
      var c = (d && d.lang) ? d.lang : "de";
      $("#ui_lang_sel").val(c);
      applyWebLang(c);
      getSSID();
      getStationList();
      getAlarms();
      loadDisplaySettings();
      loadStartColors();
    }).fail(function() {
      applyWebLang("de");
      getSSID();
      getStationList();
      getAlarms();
      loadDisplaySettings();
      loadStartColors();
    });
}

function getStationList() {
    $.when(
        $.ajax({ type: "GET", url: "/cmd/stations", data: {} }),
        $.ajax({ type: "GET", url: "/cmd/stationsActive", data: {} })
    ).done(function (full, activeOnly) {
        $("#stationlist").html(full[0]);
        $("#switchStation").html(activeOnly[0]);
        getStation();
    });
}

function getStation() {
    $.ajax({
        type:"GET",url:"/cmd/getstation",
        data:{"stationid":$("#stationlist").val()},
        success: function(data){
            const parts = data.split("\n");
            $("#name_input").val(parts[0]);
            $("#url_input").val(parts[1]);
            $("#enable_input").prop("checked",(parts[2] == 1));
            $("#pos_input").val(parts[3]);
        }
    });
}

function updateStation() {
    const checked = $("#enable_input").prop("checked")?1:0;
    $.ajax({
        type:"GET",
        url:"/cmd/setstation",
        data:{"stationid":$("#stationlist").val(),
                    "name":$("#name_input").val(),
                    "url":$("#url_input").val(),
                    "enabled":checked,
                    "position":$("#pos_input").val()},
        success: function(data){
            const n = ( $("#enable_input").prop("checked"))?"&#x25cf; ":"&#x2002; "
            $( "#stationlist option:selected" ).html(n + $("#name_input").val());
            alert(data);
            getStationList();
        }
    });
}

function restoreStations() {
    $.ajax({
        type:"GET",
        url:"/cmd/restorestations",
        data:{},
        success: function(data){
            getStationList();
        }
    });
}

function testStation() {
    const checked = $("#enable_input").prop("checked")?1:0;
    $.ajax({
        type:"GET",
        url:"/cmd/teststation",
        data:{"url":$("#url_input").val()},
        success: function(data){
            alert(tr("testend"));
            endTest();
        },
        error: function() {
            alert("ERROR");
        }
    });
}

function endTest() {
    const checked = $("#enable_input").prop("checked")?1:0;
    $.ajax({
        type:"GET",
        url:"/cmd/endtest",
        data:{},
        success: function(data){
        }
    });
}

function getSSID() {
    $.ajax({
        type:"GET",
        url:"/cmd/getaccess",
        data:{},
        success: function(data){
            const parts = data.split("\n");
            $("#ssid_input").val(parts[0]);
            $("#pkey_input").val(parts[1]);
            $("#ntp_input").val(parts[2]);
            $("#TIME_ZONE_IANA_input").val(parts[3]);
            $("#LATITUDE_input").val(parts[4]);   // Latitude hinzufügen
            $("#LONGITUDE_input").val(parts[5]);  // Longitude hinzufügen
        }
    });
}

function getAlarms() {
  $.ajax({
      type:"GET",
      url:"/cmd/getalarms",
      data:{},
      success: function(data){
          const parts = data.split("\n");
          for (var i = 0; i < 16; i++) {
              if ((i==0) || (i==8)) {
                  $("#al"+i).val(parts[i]);
              } else {
                  if (parts[i]=='1') {
                      $("#al"+i).prop('checked',true);
                  } else {
                      $("#al"+i).prop('checked',false);
                  }
              }
          }
      }
  });
}

function saveSSID() {
    $.ajax({
        type:"GET",
        url:"/cmd/setaccess",
        data:{
          "ssid": $("#ssid_input").val(),
          "pkey": $("#pkey_input").val(),
          "ntp": $("#ntp_input").val(),
          "TIME_ZONE_IANA": $("#TIME_ZONE_IANA_input").val(),
          "LATITUDE": $("#LATITUDE_input").val(),    // Latitude hinzufügen
          "LONGITUDE": $("#LONGITUDE_input").val()   // Longitude hinzufügen
        },
        success: function(data){
            alert(data);
        }
    });
}

function setAlarms() {
    const vals = new Object();
    for (var i = 0; i < 16; i++) {
        if ((i == 0) || (i == 8)) {
            vals['al'+i] = $("#al"+i).val();
        } else {
            vals['al'+i] = $("#al"+i).prop("checked")?'1':'0';
        }
    }

    $.ajax({
        type:"GET",
        url:"/cmd/setalarms",
        data:vals,
        success: function(data){
            alert(data);
        }
    });
}

function restartHost() {
    $.ajax({
        type:"POST",
        url:"/cmd/restart",
        data:{},
    });
}
// Player Tab ######################
function startPlay() {
    $.ajax({
        type:"POST",
        url:"/cmd/startPlay",
        data:{},
        success: function () { updateCurrentStatus(); }
    });
}
function stopPlay() {
    $.ajax({
        type:"POST",
        url:"/cmd/stopPlay",
        data:{},
        success: function () { updateCurrentStatus(); }
    });
}
function setupDisplaySliders() {
  $("#slider_bright").slider({
    min: 0,
    max: 100,
    slide: function (event, ui) {
      $("#disp_bright_val").text(ui.value + "%");
    },
    stop: function (event, ui) {
      $.get("/cmd/setBright", { value: ui.value });
    }
  });
  $("#slider_sleep").slider({
    min: 0,
    max: 60,
    slide: function (event, ui) {
      $("#disp_sleep_val").text(ui.value);
    },
    stop: function (event, ui) {
      $.get("/cmd/setSleepTimer", { min: ui.value });
    }
  });
  $("#slider_asnooze").slider({
    min: 0,
    max: 10,
    slide: function (event, ui) {
      $("#disp_asnooze_val").text(ui.value);
    },
    stop: function (event, ui) {
      $.get("/cmd/setAlarmSnooze", { min: ui.value });
    }
  });
  $("#slider_alarmgain").slider({
    min: 0,
    max: 100,
    slide: function (event, ui) {
      $("#disp_alarmgain_val").text(ui.value + "%");
    },
    stop: function (event, ui) {
      $.get("/cmd/setAlarmGain", { value: ui.value });
    }
  });
}
function loadDisplaySettings() {
  $.getJSON("/cmd/getDisplaySettings", function (d) {
    if (!d) return;
    if (d.bright !== undefined) {
      $("#slider_bright").slider("option", "value", d.bright);
      $("#disp_bright_val").text(d.bright + "%");
    }
    if (d.sleepTimerMin !== undefined) {
      $("#slider_sleep").slider("option", "value", d.sleepTimerMin);
      $("#disp_sleep_val").text(d.sleepTimerMin);
    }
    if (d.alarmSnoozeMin !== undefined) {
      $("#slider_asnooze").slider("option", "value", d.alarmSnoozeMin);
      $("#disp_asnooze_val").text(d.alarmSnoozeMin);
    }
    if (d.alarmGain !== undefined) {
      $("#slider_alarmgain").slider("option", "value", d.alarmGain);
      $("#disp_alarmgain_val").text(d.alarmGain + "%");
    }
    if (d.alarmWakeMode !== undefined) {
      var wm = parseInt(d.alarmWakeMode, 10) ? 1 : 0;
      $("#alm_wake" + wm).prop("checked", true);
    }
  });
}
var START_COLOR_FIELDS = [
  ["col_bg", "bg"],
  ["col_ip", "ip"],
  ["col_sleep", "sleep"],
  ["col_alarm", "alarm"],
  ["col_sFill", "sFill"],
  ["col_sBg", "sBg"],
  ["col_sBd", "sBd"],
  ["col_date", "date"],
  ["col_time", "time"],
  ["col_boxBg", "boxBg"],
  ["col_boxBd", "boxBd"],
  ["col_stName", "stName"],
  ["col_stTitle", "stTitle"]
];
function loadStartColors() {
  $.getJSON("/cmd/getStartColors", function (d) {
    if (!d) return;
    START_COLOR_FIELDS.forEach(function (pair) {
      var v = d[pair[1]];
      if (v) $("#" + pair[0]).val(v);
    });
  });
}
function saveStartColors() {
  var o = {};
  START_COLOR_FIELDS.forEach(function (pair) {
    o[pair[1]] = $("#" + pair[0]).val();
  });
  $.ajax({
    type: "POST",
    url: "/cmd/setStartColors",
    contentType: "application/json; charset=UTF-8",
    data: JSON.stringify(o),
    success: function (data) {
      loadStartColors();
      alert(data);
    },
    error: function () {
      alert(tr("fe"));
    }
  });
}
function resetStartColors() {
  $.ajax({
    type: "POST",
    url: "/cmd/resetStartColors",
    success: function () {
      loadStartColors();
    },
    error: function () {
      alert(tr("fe"));
    }
  });
}
function GainSlider() {
  $("#GainSlider").slider({
    min: 0,
    max: 100,    
    stop: function(event, ui) {
      var sliderValue = ui.value;
      console.log("Slider-Wert:", sliderValue);
      // Daten senden
      $.ajax({
          type:"GET",
          url:"/cmd/GainSlider",
          data:{"GainValue":sliderValue},
          success: function(data){

          },
          error: function() {
              alert("ERROR");
          }
      });
    }   
  });
  updateCurrentStatus();
}
function btnAlarm() {
    $.ajax({
        type:"POST",
        url:"/cmd/btnAlarm",
        data:{},
    });
}
function startSleep() {
    $.ajax({
        type:"POST",
        url:"/cmd/startSleep",
        data:{},
    });
}
function updateCurrentStatus() {
  $.ajax({
    type: "GET",
    url: "/cmd/getCurrentStatus",
    success: function (data) {
      // Lautstärke
      updateCurrentStatusVolume(data.gain);
      if (data.alarmGain !== undefined && data.alarmGain !== null) {
        var ag = parseInt(data.alarmGain, 10);
        if (!isNaN(ag)) {
          $("#slider_alarmgain").slider("option", "value", ag);
          $("#disp_alarmgain_val").text(ag + "%");
        }
      }
      if (data.alarmWakeMode !== undefined && data.alarmWakeMode !== null) {
        var wm = parseInt(data.alarmWakeMode, 10) ? 1 : 0;
        $("#alm_wake" + wm).prop("checked", true);
      }
      // Alarm 
      updateCurrentStatusAlarm(data.alarm, data.alarmtime);
      // Radio
      updateCurrentStatusRadio(data.radioStation, data.radioTitle);
      updateCurrentStatusPlayStop(data.radioPlaying);
      if (data.actStation !== undefined && data.actStation !== null) {
        var $ss = $("#switchStation");
        if (!$ss.is(":focus")) $ss.val(String(data.actStation));
      }
      // Zeit + Datum
      updateCurrentStatusDateTime(data.Date, data.Time);
      // WLAN
      updateCurrentStatusWlan(data.Rssi);
      // Sleep Timer
      updateCurrentStatusSleep(data.Sleep);
      //console.log(data);
    },
    error: function () {
      alert(tr("fe"));
    },
  });
}
function updateCurrentStatusVolume(gain) {
  var currentGain = parseInt(gain);
  $("#GainSlider").slider("value", currentGain);
}
function updateCurrentStatusAlarm(alarm, alarmtime) {
  var alarmIcon, alarmColor, btn_alarmIcon;
  if (parseInt(alarm) === 0) {
    alarmIcon = '<i class="fas fa-bell-slash"></i>';
    alarmColor = 'red';
    btn_alarmIcon = '<i class="far fa-bell"></i>';
  } else if (parseInt(alarm) === 1) {
    alarmIcon = '<i class="far fa-bell"></i>';
    alarmColor = 'orange';
    btn_alarmIcon = '<i class="fas fa-bell-slash"></i>';
  }
  $("#alarmIcon").html(alarmIcon);
  $("#alarmIcon").css('color', alarmColor);
  $("#alarmTime").text(alarmtime);
  $("#alarmTime").css('color', alarmColor);
  $("#btn_alarm").html(btn_alarmIcon);
}
function updateCurrentStatusRadio(station, title) {
  if (station) {
    $("#radioStation").text(station);
  } else {
    $("#radioStation").html("<br />");
  }
  if (title) {
    $("#radioTitle").text(title);
  } else {
    $("#radioTitle").html("<br /><br />");
  }
}
function updateCurrentStatusPlayStop(radioPlaying) {
  var on = radioPlaying === 1 || radioPlaying === true || parseInt(radioPlaying, 10) === 1;
  $("#btn_play").prop("disabled", on);
  $("#btn_stop").prop("disabled", !on);
}
function updateCurrentStatusDateTime(date, time) {
  $("#Date").text(date);
  $("#Time").text(time);
}
function updateCurrentStatusWlan(Rssi) {
  var wlanColor;
  if (parseInt(Rssi) <= -70) {
    wlanColor = 'red';
  } else if (parseInt(Rssi) <= -50) {
    wlanColor = 'yellow';
  } else {
    wlanColor = 'green';
  }
  $("#WlanRSSI").text(Rssi);
  $("#WlanSym").css('color', wlanColor);
}
function updateCurrentStatusSleep(sleep) {
  var sleepIcon, btn_sleephidden;
  if (parseInt(sleep) === 0) {
    sleepIcon = '';
    btn_sleephidden = '';
  } else {
    sleepIcon = '<i class="fas fa-bed"></i>';
    btn_sleephidden = 'hidden';
  }
  $("#sleepIcon").html(sleepIcon);
  $("#btn_sleep").css('visibility', btn_sleephidden);
}
function beforeStation() {
    $.ajax({
        type: "POST",
        url: "/cmd/beforeStation",
        data: {},
        success: function (response) {
            if (response === "OK") {
              getStationList();
            }
        }
    });
}
function nextStation() {
    $.ajax({
        type: "POST",
        url: "/cmd/nextStation",
        data: {},
        success: function (response) {
            if (response === "OK") {
              getStationList();
            }
        }
    });
}
function selectStationFromDropdown() {
    var sid = $("#switchStation").val();
    if (sid === undefined || sid === null) return;
    $.ajax({
        type: "POST",
        url: "/cmd/selectStation",
        data: { stationid: sid },
        success: function (response) {
            if (response === "OK") {
                updateCurrentStatus();
                getStationList();
            } else {
                alert(response);
                getStationList();
            }
        }
    });
}

function applyHeapFromInfoData(data) {
  var heap = null;
  if (data && data.ESP_INFO && data.ESP_INFO.HEAP) heap = data.ESP_INFO.HEAP;
  else if (data && typeof data.getFreeHeap === "number") heap = data;
  if (!heap) return;
  var heapSize = heap.getHeapSize;
  var freeHeap = heap.getFreeHeap;
  var usedHeap = heapSize - freeHeap;
  var heapUsedPct = heapSize ? (usedHeap / heapSize) * 100 : 0;
  var heapFreePct = heapSize ? (freeHeap / heapSize) * 100 : 0;
  $("#heapprogressbar").progressbar({
    value: heapUsedPct
  });
  var heapTxt = tr("heap_u") + " " + usedHeap + " B (" + heapUsedPct.toFixed(2) + "%) · " + tr("heap_f") + " " + freeHeap + " B (" + heapFreePct.toFixed(2) + "%) · " + tr("heap_t") + " " + heapSize + " B";
  var maxAlloc = heap.getMaxAllocHeap;
  if (typeof maxAlloc === "number") {
    heapTxt += " · " + tr("heap_m") + " " + maxAlloc + " B";
  }
  $("#heapBarText").text(heapTxt);
}

/* /cmd/getHeap: kleine Antwort auf dem Gerät — seltener als früher, weniger I2S/WiFi-Konflikt bei laufendem Radio */
var HEAP_POLL_INTERVAL_MS = 10000;

function refreshHeapStats() {
  $.ajax({
    type: "GET",
    url: "/cmd/getHeap",
    dataType: "json",
    success: function (data) {
      applyHeapFromInfoData(data);
    }
  });
}

function getInfo() {
  $.ajax({
    type: "GET",
    url: "/cmd/getInfo",
    success: function (data) {
      // Ausgabe des JSON-Objekts in der Konsole
      console.log(data);

      // Funktion für die rekursive Iteration über das JSON-Objekt
      function iterateObject(obj, prefix = "") {
        $.each(obj, function(key, value) {
          // Überprüfe, ob es sich um ein Objekt handelt (Unterordner)
          if (typeof value === 'object') {
            // Wenn ja, rekursiv weiter iterieren
            iterateObject(value, prefix + key + "_");
          } else {
            // Wenn nicht, aktualisiere das entsprechende HTML-Element
            $("#" + prefix + key).text(value);
            // Ausgabe der ID auf der Konsole
            //console.log("Element ID:", prefix + key);
          }
        });
      }

      // Starte die rekursive Iteration mit dem Hauptobjekt
      iterateObject(data);

      applyHeapFromInfoData(data);

      /* Partition: getSketchSize = aktuelle Firmware, getFreeSketchSpace = noch frei in der Slot-Partition */
      var usedSketch = data.ESP_INFO.SKETCH.getSketchSize;
      var freeSketch = data.ESP_INFO.SKETCH.getFreeSketchSpace;
      var totalSketch = usedSketch + freeSketch;
      var sketchUsedPct = totalSketch ? (usedSketch / totalSketch) * 100 : 0;
      $( "#sketchprogressbar" ).progressbar({
        value: sketchUsedPct
      });
      $("#sketchBarText").text(
        tr("fw_l") + " " + usedSketch + " B (" + sketchUsedPct.toFixed(2) + "%) " + tr("fw_v") + " " + totalSketch + " B · " + tr("fw_u") + " " + freeSketch + " B"
      );

      window._otaAssetName = data.httpOtaAsset;
      window._otaFreeSketch = data.httpOtaFreeBytes;

      checkForUpdate(data.radioversion);                
    },
    error: function () {
      alert(tr("fh"));
    },
  });
}

function confirmAndStartHttpOta(tag) {
  if (!tag) return;
  var asset = window._otaAssetName || ($("#httpOtaAsset").length ? $("#httpOtaAsset").text() : "") || "radiowecker-firmware.bin";
  var freeB = window._otaFreeSketch;
  var msg = tr("ota_ask") + " " + tag + "?\n\n";
  msg += tr("ota_rel") + "\n" + asset + "\n";
  if (typeof freeB === "number") msg += "\n" + tr("ota_free") + " " + freeB + " Byte\n";
  msg += "\n" + tr("ota_stop") + " " + tr("ota_iprog") + " " + tr("ota_reboot") + "\n\n";
  msg += tr("ota_web") + "\n\n" + tr("ota_go");
  if (!confirm(msg)) return;
  $.ajax({
    type: "POST",
    url: "/cmd/startHttpUpdate",
    contentType: "application/json; charset=utf-8",
    dataType: "json",
    data: JSON.stringify({ tag: tag }),
    success: function (r) {
      if (r && r.ok) {
        /* Kein Alert: Gerät startet sofort neu, Verbindungsabbruch wäre ohnehin der Normalfall. */
      }
    },
    error: function () {
      /* Kein Alert: nach Neustart bricht die Verbindung ab — das wirkt wie ein Fehler, ist aber erwartet. */
    }
  });
}

function checkForUpdate(currentVersion) {
  // Funktion zum Vergleichen von semantischen Versionen
  function compareVersions(v1, v2) {
    // Entferne das führende "v" und teile die Versionsnummern in ihre Bestandteile
    const v1Parts = v1.replace(/^v/, '').split('.').map(Number);
    const v2Parts = v2.replace(/^v/, '').split('.').map(Number);

    // Vergleiche die einzelnen Teile der Versionsnummern
    for (let i = 0; i < Math.max(v1Parts.length, v2Parts.length); i++) {
      const v1Part = v1Parts[i] || 0; // Standardwert 0, falls ein Teil fehlt
      const v2Part = v2Parts[i] || 0; // Standardwert 0, falls ein Teil fehlt
      if (v1Part > v2Part) return 1;  // Erste Version ist größer
      if (v1Part < v2Part) return -1; // Zweite Version ist größer
    }
    return 0; // Beide Versionen sind gleich
  }

  $.ajax({
    url: "https://api.github.com/repos/beabel/radiowecker/releases/latest",
    type: "GET",
    dataType: "json",
    success: function (data) {
      console.log(data);

      // Aktuellste Version von GitHub-API abrufen
      var latestVersion = data.tag_name;

      // Vergleich der aktuellen Version mit der neuesten Version
      var comparisonResult = compareVersions(currentVersion, latestVersion);

      if (comparisonResult < 0) {
        $("#githubVersion").html(
          '<i class="fas fa-exclamation-triangle" style="color: red;"></i> ' + tr("gh_new") + ' <strong>' + latestVersion + '</strong><br/>' +
          '<button type="button" id="btn_fw_update" style="margin-top:6px;">' + tr("gh_inst") + '</button> ' +
          '<a href="' + data.html_url + '" target="_blank">' + tr("gh_rel") + '</a>'
        );
        $("#btn_fw_update").off("click").on("click", function () {
          confirmAndStartHttpOta(latestVersion);
        });
      } else if (comparisonResult > 0) {
        // Wenn die aktuelle Version größer ist, zeige eine Meldung, dass diese neuer ist
        $("#githubVersion").html('<i class="fas fa-code" style="color: blue;"></i> ' + tr("gh_newer") + ' (' + currentVersion + ') ' + tr("gh_than") + ' (' + latestVersion + ').');
      } else {
        // Wenn die Versionen gleich sind, zeige an, dass die neueste Version verwendet wird
        $("#githubVersion").html('<i class="fas fa-check" style="color: green;"></i> ' + tr("gh_okv") + ' (' + currentVersion + ').');
      }
    },
    error: function () {
      // Zeige eine Fehlermeldung an, falls der AJAX-Request fehlschlägt
      $("#githubVersion").text(tr("gh_err"));
    }
  });
}

function updateGitHubInfo() {
  $.ajax({
    url: "https://api.github.com/repos/beabel/radiowecker",
    type: "GET",
    dataType: "json",
    success: function (data) {
      console.log(data);
      var updatedAt = new Date(data.updated_at);
      var day = updatedAt.getDate();
      var month = updatedAt.getMonth() + 1;
      var year = updatedAt.getFullYear();
      var hours = updatedAt.getHours();
      var minutes = updatedAt.getMinutes();

      // Führende Nullen hinzufügen, wenn nötig
      if (day < 10) { day = '0' + day; }
      if (month < 10) { month = '0' + month; }
      if (hours < 10) { hours = '0' + hours; }
      if (minutes < 10) { minutes = '0' + minutes; }

      var formattedDate = day + '.' + month + '.' + year + ' ' + hours + ':' + minutes;

      var html = '<div class="ui-state-highlight ui-corner-all">';
      html += '<p><a href="' + data.html_url + '" target="_blank"><i class="fab fa-github"></i>' + data.name + '</a></p>';      
      html += '<p><a href="' + data.html_url + '/stargazers" target="_blank"><i class="fas fa-star"></i>Stars: ' + data.stargazers_count + '</a> ';
      html += '<a href="' + data.html_url + '/wiki" target="_blank"><i class="fas fa-info"></i> Wiki</a> ';
      html += '<a href="' + data.html_url + '/discussions" target="_blank"><i class="fas fa-users"></i> Forum</a></p>';
      html += '<p>' + tr("fu") + ' ' + formattedDate + '</p>';
      html += '<p>' + tr("fl") + ' <i class="fas fa-heart"></i> by kunigunde</p>';
      html += '</div>';            
      $("#footer").html(html);
    },
    error: function () {
      $("#footer").html(tr("fgh"));
    }
  });
}


</script>
<style>
body {
    width:300px;
    background-color: silver;
}
/* Eine Zeile Tab-Icons in ~300px: kein Float, wenig Padding, gleichmäßige Aufteilung */
#tabs.ui-tabs .ui-tabs-nav {
  display: flex;
  flex-wrap: nowrap;
  width: 100%;
  box-sizing: border-box;
  padding: 2px 0 0;
  margin: 0;
}
#tabs.ui-tabs .ui-tabs-nav li {
  flex: 1 1 0;
  min-width: 0;
  float: none;
  list-style: none;
  margin: 0 1px 0 0;
  top: auto;
  padding: 0;
}
#tabs.ui-tabs .ui-tabs-nav li:last-child {
  margin-right: 0;
}
#tabs.ui-tabs .ui-tabs-nav li a {
  float: none;
  display: block;
  width: 100%;
  box-sizing: border-box;
  padding: 0.22em 0.05em;
  text-align: center;
  font-size: 1em;
  line-height: 1.15;
}
#tabs.ui-tabs .ui-tabs-nav li a i {
  font-size: 0.88em;
  vertical-align: middle;
}
#tabs.ui-tabs .ui-tabs-nav li.ui-tabs-active {
  margin-bottom: -1px;
  padding-bottom: 1px;
}
label { 
  display: inline-block;
  margin-top: 5px;
  width: 100%;
  float: left;  
}
button {
    margin: 3px;
    height: 2.0em;    
}
input {
  width:100%;
}

/* checkboxen Wochentage als "Button" */
.categorie{
  background-color: white;
  overflow: hidden;
  float: left;
}
.categorie label {
  float: left;
  line-height: 2.0em;
  width: 2.0em; 
  height: 2.0em;
  margin: 1px;
}
.categorie label span {
  text-align: center;
  padding: 2px 0;
  display: block;
  cursor: pointer;
  border: 1px solid silver;
}
.categorie label input {
  position: absolute;
  display: none;
}
.categorie input:checked + span {
    color: white; 
}
.categorie input:checked + span{background-color: #82D44E;}
/* Sprachzeile: globales label{float:left} + leeres Parent-div → Höhe 0, Tabs überdecken die Zeile */
.lang-bar {
  margin: 6px 0;
  clear: both;
  display: flex;
  flex-wrap: wrap;
  align-items: center;
  gap: 8px;
  box-sizing: border-box;
}
.lang-bar label {
  float: none;
  display: inline-block;
  width: auto;
  margin-top: 0;
  flex: 0 0 auto;
}
.lang-bar select {
  flex: 0 1 auto;
  min-width: 4.5em;
  max-width: 100%;
}
/* Kein float:left wie global bei label — sonst kollabiert die <p>-Höhe und der Slider liegt über dem Text */
#display label {
  float: none;
  display: block;
  width: 100%;
  margin-top: 0.5em;
  margin-bottom: 0.15em;
}
#display > p {
  margin: 0;
  clear: both;
}
#display .display-slider {
  clear: both;
  width: 100%;
  max-width: 320px;
  margin: 0.35em 0 0.85em 0;
  position: relative;
  z-index: 0;
}
/* Tab Wecker: Floats (.categorie) + letztes Element ggf. float → ohne BFC ragt Inhalt aus dem Tab */
#wecker {
  display: flow-root;
  box-sizing: border-box;
  max-width: 100%;
}
/* Tab Wecker: Wecklautstärke — sonst global label{float:left} + kollabierende <p> → Slider über dem Text */
#wecker > p {
  margin: 0;
  clear: both;
}
#wecker > p label {
  float: none;
  display: block;
  width: 100%;
  margin-top: 0.5em;
  margin-bottom: 0.15em;
}
#wecker .display-slider {
  clear: both;
  width: 100%;
  max-width: 100%;
  margin: 0.35em 0 0.85em 0;
  position: relative;
  z-index: 0;
}
#wecker .wecker-wake {
  margin: 0.5em 0 0.15em 0;
  clear: both;
}
/* Weckton Radio/Piep: kein float am Container (sonst Höhe des Tabs endet „zu früh“) */
#wecker .categorie.wecker-wake-opts {
  clear: both;
  float: none;
  width: 100%;
  max-width: 100%;
  margin: 0.15em 0 0.5em 0;
  display: flex;
  flex-wrap: wrap;
  gap: 4px;
  align-items: stretch;
  box-sizing: border-box;
}
#wecker .categorie.wecker-wake-opts label {
  float: none;
  flex: 1 1 calc(50% - 4px);
  min-width: 0;
  width: auto;
  max-width: 100%;
  height: 2.0em;
  line-height: 2.0em;
  margin: 0;
  box-sizing: border-box;
}
#wecker .categorie.wecker-wake-opts label span {
  box-sizing: border-box;
  width: 100%;
  min-width: 0;
  height: 100%;
  min-height: 2em;
  padding: 0 0.35em;
  line-height: 2.0em;
  white-space: nowrap;
  overflow: hidden;
  text-overflow: ellipsis;
  display: flex;
  align-items: center;
  justify-content: center;
}
#display .hint {
  font-size: 0.88em;
  opacity: 0.88;
  margin: 0 0 0.35em 0;
  max-width: 320px;
  clear: both;
}
#wecker .hint {
  font-size: 0.88em;
  opacity: 0.88;
  margin: 0 0 0.35em 0;
  max-width: 100%;
  clear: both;
}
#display .color-row {
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 6px;
  margin: 0.15em 0;
  max-width: 300px;
}
#display .color-row label {
  float: none;
  width: auto;
  flex: 1;
  margin: 0;
  font-size: 0.78em;
  line-height: 1.2;
}
#display input[type="color"] {
  width: 34px;
  height: 26px;
  padding: 0;
  border: 0;
  flex-shrink: 0;
  cursor: pointer;
}
#display hr.disp-sep { margin: 0.85em 0; border: 0; border-top: 1px solid #999; }
#display .help-tip-wrap,
#wecker .help-tip-wrap {
  display: inline-block;
  vertical-align: middle;
  max-width: 100%;
  margin-left: 0.2em;
}
#display .help-tip,
#wecker .help-tip {
  cursor: pointer;
  margin: 0;
  padding: 0.12em;
  opacity: 0.85;
  font-size: 0.82em;
  vertical-align: middle;
  display: inline-block;
  line-height: 1;
  color: #333;
  border: none;
  background: transparent;
  font-family: inherit;
  -webkit-tap-highlight-color: rgba(0,0,0,0.08);
}
#display .help-tip-wrap.is-open .help-tip,
#wecker .help-tip-wrap.is-open .help-tip { opacity: 1; }
#display .help-tip:focus,
#wecker .help-tip:focus {
  outline: 1px dotted #666;
  outline-offset: 2px;
}
#display .help-tip-panel,
#wecker .help-tip-panel {
  display: none;
  margin: 0.35em 0 0 0;
  padding: 0.45em 0.55em;
  background: #f2f2f2;
  border: 1px solid #bbb;
  border-radius: 4px;
  font-size: 0.88em;
  line-height: 1.38;
  font-weight: normal;
  color: #222;
  box-sizing: border-box;
  max-width: min(100%, 340px);
  text-align: left;
}
#display .help-tip-wrap.is-open .help-tip-panel,
#wecker .help-tip-wrap.is-open .help-tip-panel { display: block; }
</style>
</head>
<body>
<h1 data-i18n="title">Web Radiowecker</h1>
<div class="lang-bar">
<label for="ui_lang_sel"><span data-i18n="lang">Sprache</span></label>
<select id="ui_lang_sel" aria-label="UI language">
<option value="de">DE</option>
<option value="en">EN</option>
<option value="fr">FR</option>
<option value="ru">RU</option>
<option value="es">ES</option>
<option value="nl">NL</option>
</select>
</div>

<div id="tabs">
    <ul>
        <li><a href="#player"><i class="fa fa-music"></i></a></li>    
        <li><a href="#wecker"><i class="fa fa-clock"></i></a></li>
        <li><a href="#radio"><i class="fas fa-list-ol"></i></a></li>   
        <li><a href="#display"><i class="fas fa-sun"></i></a></li>
        <li><a href="#wlan"><i class="fa fa-gears"></i></a></li>
        <li><a href="#info"><i class="fas fa-question"></i></a></li>        
    </ul>
    <div id="player">
      <div align="center">
        <select id="switchStation" aria-label="Sender"></select>
        <br />
        <button id="btn_alarm" type="button"></button>
        <button id="btn_bwd" type="button"><i class="fa fa-step-backward"></i></button>
        <button id="btn_play" type="button"><i class="fas fa-play"></i></button>
        <button id="btn_fwd" type="button"><i class="fa fa-step-forward"></i></button>
        <button id="btn_stop" type="button" disabled><i class="fas fa-stop"></i></button>
        <button id="btn_sleep" type="button"><i class="fas fa-bed"></i></button>
        <br />
        
      </div>
      <table width="100%" style="background-color: black;">
        <tr>
          <td>
            <!-- Symbol Alarm ON/OFF -->
            <span id="alarmIcon"></span>
            <!-- AlarmText -->
            <span id="alarmTime"></span>
          </td>
          <td align="right" style="color: orange;">
            <!-- Sleep ON/OFF -->
            <span id="sleepIcon" style="color: orange;"></span>          
            <!-- WlanRssi -->
            <span id="WlanRSSI"></span>          
            <!-- WLAN Symbol -->
            <i class="fas fa-wifi" id="WlanSym"></i>          
          </td>
        </tr>
        <tr>
          <td colspan="2" align="center" style="color: orange; font-size: 80px; font-weight: bold;">
            <!-- Uhrzeit -->
            <span id="Time"></span>          
          </td>
        </tr>
        <tr>
          <td colspan="2" align="center" style="color: orange; font-weight: bold;">
            <!-- Datum -->
            <span id="Date"></span>            
          </td>
        </tr>
        <tr>
          <td colspan="2" align="center" style="color: yellow; font-weight: bold;">
            <!-- Radio Name -->
            <span id="radioStation"></span>         
          </td>
        </tr>
        <tr>
          <td colspan="2" align="center" style="color: white;">
            <!-- Radio Title -->
            <span id="radioTitle"></span>          
          </td>
        </tr>
        <tr>
          <td colspan="2" align="center"><div id="GainSlider"></div></td>
        </tr>
      </table>      
    </div>    
    <div id="wecker">
      <label><span data-i18n="w1">Weckzeit 1:</span>
          <input id="al0" type="time" />
      </label>
      <br />
      <div class="categorie days" align="center">
        <label>
            <input type="checkbox" id="al2"/>
            <span data-i18n="dow_mo">Mo</span>
        </label>
        <label>
            <input type="checkbox" id="al3"/>
            <span data-i18n="dow_di">Di</span>
        </label>
        <label>
            <input type="checkbox" id="al4"/>
            <span data-i18n="dow_mi">Mi</span>
        </label>
        <label>
            <input type="checkbox" id="al5"/>
            <span data-i18n="dow_do">Do</span>
        </label>
        <label>
            <input type="checkbox" id="al6"/>
            <span data-i18n="dow_fr">Fr</span>
        </label>
        <label>
            <input type="checkbox" id="al7"/>
            <span data-i18n="dow_sa">Sa</span>
        </label>
        <label>
            <input id="al1" type="checkbox"/>
            <span data-i18n="dow_so">So</span>
        </label>
      </div>
      <br />
      <label><span data-i18n="w2">Weckzeit 2:</span>
          <input id="al8" type="time"/>
      </label>
      <br />
      <div class="categorie days"  align="center">
        <label>
            <input type="checkbox" id="al10"/>
            <span data-i18n="dow_mo">Mo</span>
        </label>
        <label>
            <input type="checkbox" id="al11"/>
            <span data-i18n="dow_di">Di</span>
        </label>
        <label>
            <input type="checkbox" id="al12"/>
            <span data-i18n="dow_mi">Mi</span>
        </label>
        <label>
            <input type="checkbox" id="al13"/>
            <span data-i18n="dow_do">Do</span>
        </label>
        <label>
            <input type="checkbox" id="al14"/>
            <span data-i18n="dow_fr">Fr</span>
        </label>
        <label>
            <input type="checkbox" id="al15"/>
            <span data-i18n="dow_sa">Sa</span>
        </label>
        <label>
            <input type="checkbox" id="al9"/>
            <span data-i18n="dow_so">So</span>
        </label>
      </div>
      <br />
      <div align="center">
        <button id="btn_savealarm" type="button" data-i18n="save">Speichern</button>
        <button id="btn_cancelalarm" type="button" data-i18n="cancel">Rückgängig</button>
      </div>
      <br />
      <p><label><span data-i18n="lbl_alarmgain">Wecklautstärke (%):</span> <strong id="disp_alarmgain_val">0%</strong></label></p>
      <div id="slider_alarmgain" class="display-slider"></div>
      <p><label><span data-i18n="lbl_asnooze">Wecker-Schlummer (Min.):</span><span class="help-tip-wrap"><button type="button" class="help-tip" data-i18n-title="help_snooze_hw" aria-expanded="false" aria-controls="help_panel_snooze" id="help_btn_snooze"><i class="fas fa-circle-question" aria-hidden="true"></i></button><span class="help-tip-panel" id="help_panel_snooze" role="tooltip" aria-hidden="true"></span></span> <strong id="disp_asnooze_val">0</strong></label></p>
      <p class="hint"><span data-i18n="hint_asnooze"></span></p>
      <div id="slider_asnooze" class="display-slider"></div>
      <p class="wecker-wake"><span data-i18n="lbl_alarmwake">Weckton:</span></p>
      <div class="categorie wecker-wake-opts" align="center">
        <label>
          <input type="radio" name="alm_wake" id="alm_wake0" value="0" checked="checked" />
          <span data-i18n="wake_radio">Radio</span>
        </label>
        <label>
          <input type="radio" name="alm_wake" id="alm_wake1" value="1" />
          <span data-i18n="wake_beep">Piep</span>
        </label>
      </div>
    </div>
    <div id="radio">
      <label><span data-i18n="list">Liste:</span>
        <select id="stationlist"></select>
      </label>
      <br />
      <label><span data-i18n="name">Name:</span>
          <input id="name_input"/>
      </label>
      <br />
      <label><span data-i18n="url">URL:</span>
          <input id="url_input"/>
      </label>
      <br />
      <label><span data-i18n="use">Verwenden:</span>
          <input id="enable_input" type="checkbox">
      </label>
      <br />
      <label><span data-i18n="pos">Position:</span>
          <input id="pos_input" type="number" step="1" min="1" max="99" size="2"/>
      </label>
      <br />
      <div align="center">
        <button id="btn_test" type="button" data-i18n="btest">Testen</button>
        <button id="btn_updt" type="button" data-i18n="bchange">Ändern</button>
      </div>
    </div>
    <div id="display">
      <p><label><span data-i18n="lbl_bright">Helligkeit (%):</span><span class="help-tip-wrap"><button type="button" class="help-tip" data-i18n-title="help_ldr" aria-expanded="false" aria-controls="help_panel_ldr" id="help_btn_ldr"><i class="fas fa-circle-question" aria-hidden="true"></i></button><span class="help-tip-panel" id="help_panel_ldr" role="tooltip" aria-hidden="true"></span></span> <strong id="disp_bright_val">0</strong></label></p>
      <p class="hint"><span data-i18n="hint_bright"></span></p>
      <div id="slider_bright" class="display-slider"></div>
      <p><label><span data-i18n="lbl_sleep">Einschlafzeit (Min.):</span> <strong id="disp_sleep_val">0</strong></label></p>
      <div id="slider_sleep" class="display-slider"></div>
      <hr class="disp-sep" />
      <p><strong data-i18n="colors_title">Startbildschirm-Farben</strong></p>
      <p class="hint"><span data-i18n="colors_hint"></span></p>
      <div id="start_color_inputs">
        <div class="color-row"><label for="col_bg" data-i18n="c_bg">Hintergrund</label><input type="color" id="col_bg" /></div>
        <div class="color-row"><label for="col_ip" data-i18n="c_ip">IP</label><input type="color" id="col_ip" /></div>
        <div class="color-row"><label for="col_sleep" data-i18n="c_sleep">Schlaf</label><input type="color" id="col_sleep" /></div>
        <div class="color-row"><label for="col_alarm" data-i18n="c_alarm">Wecker</label><input type="color" id="col_alarm" /></div>
        <div class="color-row"><label for="col_sFill" data-i18n="c_sFill">Slider</label><input type="color" id="col_sFill" /></div>
        <div class="color-row"><label for="col_sBg" data-i18n="c_sBg">Slider-BG</label><input type="color" id="col_sBg" /></div>
        <div class="color-row"><label for="col_sBd" data-i18n="c_sBd">Slider-Rahmen</label><input type="color" id="col_sBd" /></div>
        <div class="color-row"><label for="col_date" data-i18n="c_date">Datum</label><input type="color" id="col_date" /></div>
        <div class="color-row"><label for="col_time" data-i18n="c_time">Uhrzeit</label><input type="color" id="col_time" /></div>
        <div class="color-row"><label for="col_boxBg" data-i18n="c_boxBg">Kästen</label><input type="color" id="col_boxBg" /></div>
        <div class="color-row"><label for="col_boxBd" data-i18n="c_boxBd">Kasten-Rahmen</label><input type="color" id="col_boxBd" /></div>
        <div class="color-row"><label for="col_stName" data-i18n="c_stName">Sender</label><input type="color" id="col_stName" /></div>
        <div class="color-row"><label for="col_stTitle" data-i18n="c_stTitle">Titel</label><input type="color" id="col_stTitle" /></div>
      </div>
      <div align="center" style="margin-top:8px">
        <button type="button" id="btn_colors_save" data-i18n="colors_save">Speichern</button>
        <button type="button" id="btn_colors_reset" data-i18n="colors_reset">Werksfarben</button>
      </div>
    </div>
    <div id="wlan">
      <div id="accordion">
        <h3><i class="fa fa-wifi"></i> <span data-i18n="acc_wlan">Wlan</span></h3>
        <div>
          <p>
            <label>SSID:
              <input id="ssid_input" />
            </label>
            <br />
            <label>PKEY:
              <input id="pkey_input" type="password"/>
            </label>
            <br />
          </p>
        </div>
        <h3><i class="fa fa-business-time"></i> <span data-i18n="acc_ntp">NTP</span></h3>
        <div>
          <p>
            <label>NTP:
              <input id="ntp_input" />
            </label>
            <br />
          </p>
        </div>
        <h3><i class="fa fa-umbrella"></i> <span data-i18n="acc_wx">Wetter</span></h3>
        <div>
          <p>
            <!-- ################# Wetter -------------- -->
            <label>TIME_ZONE_IANA:
              <input id="TIME_ZONE_IANA_input" name="TIME_ZONE_IANA" />
            </label>
            <br />
            <label>Latitude:
              <input id="LATITUDE_input" name="LATITUDE" type="number" step="0.000001" min="-90" max="90" />
            </label>
            <br />
            <label>Longitude:
              <input id="LONGITUDE_input" name="LONGITUDE" type="number" step="0.000001" min="-180" max="180" />
            </label>
            <br />
          </p>
        </div>
      </div>
      <hr />      
      <div align="center">
        <button id="btn_save" type="button" data-i18n="bsave">Speichern</button>
        <button id="btn_reset" type="button" data-i18n="brestart">Neustart</button>
        <br />
        <button id="btn_restore" type="button" data-i18n="brestore">Senderliste Reset</button>
      </div>
    </div>
    <div id="info">
      <label for="radioversion"><span data-i18n="lbl_ver">Version:</span>
        <span id="radioversion"></span><br />
        <span id="githubVersion"></span>
        <span id="httpOtaAsset" style="display:none"></span>
        <span id="httpOtaFreeBytes" style="display:none"></span>
      </label>
      <hr />
      
      <label for="BarContainer"><span data-i18n="lbl_heap">HEAP:</span>
      </label>
      <div id="heapprogressbar"></div>
      <div id="heapBarText"></div>
      <br />
      <label for="BarContainer"><span data-i18n="lbl_sk">SKETCH:</span>
      </label>
      <div id="sketchprogressbar"></div>
      <div id="sketchBarText"></div>
      <hr />
      <label for="ESP_INFO_CHIP_getChipModel"><span data-i18n="lbl_chip">ChipModel:</span>
        <span id="ESP_INFO_CHIP_getChipModel"></span>
      </label>
      <hr />
      <div id="footer" class="ui-widget">
        <!-- Hier wird der Footer mit Informationen zum GitHub-Repository angezeigt -->
      </div>
      <hr />
      <div id="sponsor" class="ui-widget">
        <div class="ui-state-error ui-corner-all">
          <span data-i18n="sp">Sponsor this Project:</span> 
          <a href="https://buymeacoffee.com/kunigunde" target="_blank"><img width="16" height="16" class="octicon rounded-2 d-block" alt="buy_me_a_coffee" src="https://github.githubassets.com/assets/buy_me_a_coffee-63ed78263f6e.svg"></a>
          <a href="https://www.paypal.com/paypalme/pleissa" target="_blank"><i class="fa-brands fa-paypal"></i></a>
          <a href="https://thanks.dev/beabel" target="_blank"><img width="16" height="16" class="octicon rounded-2 d-block" alt="thanks_dev" src="https://github.githubassets.com/assets/thanks_dev-0fe87803d1ed.svg"></a>
        </div>
      </div>
    </div>    
</div>
</body>
</html>
)=====";

//*****************************************************************************************************************
const char CONFIG_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
<meta http-equiv='content-type' content='text/html; charset=UTF-8'>
<meta name='viewport' content='width=320' />
<style>
body {
  width:300px;
  font-family:arial;
}
button {
  width:110px;
  background-color:silver;
  border-radius:10px;
}
.theme {
  background-color:cadetblue;
  padding:10px;
  border-radius: 10px;
  border: 2px solid silver;
}
.confpane {
  width:280px;
  height:75px;
}
.txtinput {
  position:absolute;
  left:90px;
  width:200px;
}
.label {
  color:white;
  font-family:arial;
}
</style>
</head>
<body>
<h1 style="text-align:center;">Internet Radio</h1>
<form>
    <div class="confpane theme">
        <div>
            <span class="label">SSID:</span>
            <input name="conf_ssid" class="txtinput"/>
        </div>
        <div style="padding-top:10px;">
            <span class="label">PKEY:</span>
            <input name="conf_pkey" class="txtinput" type="password"/>
        </div>
        <div style="padding-top:10px;text-align:center">
            <button type="submit" name="reset">Neustart</button>
        </div>
    </div>
</form>
</body>
</html>
)=====";
