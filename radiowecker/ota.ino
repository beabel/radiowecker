uint32_t oldprc;

//prepare OTA
void setup_ota() {
  //set host name and passwort
  ArduinoOTA.setHostname("radiowecker");
  ArduinoOTA.setPassword("weckerupdate");
  //register callback routines
  ArduinoOTA.onStart(ota_onStart);
  ArduinoOTA.onEnd(ota_onEnd);
  ArduinoOTA.onProgress(ota_onProgress);
  ArduinoOTA.onError(ota_onError);
  //start OTA handling
  ArduinoOTA.begin();
}

//on start decide between program or file upload
void ota_onStart() {
  displayClear();
  setBGLight(100);
  oldprc = 0;
  String type;
  if (ArduinoOTA.getCommand() == U_FLASH)
    type = "sketch";
  else // U_SPIFFS
    type = "filesystem";

  // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
  displayMessage(5, 10, 310, 30, "Updating Sketch", ALIGNCENTER, true, ILI9341_YELLOW, ILI9341_BLACK,1);
  showProgress(0);
}
//on end no extra processing
void ota_onEnd() {
  displayMessage(5, 120, 310, 30, "Done", ALIGNLEFT, true, ILI9341_GREEN, ILI9341_BLACK,1);
}

//s
void ota_onProgress(unsigned int progress, unsigned int total) {
  uint32_t prc = progress / (total / 100);
  char tmp[20];
  if (prc > oldprc) {
    showProgress(prc);
    oldprc = prc;
  }
}

void ota_onError(ota_error_t error) {
  char err[80];
  sprintf(err,"Error[%u]: ", error);
  if (error == OTA_AUTH_ERROR) sprintf(err,"Error[%u]: Auth Failed",error);
  else if (error == OTA_BEGIN_ERROR) sprintf(err,"Error[%u] Begin Failed",error);
  else if (error == OTA_CONNECT_ERROR) sprintf(err,"Error[%u] Connect Failed",error);
  else if (error == OTA_RECEIVE_ERROR) sprintf(err,"Error[%u] Receive Failed",error);
  else if (error == OTA_END_ERROR) sprintf(err,"Error[%u] End Failed",error);
  displayMessage(5,200,310,30,err,ALIGNLEFT,false,ILI9341_RED);
}
