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
 de: {lang:"Sprache",title:"Web Radiowecker",w1:"Weckzeit 1:",w2:"Weckzeit 2:",save:"Speichern",cancel:"Rückgängig",list:"Liste:",name:"Name:",url:"URL:",use:"Verwenden:",pos:"Position:",btest:"Testen",bchange:"Ändern",acc_wlan:"Wlan",acc_ntp:"NTP",acc_wx:"Wetter",bsave:"Speichern",brestart:"Neustart",brestore:"Senderliste Reset",lbl_ver:"Version:",lbl_heap:"HEAP:",lbl_sk:"SKETCH:",lbl_chip:"ChipModel:",fh:"Fehler beim Abrufen der Daten.",fe:"Fehler beim Laden der Daten.",gh_err:"Fehler beim Überprüfen auf Updates.",dow_mo:"Mo",dow_di:"Di",dow_mi:"Mi",dow_do:"Do",dow_fr:"Fr",dow_sa:"Sa",dow_so:"So",testend:"Mit OK beenden Sie den Test",ota_ask:"Firmware auf",ota_rel:"Im GitHub-Release muss die Datei angehängt sein:",ota_free:"Freier OTA-Slot (ungefähr):",ota_stop:"Das Radio wird gestoppt;",ota_iprog:"Der Fortschritt erscheint auf dem Display.",ota_reboot:"Anschließend startet das Gerät neu.",ota_web:"Diese Webseite verliert danach die Verbindung zum Gerät (erwartet) — es erscheint kein weiteres Meldungsfenster.",ota_go:"Fortfahren?",gh_new:"Neue Version:",gh_inst:"Per Web installieren…",gh_rel:"Release-Seite",gh_newer:"Du verwendest eine neuere Version",gh_than:"als die neueste offizielle Version",gh_okv:"Du verwendest bereits die neueste Version",heap_u:"Belegt:",heap_f:"Frei:",heap_t:"Heap gesamt:",heap_m:"max. Block:",fw_l:"Firmware:",fw_v:"von",fw_u:"frei für nächstes Update:",fu:"Letztes Update:",fl:"Created with",sp:"Sponsor this Project:",fgh:"Fehler beim Abrufen der GitHub-Informationen."},
 en: {lang:"Language",title:"Web clock radio",w1:"Alarm 1:",w2:"Alarm 2:",save:"Save",cancel:"Undo",list:"List:",name:"Name:",url:"URL:",use:"Use:",pos:"Position:",btest:"Test",bchange:"Apply",acc_wlan:"Wi‑Fi",acc_ntp:"NTP",acc_wx:"Weather",bsave:"Save",brestart:"Restart",brestore:"Reset station list",lbl_ver:"Version:",lbl_heap:"HEAP:",lbl_sk:"SKETCH:",lbl_chip:"Chip model:",fh:"Failed to fetch data.",fe:"Failed to load data.",gh_err:"Update check failed.",dow_mo:"Mo",dow_di:"Tu",dow_mi:"We",dow_do:"Th",dow_fr:"Fr",dow_sa:"Sa",dow_so:"Su",testend:"Press OK to end the test",ota_ask:"Install firmware",ota_rel:"The GitHub release must include file:",ota_free:"Approx. free OTA slot:",ota_stop:"Playback will stop;",ota_iprog:"progress is shown on the device.",ota_reboot:"Then the device reboots.",ota_web:"This page will lose connection (expected).",ota_go:"Continue?",gh_new:"New version:",gh_inst:"Install via web…",gh_rel:"Release page",gh_newer:"You are running a newer version",gh_than:"than the latest official release",gh_okv:"You are on the latest version",heap_u:"Used:",heap_f:"Free:",heap_t:"Heap total:",heap_m:"max block:",fw_l:"Firmware:",fw_v:"of",fw_u:"free for next update:",fu:"Last update:",fl:"Created with",sp:"Sponsor this project:",fgh:"Failed to fetch GitHub info."},
 fr: {lang:"Langue",title:"Radio-réveil web",w1:"Alarme 1 :",w2:"Alarme 2 :",save:"Enregistrer",cancel:"Annuler",list:"Liste :",name:"Nom :",url:"URL :",use:"Actif :",pos:"Position :",btest:"Tester",bchange:"Modifier",acc_wlan:"Wi‑Fi",acc_ntp:"NTP",acc_wx:"Météo",bsave:"Enregistrer",brestart:"Redémarrer",brestore:"Réinit. liste stations",lbl_ver:"Version :",lbl_heap:"TAS :",lbl_sk:"SKETCH :",lbl_chip:"Modèle puce :",fh:"Échec de lecture des données.",fe:"Échec du chargement.",gh_err:"Échec vérif. mises à jour.",dow_mo:"Lu",dow_di:"Ma",dow_mi:"Me",dow_do:"Je",dow_fr:"Ve",dow_sa:"Sa",dow_so:"Di",testend:"OK pour terminer le test",ota_ask:"Installer le firmware",ota_rel:"Le release GitHub doit contenir :",ota_free:"Espace OTA libre (approx.) :",ota_stop:"La radio s'arrête ;",ota_iprog:"la progression s'affiche sur l'appareil.",ota_reboot:"Puis redémarrage.",ota_web:"Cette page perdra la connexion (normal).",ota_go:"Continuer ?",gh_new:"Nouvelle version :",gh_inst:"Installer via le web…",gh_rel:"Page release",gh_newer:"Version plus récente que",gh_than:"la dernière version officielle",gh_okv:"Déjà la dernière version",heap_u:"Utilisé :",heap_f:"Libre :",heap_t:"Tas total :",heap_m:"bloc max :",fw_l:"Firmware :",fw_v:"sur",fw_u:"libre pour prochaine MAJ :",fu:"Dernière MAJ :",fl:"Créé avec",sp:"Soutenir le projet :",fgh:"Échec infos GitHub."},
 ru: {lang:"Язык",title:"Веб‑радиобудильник",w1:"Будильник 1:",w2:"Будильник 2:",save:"Сохранить",cancel:"Отменить",list:"Список:",name:"Имя:",url:"URL:",use:"Вкл.:",pos:"Позиция:",btest:"Тест",bchange:"Изменить",acc_wlan:"Wi‑Fi",acc_ntp:"NTP",acc_wx:"Погода",bsave:"Сохранить",brestart:"Перезагрузка",brestore:"Сброс списка станций",lbl_ver:"Версия:",lbl_heap:"КУЧА:",lbl_sk:"ПРОШИВКА:",lbl_chip:"Чип:",fh:"Ошибка получения данных.",fe:"Ошибка загрузки.",gh_err:"Ошибка проверки обновлений.",dow_mo:"Пн",dow_di:"Вт",dow_mi:"Ср",dow_do:"Чт",dow_fr:"Пт",dow_sa:"Сб",dow_so:"Вс",testend:"Нажмите OK, чтобы завершить тест",ota_ask:"Установить прошивку",ota_rel:"В релизе GitHub должен быть файл:",ota_free:"Свободно под OTA (прибл.):",ota_stop:"Радио остановится;",ota_iprog:"прогресс на экране устройства.",ota_reboot:"Затем перезагрузка.",ota_web:"Страница потеряет связь (норма).",ota_go:"Продолжить?",gh_new:"Новая версия:",gh_inst:"Установить через веб…",gh_rel:"Страница релиза",gh_newer:"У вас новее, чем",gh_than:"последний официальный релиз",gh_okv:"Уже последняя версия",heap_u:"Занято:",heap_f:"Свободно:",heap_t:"Куча всего:",heap_m:"макс. блок:",fw_l:"Прошивка:",fw_v:"из",fw_u:"свободно для обновления:",fu:"Последнее обновление:",fl:"Создано с",sp:"Поддержать проект:",fgh:"Ошибка GitHub."},
 es: {lang:"Idioma",title:"Radio despertador web",w1:"Alarma 1:",w2:"Alarma 2:",save:"Guardar",cancel:"Deshacer",list:"Lista:",name:"Nombre:",url:"URL:",use:"Usar:",pos:"Posición:",btest:"Probar",bchange:"Aplicar",acc_wlan:"Wi‑Fi",acc_ntp:"NTP",acc_wx:"Tiempo",bsave:"Guardar",brestart:"Reiniciar",brestore:"Restablecer lista de emisoras",lbl_ver:"Versión:",lbl_heap:"HEAP:",lbl_sk:"SKETCH:",lbl_chip:"Modelo de chip:",fh:"Error al obtener los datos.",fe:"Error al cargar los datos.",gh_err:"Error al comprobar actualizaciones.",dow_mo:"Lu",dow_di:"Ma",dow_mi:"Mi",dow_do:"Ju",dow_fr:"Vi",dow_sa:"Sá",dow_so:"Do",testend:"Pulse OK para finalizar la prueba",ota_ask:"Instalar firmware",ota_rel:"El release de GitHub debe incluir el archivo:",ota_free:"Espacio OTA libre (aprox.):",ota_stop:"Se detiene la reproducción;",ota_iprog:"el progreso se muestra en el dispositivo.",ota_reboot:"Luego el dispositivo se reinicia.",ota_web:"Esta página perderá la conexión (esperado).",ota_go:"¿Continuar?",gh_new:"Nueva versión:",gh_inst:"Instalar vía web…",gh_rel:"Página del release",gh_newer:"Tiene una versión más reciente",gh_than:"que el último release oficial",gh_okv:"Ya tiene la última versión",heap_u:"Usado:",heap_f:"Libre:",heap_t:"Montón total:",heap_m:"bloque máx.:",fw_l:"Firmware:",fw_v:"de",fw_u:"libre para el próximo update:",fu:"Última actualización:",fl:"Creado con",sp:"Patrocinar el proyecto:",fgh:"Error al obtener GitHub."},
 nl: {lang:"Taal",title:"Webklokradio",w1:"Wekker 1:",w2:"Wekker 2:",save:"Opslaan",cancel:"Ongedaan maken",list:"Lijst:",name:"Naam:",url:"URL:",use:"Gebruik:",pos:"Positie:",btest:"Test",bchange:"Toepassen",acc_wlan:"Wi‑Fi",acc_ntp:"NTP",acc_wx:"Weer",bsave:"Opslaan",brestart:"Herstarten",brestore:"Zenderlijst resetten",lbl_ver:"Versie:",lbl_heap:"HEAP:",lbl_sk:"SKETCH:",lbl_chip:"Chipmodel:",fh:"Gegevens ophalen mislukt.",fe:"Laden mislukt.",gh_err:"Updatecontrole mislukt.",dow_mo:"Ma",dow_di:"Di",dow_mi:"Wo",dow_do:"Do",dow_fr:"Vr",dow_sa:"Za",dow_so:"Zo",testend:"Druk op OK om de test te beëindigen",ota_ask:"Firmware installeren",ota_rel:"De GitHub-release moet het bestand bevatten:",ota_free:"Vrij OTA-blok (ca.):",ota_stop:"Afspelen stopt;",ota_iprog:"voortgang op het apparaat.",ota_reboot:"Daarna start het apparaat opnieuw.",ota_web:"Deze pagina verliest de verbinding (verwacht).",ota_go:"Doorgaan?",gh_new:"Nieuwe versie:",gh_inst:"Installeren via web…",gh_rel:"Releasepagina",gh_newer:"U heeft een nieuwere versie",gh_than:"dan de laatste officiële release",gh_okv:"U heeft al de nieuwste versie",heap_u:"Bezet:",heap_f:"Vrij:",heap_t:"Heap totaal:",heap_m:"max. blok:",fw_l:"Firmware:",fw_v:"van",fw_u:"vrij voor volgende update:",fu:"Laatste update:",fl:"Gemaakt met",sp:"Project steunen:",fgh:"GitHub-info ophalen mislukt."}
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
  $("#btn_bwd").click(beforeStation);// toggle beforeStation zum ESP 
  $("#btn_fwd").click(nextStation);// toggle nextStation zum ESP 
  $("#switchStation").on("change", selectStationFromDropdown);

  // Zeugs um die aktuellen Daten des ESP regelmäßig zu erhalten um halbwegs sinvolle Anzeigen zu sehen
  $("#tabs").tabs({
    activate: function(event, ui) {
      // Überprüfen, ob das aktivierte Tab die ID "#player" hat
      if (ui.newTab.find("a").attr("href") === "#player") {
        // starten Intervall, um die Daten alle x Sekunden abzurufen
        var intervalId = setInterval(updateCurrentStatus, 5000); // alle 5 Sekunden
        ui.newPanel.data("intervalId", intervalId); // Speichern der Intervall-ID im Tab-Panel-Daten
      } else {
        // Wenn ein anderes Tab aktiv ist, beenden des Intervall
        var intervalId = ui.oldPanel.data("intervalId");
        if (intervalId) {
          clearInterval(intervalId);
        }
      }
    }
  });

  if ($("#tabs").tabs("option", "active") === 0) { // 0 entspricht dem Index von "#player"
    updateCurrentStatus();
    var intervalId = setInterval(updateCurrentStatus, 3000); // Starten des Intervall
    $("#player").data("intervalId", intervalId);
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
    }).fail(function() {
      applyWebLang("de");
      getSSID();
      getStationList();
      getAlarms();
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

      // Heap: getFreeHeap = frei, Differenz = belegt (vorher fälschlich „used“ für freeHeap)
      var heapSize = data.ESP_INFO.HEAP.getHeapSize;
      var freeHeap = data.ESP_INFO.HEAP.getFreeHeap;
      var usedHeap = heapSize - freeHeap;
      var heapUsedPct = heapSize ? (usedHeap / heapSize) * 100 : 0;
      var heapFreePct = heapSize ? (freeHeap / heapSize) * 100 : 0;
      $( "#heapprogressbar" ).progressbar({
        value: heapUsedPct
      });
      var heapTxt = tr("heap_u") + " " + usedHeap + " B (" + heapUsedPct.toFixed(2) + "%) · " + tr("heap_f") + " " + freeHeap + " B (" + heapFreePct.toFixed(2) + "%) · " + tr("heap_t") + " " + heapSize + " B";
      var maxAlloc = data.ESP_INFO.HEAP.getMaxAllocHeap;
      if (typeof maxAlloc === "number") {
        heapTxt += " · " + tr("heap_m") + " " + maxAlloc + " B";
      }
      $("#heapBarText").text(heapTxt);

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
