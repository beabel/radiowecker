#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "fonts.h"
#include "tft_color_setting.h"

//pins for touchscreen
#define TOUCH_CS 14
#define TOUCH_IRQ 27

//define names for fonts
#define FNT9 &AT_Standard9pt7b
#define FNT12 &AT_Bold12pt7b

//pins for display
#define TFT_CS   5   
#define TFT_DC   4   
#define TFT_RST  22
#define TFT_LED  15
#define TOUCH_CS 14
#define LED_ON 0
#define LS 23  // line spacing

//day and month names in German
const char* const PROGMEM days[] = {"Sonntag","Montag","Dienstag","Mittwoch","Donnerstag","Freitag","Samstag"};
const char* PROGMEM months[] = {"Jan.","Feb.","März","April","Mai","Juni","Juli","Aug.","Sept.","Okt.","Nov.","Dez."};

//instance of display driver
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
//instance for touchscreen driver
XPT2046_Touchscreen touch(TOUCH_CS, TOUCH_IRQ);
//instance to start callbacks on touch events
TouchEvent tevent(touch);

//register a callback for any touch event.
//we get position and type of the event
void onTouchClick(TS_Point p) {
  if (!clockmode) start_conf = millis(); //if we are in config mode reset start_conf on any event
  Serial.printf("Touch on %i, %i\n",p.x,p.y);
  if (clockmode) { //if we are in the clock mode, we switch into the config mode. Independent where the event occured.
    clockmode = false;
    showCommand(); 
  } else {
    //we are in the config mode
    if (p.y > 180) { //if the y value > 180, we are in the button area
      //we nned the p.x position to find which button was clicked
      if (p.x < 64) toggleRadio(radio);
      if ((p.x>64) && (p.x<128)) startSnooze();
      if ((p.x>128) && (p.x<192)) toggleAlarm();
      if ((p.x>192) && (p.x<256)) changeStation();
      if (p.x > 256) {
        clockmode = true;
        showClock();
      }
    }
    //from 0 to 44 we have the slider for gain
    if ((p.y > 0) && (p.y<44)) setGainValue(p.x);
    //from 44 to 88 we have the slider for brightness
    if ((p.y > 44) && (p.y<88)) setBrightness(p.x);
    //from 88 to 132 we have the slider for snooze time
    if ((p.y > 88) && (p.y<132)) setSnoozeTime(p.x);
    //from 132 to 175 we have the station list
    if ((p.y > 132) && (p.y<175)) selectStation(p.x);
  }
}

//set the gain for x-position where the slider was clicked
void setGainValue(uint16_t value) {
  char txt[10];
  //calculate gain from x-Position 0 to 100%
  float v = (value - 15) * 0.345;
  if (v > 100) v = 100;
  if (v < 0) v = 0;
  curGain = v;
  //save gain and adjust slider and set gain to the new value
  pref.putUShort("gain",curGain);
  showSlider(27,curGain,100);
  setGain(curGain);
  sprintf(txt,"%i %%",curGain);
  displayMessage(231,8,80,20,txt,ALIGNRIGHT,false,ILI9341_BLACK,ILI9341_LIGHTGREY,1);
}

//set the brightness for x-position where the slider was clicked
void setBrightness(uint16_t value) {
  char txt[10];
  //calculate brightness from x-Position 0 to 100 %
  float v = (value - 15) * 0.345;
  if (v > 100) v = 100;
  if (v < 5) v = 0;
  bright = v;
  //save brightness and adjust slider 
  pref.putUShort("bright",bright);
  //setBGLight(bright);
  showSlider(71,bright,100);
  sprintf(txt,"%i %%",bright);
  displayMessage(231,52,80,20,txt,ALIGNRIGHT,false,ILI9341_BLACK,ILI9341_LIGHTGREY,1);
}

//set the snooze time for x-position where the slider was clicked
void setSnoozeTime(uint16_t value) {
  char txt[10];
  //calculate snooze time from x-Position 0 to 60 minutes
  float v = (value - 15) * 0.21;
  if (v > 60) v = 60;
  if (v < 0) v = 0;
  snoozeTime = v;
  //save snooze time and adjust slider 
  pref.putUShort("snooze",snoozeTime);
  showSlider(115,snoozeTime,60);
  sprintf(txt,"%i min",snoozeTime);
  displayMessage(231,96,80,20,txt,ALIGNRIGHT,false,ILI9341_BLACK,ILI9341_LIGHTGREY,1);
}

//adjust the brightness of the background led
void setBGLight(uint8_t prct) {
  uint16_t ledb;
  //if brightness is 0, we read the ambient light from LDR
  if (prct == 0) {
    ledb = analogRead(LDR) * 255 /4096;
  } else {
    ledb = 255*prct/100;
  }
  if (ledb > 255) ledb = 255;
  if (ledb <3) ledb = 3; //minimal brightness
  if (LED_ON == 0) ledb = 255 - ledb;
  Serial.printf("percent = %i LED = %i\n",prct,ledb);
  //set the LED
  analogWrite(TFT_LED, ledb);
}

//select a station from the stationlist
void selectStation(uint16_t x) {
  //there are two buttons left and right to scroll through the stationlist
  if (x<50) { //left button to scroll down
    //we go down until we find an enabled station
    do {
      if (curStation == 0) {
        curStation = STATIONS - 1;
      }else {
        curStation --;
      }
    }
    while (! stationlist[curStation].enabled);
  }
  if (x>270) { //left button to scroll up
    //we go up until we find an enabled station
    do {
      if (curStation == (STATIONS - 1)) {
        curStation = 0;
      }else {
        curStation ++;
      }
    }
    while (! stationlist[curStation].enabled);
  }
  //show the selected station
  updateStation();
}

//turn the radio on or off
void toggleRadio(boolean off) {
  if (off) {
    //if off stop the stream
    stopPlaying();
    radio = false;
    //setGain(0);
  } else {
    if (connected) {
      //if on start the stream an set the gain
      radio = true;
      if (!startUrl(String(stationlist[actStation].url))) {
        //if no success switch to station 0
        actStation = 0;
        startUrl(String(stationlist[actStation].url));
      }
      setGain(curGain);
    }
  }
  //switch to clock screen
  clockmode = true;
  showClock();
}

//turn the alarm on or off
void toggleAlarm() {
  if (alarmday > 7) {
    //if the alarm was off find the next alarm an activate it
    findNextAlarm();
    pref.putBool("alarmon",true);
  } else {
    //deactivate the alarm
    alarmday = 8;
    pref.putBool("alarmon",false);
  }
  //switch to clock screen
  clockmode = true;
  showClock();
  }

void startSnooze() {
  snoozeWait = snoozeTime;
  toggleRadio(false);
  clockmode = true;
  showClock();
}

//set the selected station as the active station
void changeStation() {
    actStation = curStation;
    //save the new value and start stream
    pref.putUShort("station",curStation);
    if (!startUrl(String(stationlist[actStation].url))) {
      //if start fails we switch back to station 0
      actStation = 0;
      startUrl(String(stationlist[actStation].url));
    }
  //switch to clock screen
  clockmode = true;
  showClock();
}

//called from the main loop to handle touch events
void touch_loop() {
    tevent.pollTouchScreen();
}

//prepare display
void setup_display() {
  pinMode(TFT_LED,OUTPUT);
  pinMode(LDR,INPUT);
  lastldr = analogRead(LDR);
  setBGLight(100);
  tft.begin();
  tft.fillScreen(ILI9341_BLACK);
  tft.setRotation(3);
  tft.setFont(&AT_Bold12pt7b);
  touch.begin();
  tevent.setResolution(tft.width(),tft.height());
  tevent.setDrawMode(false);
  //register callback function
  tevent.registerOnTouchClick(onTouchClick);
}
//display a text in a rectangle on position x,y with the size w,h 
//the text can bbe aligned left, center or right text size can be set to standard or big 
//font color and background color can be selected as well as the number of lines
void textInBox(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const char* text, uint8_t align = ALIGNLEFT, boolean big = false, uint16_t fc = ILI9341_WHITE , uint16_t bg = ILI9341_BLACK, uint8_t lines = 1 ) {
  char tmp[256];
  char msg[50];
  char * token;
  int16_t xt,yt,h0,x0,sp;
  uint16_t wt,ht,len;
  uint8_t l;
  //first the rectangle will be filled with the background color to erase old content
  tft.fillRect(x,y,w,h,bg);
  if (big) tft.setFont(FNT12); else tft.setFont(FNT9);
  tft.setTextColor(fc);
  //encoding for german umlauts
  encodeUnicode(text, tmp);
  tft.getTextBounds("n",0,100,&xt,&yt,&wt,&ht);
  sp =wt;
  Serial.printf("Space = %i\n",sp);
  tft.getTextBounds(tmp,0,100,&xt,&yt,&wt,&ht);
  Serial.printf("Text %s Länge %i\n",text,wt);
  h0 = 100 - yt;
  x0 = x;
  y = y + h0 +1;
  if (wt<w) { //enough space in one line
    if (align == ALIGNCENTER) x0 += (w-wt)/2;
    if (align == ALIGNRIGHT) x0 += (w-wt);
    Serial.printf("x= %i, y= %i, ht= %i, text = %s\n",x0,y,ht,tmp);
    tft.setCursor(x0, y);
    tft.print(tmp);
  } else { //if there is not enough space in one line we adjust the text by word wrap
    //if only one line is allowed, the text will be cut off on a word boundary
    l=lines;
    token = strtok(tmp," ");
    
    msg[0] = 0;
    len = 0;
    while ((token != NULL) && (l>0)) {
      tft.getTextBounds(token,0,100,&xt,&yt,&wt,&ht); 
      Serial.printf("Token %s Länge %i gesamt %s Länge %i Line %i\n",token,wt,msg,len,l);
      if ((len + wt + sp) < w) {
        if (len > 0) { strcat(msg," "); len +=sp; }
        len = len + wt;
        strcat(msg,token);
      } else {
        x0 = x;
        if (align == ALIGNCENTER) x0 += (w-len)/2;
        if (align == ALIGNRIGHT) x0 += (w-len);
        tft.setCursor(x0, y);
        tft.print(msg);
        len = wt;
        strcpy(msg,token);
        y = y + ht +1;
        l--;
      }
      Serial.println(token);
      token = strtok(NULL, " ");
      
    }
    if ((len > 0) && (l>0)) {
      x0 = x;
      if (align == ALIGNCENTER) x0 += (w-len)/2;
      if (align == ALIGNRIGHT) x0 += (w-len);
      y = y + h0 +1;
      tft.setCursor(x0, y);
       tft.print(msg);
    }
  }
}

//the clock display will be updated. To avoid flicker, only changed parts will be redrawn
//is the parameter redraw true, the complete content will be redrawn
void updateTime(boolean redraw) {
  char tim[40];
  //these variables are static which means they will be initialized on the first call and keep there values
  static char lastdate[40] = "";
  static char lasttime[10] = "";
  
  if (getLocalTime(&ti)) {
    sprintf(tim,"%s %i. %s %i",days[ti.tm_wday],ti.tm_mday,months[ti.tm_mon],ti.tm_year + 1900);
    //if redraw is true or the date has changed, the date line will be redrawn
    if (redraw || (strcmp(tim,lastdate) != 0)) {
      strcpy(lastdate,tim);
      textInBox(0,0,320,25,tim,ALIGNCENTER,true,COLOR_DATE,COLOR_BG,1);
    }
    uint8_t z;
    strftime(tim, sizeof(tim), "%H:%M", &ti);
    Serial.printf("Zeit = %s\n",tim);
    //we iterate over the time string
    //if redraw is true or a number has changed this single number will be redrawn
    for (uint8_t i = 0; i<5; i++) {
      z = (i==2)?10:tim[i]-'0';
      Serial.printf("Ziffer %i %c = %c\n",z,tim[i],lasttime[i]);
      if ((z<11) && (redraw || (tim[i] != lasttime[i]))) {
        //UHRZEIT auf Haupzseite anzeigen jetzt variabel in der Farbe
        tft.drawBitmap(30+i*55,30,ziffern_rot[z],50,70,COLOR_TIME,COLOR_BG);        
        //tft.drawRGBBitmap(30+i*55,30,ziffern_rot[z],50,70);
      }
    }
    strcpy(lasttime,tim);
  }
}

//clear the whole display
void displayClear() {
  tft.fillScreen(COLOR_BG);
}


//show date, time and loudness in the first line
void displayDateTime() {
  Serial.println("Show Time");
  updateTime(false);
}

//show the progress of softwareupdate as a bar and a number
void showProgress(uint32_t prc) {
  char tmp[20];
  sprintf(tmp,"Progress %i%%",prc); 
  textInBox(5, 50, 310, 30, tmp, ALIGNLEFT, true, ILI9341_GREEN, COLOR_BG,1);
  if (prc == 0) {
    tft.drawRect(5,80,310,20,ILI9341_BLUE);
  } else {
    tft.fillRect(5,80,310 * prc / 100,20,ILI9341_BLUE);
  }
}

//encode text from unicode to display code for supplied font
//src and dst buffer needs to exist
void encodeUnicode(const char* src, char* dst){
  uint8_t i = 0, i1 = 0;
  char c;
  uint16_t  m = strlen(src);
  do {
    c = src[i];
    if (c==195) { //UTF8 characters german umlauts
      i++;
      switch (src[i]) {
        case 164: c=130; break; //ä
        case 182: c=131; break; //ö
        case 188: c=132; break; //ü
        case 159: c=133; break; //ß
        case 132: c=127; break; //Ä
        case 150: c=128; break; //Ö
        case 156: c=129; break; //Ü
        default: c=255;
      }
    } else if (c == 194) { //UTF8 char for degree symbol
      i++;
      if (src[i] == 176) c=134; else c=255;
    }
    if (c<135) dst[i1] = c;
    i++;
    i1++;
  } while (i<m);
  dst[i1] = 0;
}

//show a slider. y defines the top line value is the place to display the rectangle
void showSlider(uint16_t y,float value, uint16_t vmax) {
  tft.fillRect(11,y,298,14,ILI9341_LIGHTGREY);
  tft.drawLine(15,y+6,305,y+6,ILI9341_BLACK);
  tft.drawLine(15,y+7,305,y+7,ILI9341_BLACK);
  uint16_t pos = value*290/vmax;
  tft.fillRect(12+pos,y,8,14,ILI9341_BLACK);
}

//display the area to show the gain on config screen
void showGain() {
  char txt[20];
  tft.fillRect(0,0,320,44,ILI9341_LIGHTGREY);
  tft.drawRect(0,0,320,44,ILI9341_BLUE);
  sprintf(txt,"%i %%",curGain);
  textInBox(231,8,80,20,txt,ALIGNRIGHT,false,ILI9341_BLACK,ILI9341_LIGHTGREY,1);
  tft.setCursor(5,23);
  tft.setFont(FNT9);
  tft.setTextColor(ILI9341_BLACK);
  encodeUnicode("Lautstärke",txt);
  tft.print(txt);
  showSlider(27,curGain,100);
}

//display the area to show the brightness on config screen
void showBrigthness() {
  char txt[20];
  tft.fillRect(0,44,320,44,ILI9341_LIGHTGREY);
  tft.drawRect(0,44,320,44,ILI9341_BLUE);
  sprintf(txt,"%i %%",bright);
  textInBox(231,52,80,20,txt,ALIGNRIGHT,false,ILI9341_BLACK,ILI9341_LIGHTGREY,1);
  tft.setCursor(5,67);
  tft.setFont(FNT9);
  tft.setTextColor(ILI9341_BLACK);
  encodeUnicode("Helligkeit",txt);
  tft.print(txt);
  showSlider(71,bright,100);
}

//display the area to show the snooze time on config screen
void showSnoozeTime() {
  char txt[20];
  tft.fillRect(0,88,320,44,ILI9341_LIGHTGREY);
  tft.drawRect(0,88,320,44,ILI9341_BLUE);
  sprintf(txt,"%i min",snoozeTime);
  textInBox(231,96,80,20,txt,ALIGNRIGHT,false,ILI9341_BLACK,ILI9341_LIGHTGREY,1);
  tft.setCursor(5,111);
  tft.setFont(FNT9);
  tft.setTextColor(ILI9341_BLACK);
  encodeUnicode("Einschlafzeit",txt);
  tft.print(txt);
  showSlider(115,snoozeTime,60);
}

//show the selected station in the station area
void updateStation() {
  char txt[40];
  encodeUnicode(stationlist[curStation].name,txt);
  textInBox(40,145,240,20,txt,ALIGNCENTER,false,ILI9341_BLACK,ILI9341_LIGHTGREY,1);
}

//display the area to show the stations on config screen
void showStationList() {
  tft.fillRect(0,132,320,44,ILI9341_LIGHTGREY);
  tft.drawRect(0,132,320,44,ILI9341_BLUE);
  tft.fillTriangle(5,138,20,169,35,138,ILI9341_BLUE);
  tft.fillTriangle(279,169,294,138,309,169,ILI9341_BLUE);
  curStation = actStation;
  updateStation();
}

//display the complete config screen
void showCommand() {
    setBGLight(100);
    tft.fillScreen(COLOR_BG);
    showGain();
    showBrigthness();
    showSnoozeTime();
    showStationList();
    //tft.drawRGBBitmap(0,176,knopfimg,320,64);
    //kunigunde Knoepfe jetzt als monochrome Bitmaps, somit anpassbar
    //in naechster Version einzelne Knoepfe, um sie dynamisch farblich zu änden
    //Achtung in radiowecker.ino die neue knoepfe_neu.h am Anfang statt der alten includen
    tft.drawBitmap(0,176,knopfimg_neu,320,64,ILI9341_BLUE,ILI9341_LIGHTGREY);
    start_conf = millis();
}


//show name of active station on TFT display
void showStation() {
  textInBox(5,122,310,35,stationlist[actStation].name,ALIGNCENTER,true,COLOR_STATION_NAME, COLOR_STATION_BOX_BG);
}

//show current meta data on TFT display
void showTitle() {
    displayMessage(8, 152, 304, 57, title, ALIGNCENTER, false, COLOR_STATION_TITLE, COLOR_STATION_BOX_BG,2);
}

//display the radio area on the clock screen
void showRadio() {
  if (radio) {
    tft.fillRect(3,117,314,93,COLOR_STATION_BOX_BG);
    tft.drawRect(3,117,314,93,COLOR_STATION_BOX_BORDER);
    showStation();
    showTitle();
  } else {
    tft.fillRect(3,117,314,93,COLOR_BG);
  }
}

//if an alarm is active, the next alarm date and time will be displayed
//on the bottom line of clock screen
void showNextAlarm(){
  char txt[100] = "";
  uint8_t h,m;
  if (clockmode) {
    if (alarmday < 8){
      h = alarmtime / 60;
      m = alarmtime % 60;
      sprintf(txt,"Wecker: %s um %i:%02i",days[alarmday],h,m);
      textInBox(0,220,320,20,txt,ALIGNCENTER,false,COLOR_NEXT_ALARM,COLOR_BG,1);
    }else{
      sprintf(txt,"WECKER AUSGESCHALTEN !!!");
      textInBox(0,220,320,20,txt,ALIGNCENTER,false,ILI9341_RED,COLOR_BG,1);
    }
  }
}

//could be used to display three integer values on the bottom line of the display
void showDebugInfo(int16_t v1, int16_t v2, int16_t v3){
  char txt[100] = "";
  uint8_t h,m;
  if (clockmode ) {
   sprintf(txt,"Info: v1 = %i, v2 = %i, v3 = %i",v1,v2,v3);
    textInBox(0,220,320,20,txt,ALIGNCENTER,false,ILI9341_WHITE,ILI9341_BLACK,1);
  }
}

//redraw the clock screen
void showClock() {
    start_conf = 0;
    setBGLight(bright);
    tft.fillScreen(COLOR_BG);
    updateTime(true);
    if (radio) showRadio();
    showNextAlarm();
}


//a cover function for text in the box to be used from other sub parts
void displayMessage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const char* text, uint8_t align, boolean big, uint16_t fc, uint16_t bg, uint8_t lines) {
  textInBox(x,y,w,h,text,align,big,fc,bg,lines);
}
