//html for web pages
const char OPTION_entry[] PROGMEM = 
"<option value='%i' %s>%s %s</option>";

const char MAIN_page[] PROGMEM = R"=====(
<html>
<head>
<meta http-equiv='content-type' content='text/html; charset=UTF-8'>
<meta name='viewport' content='width=320' />
<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.5.1/jquery.min.js"></script>
<link rel="stylesheet" href="//code.jquery.com/ui/1.12.1/themes/base/jquery-ui.min.css">
<script src="https://code.jquery.com/ui/1.12.1/jquery-ui.min.js"></script>
<link rel="stylesheet" href="//use.fontawesome.com/releases/v5.0.7/css/all.css">
<script>
$(document).ready(function() {
  $("#tabs").tabs();
  //Standard Stuff
  getAll();
  $("#btn_save").click(saveSSID);
  $("#btn_reset").click(restartHost);
  $("#btn_test").click(testStation);
  $("#btn_updt").click(updateStation);
  $("#btn_restore").click(restoreStations);
  $("#btn_savealarm").click(setAlarms);
  $("#btn_cancelalarm").click(getAlarms);
  $("#stationlist").change(getStation);
  // Player Tab ######################
  $("#btn_play").click(startPlay);// senden Play zum ESP
  $("#btn_stop").click(stopPlay);// senden Stop zum ESP
  $("#btn_alarm").click(btnAlarm);// toggle Alarm zum ESP
  $("#btn_sleep").click(startSleep);// toggle Sleep zum ESP 
  GainSlider();// senden des Sliders zum ESP

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
    getSSID();
    getStationList();
    getAlarms();
}

function getStationList() {
    $.ajax({
        type:"GET",
        url:"/cmd/stations",
        data:{},
        success: function(data){
            $("#stationlist").html(data);
            getStation();
        }
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
            alert("Mit OK beenden Sie den Test");
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
      data:{"ssid":$("#ssid_input").val(),"pkey":$("#pkey_input").val(),"ntp":$("#ntp_input").val()},
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
    });
}
function stopPlay() {
    $.ajax({
        type:"POST",
        url:"/cmd/stopPlay",
        data:{},
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
      // Zeit + Datum
      updateCurrentStatusDateTime(data.Date, data.Time);
      // WLAN
      updateCurrentStatusWlan(data.Rssi);
      // Sleep Timer
      updateCurrentStatusSleep(data.Sleep);
      //console.log(data);
    },
    error: function () {
      alert(data);
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
    margin: 5px;
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
</style>
</head>
<body>
<h1>Web Radiowecker</h1>

<div id="tabs">
    <ul>
        <li><a href="#player"><i class="fa fa-music"></i></a></li>    
        <li><a href="#wecker"><i class="fa fa-clock"></i></a></li>
        <li><a href="#radio"><i class="fas fa-list-ol"></i></a></li>   
        <li><a href="#wlan"><i class="fa fa-wifi"></i></a></li>
    </ul>
    <div id="player">
      <div align="center">
        <button id="btn_alarm" type="button"></button>
        <button id="btn_play" type="button"><i class="fas fa-play"></i></button>
        <button id="btn_stop" type="button"><i class="fas fa-stop"></i></button>
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
      <label>Weckzeit 1:
          <input id="al0" type="time" />
      </label>
      <br />
      <div class="categorie days" align="center">
        <label>
            <input type="checkbox" id="al2"/>
            <span>Mo</span>
        </label>
        <label>
            <input type="checkbox" id="al3"/>
            <span>Di</span>
        </label>
        <label>
            <input type="checkbox" id="al4"/>
            <span>Mi</span>
        </label>
        <label>
            <input type="checkbox" id="al5"/>
            <span>Do</span>
        </label>
        <label>
            <input type="checkbox" id="al6"/>
            <span>Fr</span>
        </label>
        <label>
            <input type="checkbox" id="al7"/>
            <span>Sa</span>
        </label>
        <label>
            <input id="al1" type="checkbox"/>
            <span>So</span>
        </label>
      </div>
      <br />
      <label>Weckzeit 2:
          <input id="al8" type="time"/>
      </label>
      <br />
      <div class="categorie days"  align="center">
        <label>
            <input type="checkbox" id="al10"/>
            <span>Mo</span>
        </label>
        <label>
            <input type="checkbox" id="al11"/>
            <span>Di</span>
        </label>
        <label>
            <input type="checkbox" id="al12"/>
            <span>Mi</span>
        </label>
        <label>
            <input type="checkbox" id="al13"/>
            <span>Do</span>
        </label>
        <label>
            <input type="checkbox" id="al14"/>
            <span>Fr</span>
        </label>
        <label>
            <input type="checkbox" id="al15"/>
            <span>Sa</span>
        </label>
        <label>
            <input type="checkbox" id="al9"/>
            <span>So</span>
        </label>
      </div>
      <br />
      <div align="center">
        <button id="btn_savealarm" type="button">Speichern</button>
        <button id="btn_cancelalarm" type="button">Rückgängig</button>
      </div>      
    </div>
    <div id="radio">
      <label>Liste:
        <select id="stationlist"></select>
      </label>
      <br />
      <label>Name:
          <input id="name_input"/>
      </label>
      <br />
      <label>URL:
          <input id="url_input"/>
      </label>
      <br />
      <label>Verwenden:
          <input id="enable_input" type="checkbox">
      </label>
      <br />
      <label>Position:
          <input id="pos_input" type="number" step="1" min="1" max="99" size="2"/>
      </label>
      <br />
      <div align="center">
        <button id="btn_test" type="button">Testen</button>
        <button id="btn_updt" type="button">Ändern</button>
      </div>
    </div>         
    <div id="wlan">
      <label>SSID:
        <input id="ssid_input" />
      </label>
      <br />
      <label>PKEY:
        <input id="pkey_input" type="password"/>
      </label>
      <br />
      <label>NTP:
        <input id="ntp_input" />
      </label>
      <br />
      <div align="center">
        <button id="btn_save" type="button">Speichern</button>
        <button id="btn_reset" type="button">Neustart</button>
        <br />
        <button id="btn_restore" type="button">Senderliste Reset</button>
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
