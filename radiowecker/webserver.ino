#include <WebServer.h>
//home page and templatefor options
#include "index.h"

WebServer server(80);

//setup request distribution
void setup_webserver() {
  //server.enableCORS(); 
  server.on("/",handleRoot);
  //handle ajax commands
  server.on("/cmd/stations",sendStations);
  server.on("/cmd/restorestations",restoreStations);
  server.on("/cmd/restart",restart);
  server.on("/cmd/setaccess",setAccessData);
  server.on("/cmd/getaccess",getAccessData);
  server.on("/cmd/getalarms",getAlarms);
  server.on("/cmd/setalarms",setAlarms);
  server.on("/cmd/getstation",getStationData);
  server.on("/cmd/setstation",setStationData);
  server.on("/cmd/teststation",testStation);
  server.on("/cmd/endtest",endTest);
  //start webserver
  server.begin();
}

//called from main loop
void webserver_loop() {
    server.handleClient();
}

//handle root request
void handleRoot() {
  if (connected) {
    //if connected send the main page
    server.send(200,"text/html",MAIN_page);
  } else {
    //if not connected we use the config page without ajax
    //we get parameters as arguments in the HTML-request
    Serial.println("Got config:");
    uint8_t a = server.args();
    Serial.print(a); Serial.println(" Arguments");
    for (uint8_t i = 0; i<a; i++)  Serial.println(server.arg(i));
    if (server.hasArg("conf_ssid")) {
      pref.putString("ssid",server.arg("conf_ssid")); //save SSID in the preferences
      Serial.println(server.arg("conf_ssid"));
    }
    if (server.hasArg("conf_pkey")) {
      pref.putString("pkey",server.arg("conf_pkey")); // save PKEY in the preferences
      Serial.println(server.arg("conf_pkey"));
    }
    if (server.hasArg("reset")) {
      Serial.println("Restart!"); //initiate a restart
      ESP.restart();
    }
    server.send(200,"text/html",CONFIG_page); //send the config page to the client
  }

}

//AJAX command /cmd/stations
void sendStations() {
  char * s;
  char * sel;
  char buf[100];
  //prepare HTML response
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, "text/html", "");
  //send all entries as an option list
  for (uint8_t i = 0; i<STATIONS; i++) {
    //add selected if the station is tthe current one
    sel =  "";
    if (i == actStation) sel = "selected";
    //set a black dot in front of station name if active
    s = "&#x2002;";
    if (stationlist[i].enabled) s="&#x25cf;";
    //create the option entry with the template
    sprintf(buf,OPTION_entry,i,sel,s,stationlist[i].name);
    server.sendContent(buf);
  }
  
}

//AJAX command /cmd/setaccess
void setAccessData() {
  //the command expects three arguments
  //access data will be saved in preferences
  if (server.hasArg("ssid")) {
    ssid = server.arg("ssid");
    pref.putString("ssid",ssid);
  }
  if (server.hasArg("pkey")) {
    pkey = server.arg("pkey");
    pref.putString("pkey",pkey);
  }
  if (server.hasArg("ntp")) {
    ntp = server.arg("ntp");
    pref.putString("ntp",ntp);
  }
  //respond with OK
  server.send(200,"text/plain","OK");
}

//AJAX command /cmd/getaccess
void getAccessData() {
  //send access data separated by new line
  String msg = String(ssid) + "\n" + String(pkey) + "\n" + String(ntp);
  //respond with access data
  server.send(200,"text/plain",msg);
}

//AJAX command /cmd/getalarms
void getAlarms() {
  char buf[45];
  uint8_t h,m,mask,i;
  h = alarm1 /60;
  m = alarm1 % 60;
  sprintf(buf,"%02i:%02i\n",h,m);
  h = alarm2 /60;
  m = alarm2 % 60;
  sprintf(&buf[20],"%02i:%02i\n",h,m);
  for (i=0;i<7;i++) {
    mask = 1 << i;
    buf[6+i*2] = ((alarmday1 & mask) != 0)? '1':'0';
    buf[26+i*2] = ((alarmday2 & mask) != 0)? '1':'0';
    buf[7+i*2] = '\n';
    buf[27+i*2] = '\n';
  }
  buf[40] = 0;
  //send access data separated by new line
  //respond with access data
  server.send(200,"text/plain",String(buf));
}

//calculate minutes ffrom hours and minutes
uint16_t stringToMinutes(String val){
  uint8_t h,m;
  h = val.substring(0,2).toInt();
  m = val.substring(3).toInt();
  return h * 60 + m;
}

//AJAX command /cmt/setalarms
void setAlarms() {
  char txt[10];
  uint8_t b;
  Serial.println("Set alarms start");
  if (server.hasArg("al0")) {
    alarm1 = stringToMinutes(server.arg("al0"));
    Serial.print(server.arg("al0"));
    Serial.printf(" = %i\n",alarm1);
    pref.putUInt("alarm1",alarm1);
  }
  if (server.hasArg("al8")) {
    alarm2 = stringToMinutes(server.arg("al8"));
    Serial.print(server.arg("al8"));
    Serial.printf(" = %i\n",alarm2);
    pref.putUInt("alarm1",alarm1);
  }
  alarmday1 = 0;
  alarmday2 = 0;
  for (uint8_t i=0; i<7; i++) {
    sprintf(txt,"al%i",i+1);
    if (server.hasArg(txt)) {
      if (server.arg(txt) == "1") alarmday1 = alarmday1 | (1 << i); 
    }
    sprintf(txt,"al%i",i+9);
    if (server.hasArg(txt)) {
      if (server.arg(txt) == "1") alarmday2 = alarmday2 | (1 << i); 
    }
  }
  pref.putUShort("alarmday1",alarmday1);
  pref.putUShort("alarmday2",alarmday2);
  Serial.printf("days1 %x days2 %x\n",alarmday1,2);
  findNextAlarm();
  if (clockmode) showNextAlarm();
  server.send(200,"text/plain","OK");
}

//AJAX command /cmd/getstation
void getStationData() {
  //expects one argument with the stationid
  if (server.hasArg("stationid")) {
    uint8_t i = server.arg("stationid").toInt();
    if (i>=STATIONS) i = STATIONS-1;
    //build message string with station name, station url and enabled flag. Separated by new line
    String msg = String(stationlist[i].name) + "\n" + String(stationlist[i].url) + "\n" + String(stationlist[i].enabled) + "\n" + String(i+1);
    server.send(200,"text/plain",msg);
  } else {
    server.send(200,"text/plain","Invalid command");
  }
}

//AJAX command /cmd/setstation
void setStationData() {
  //expects an argument with a valid station id
  //data are sent as arguments too
  //name = the station name
  //url = the station url
  //enabled = enabled flag
  //position = new position in list
  char key[5];
  if ((server.hasArg("stationid")) && (server.arg("stationid").toInt() < STATIONS))  {
    uint8_t i = server.arg("stationid").toInt();
    //The values from the arguments will be saved in the station list
    if (server.hasArg("name")){
      sprintf(key,"n%i",i);
      sender.putString(key,server.arg("name"));
      strlcpy(stationlist[i].name,server.arg("name").c_str(),32);
    }
    if (server.hasArg("url")){
      sprintf(key,"u%i",i);
      sender.putString(key,server.arg("url"));
      strlcpy(stationlist[i].url,server.arg("url").c_str(),150);
    }
    if (server.hasArg("enabled")){
      sprintf(key,"f%i",i);
      sender.putUChar(key,server.arg("enabled").toInt());
      stationlist[i].enabled = server.arg("enabled").toInt();    
    }
    if (server.hasArg("position")){
      int16_t newpos = server.arg("position").toInt();
      newpos--;
      Serial.printf("Move %i to position %i\n",i+1,newpos+1);
      if ((i != newpos) && (newpos >= 0) && (newpos < STATIONS)) {
        reorder(i,newpos);
        saveList();
      }
    }
    server.send(200,"text/plain","OK");
  } else {
    server.send(200,"text/plain","Invalid station ID");
  }
}

//AJAX command /cmd/teststation
void testStation() {
  //exspects one argument with the url to be tested
  bool ret = true;
  if (server.hasArg("url"))  {
    ret = startUrl(server.arg("url"));
  }
  if (ret) {
    //if success respond is OK
    server.send(200,"text/plain","OK");
  } else {
    //if no success switch back to the current station
    //and respond with ERROR
    startUrl(String(stationlist[actStation].url));
    server.send(300,"text/plain","ERROR");
  }
}

//AJAX command /cmd/endtest
void endTest() {
  //switch back to the current station to end the test
  startUrl(String(stationlist[actStation].url));
  //respond with OK
  server.send(200,"text/plain","OK");
}

void restoreStations() {
  restore();
  //respond with OK
  server.send(200,"text/plain","OK");
}
//AJAX command /cmd/restart
void restart(){
  ESP.restart();
}
