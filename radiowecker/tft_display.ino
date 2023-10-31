#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "fonts.h"
#include "tft_color_setting.h"
#include "knoepfe.h" //Graphic data for buttons
#include "symbole.h" //Graphic data for symbolic
#include "num_64_44.h" //Graphic data for Fav Buttons
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
#define LED_ON 0
#define LS 23  // line spacing


//day and month names in German
const char* const PROGMEM days[] = {"Sonntag","Montag","Dienstag","Mittwoch","Donnerstag","Freitag","Samstag"};
const char* const PROGMEM days_short[] = {"So","Mo","Di","Mi","Do","Fr","Sa"};
const char* PROGMEM months[] = {"Jan.","Feb.","März","April","Mai","Juni","Juli","Aug.","Sept.","Okt.","Nov.","Dez."};

//instance of display driver
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
//instance for touchscreen driver
XPT2046_Touchscreen touch(TOUCH_CS, TOUCH_IRQ);
//instance to start callbacks on touch events
TouchEvent tevent(touch);
//Prototype created (i have now optional Arguments) the function down like before
void showSlider(uint16_t y, float value, uint16_t vmax, uint16_t bgColor = ILI9341_LIGHTGREY, uint16_t lineColor = ILI9341_BLACK);

//register a callback for any touch event.
//we get position and type of the event
void onTouchClick(TS_Point p) {
  if (!clockmode) start_conf = millis(); //if we are in config mode reset start_conf on any event
  Serial.printf("Touch on %i, %i\n",p.x,p.y);
  if (clockmode) { //if we are in the clock mode, we switch into the radiopage or adjust Gain
    if (p.y < 210) {// only on the top area we switch to radiopage
      clockmode = false;
      configpage = false;
      radiopage = true;
      showRadioPage(); 
    }else{// we adjust the Gain
      setGainValue(p.x, "MainPage");      
    }
  } else if (radiopage) {//################################################ RADIO PAGE
    //we are in the radio mode
    if (p.y > 180) { //if the y value > 180, we are in the button area
      //we need the p.x position to find which button was clicked
      if (p.x < 64) toggleRadio(radio);
      if ((p.x>64) && (p.x<128)) startSnooze();
      if ((p.x>128) && (p.x<192)) toggleAlarm();
      if ((p.x>192) && (p.x<256)) changeStation();
      if (p.x > 256) {//rechter Button zu configpage
        clockmode = false;
        configpage = true;
        radiopage = false;
        showCommand(); 
      }
    }
    //from 0 to 44 we have the slider for gain
    if ((p.y > 0) && (p.y<44)) setGainValue(p.x, "RadioPage");
    //from 44 to 132 we have now space for Favorite Sender Buttons/////////////////////////////////////////
    if ((p.y > 44) && (p.y<132)){
      if(p.y<88){
        if (p.x < 64){curStation = 0;} 
        if ((p.x>64) && (p.x<128)){curStation = 1;} 
        if ((p.x>128) && (p.x<192)){curStation = 2;} 
        if ((p.x>192) && (p.x<256)){curStation = 3;} 
        if (p.x > 256){curStation = 4;}
      }else{
        if (p.x < 64){curStation = 5;} 
        if ((p.x>64) && (p.x<128)){curStation = 6;} 
        if ((p.x>128) && (p.x<192)){curStation = 7;} 
        if ((p.x>192) && (p.x<256)){curStation = 8;} 
        if (p.x > 256){curStation = 9;}        
      }
      changeStation();
      showClock();       
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    //from 132 to 175 we have the station list
    if ((p.y > 132) && (p.y<175)) selectStation(p.x);
  } else if (configpage) {//################################################ CONFIG PAGE
    //we are in the config mode
    if (p.y > 180) { //if the y value > 180, we are in the button area
      //we need the p.x position to find which button was clicked
      if (p.x < 64) toggleRadio(radio);
      if ((p.x>64) && (p.x<128)) startSnooze();
      if ((p.x>128) && (p.x<192)) toggleAlarm();
      if ((p.x>192) && (p.x<256)) changeStation();
      if (p.x > 256) {//rechter Button zurück auf Main
        clockmode = true;
        configpage = false;
        radiopage = false;
        showClock();
      }
    }
    //from 0 to 44 we have the slider for gain
    if ((p.y > 0) && (p.y<44)) setGainValue(p.x, "SettingPage");
    //from 44 to 88 we have the slider for brightness
    if ((p.y > 44) && (p.y<88)) setBrightness(p.x);
    //from 88 to 132 we have the slider for snooze time
    if ((p.y > 88) && (p.y<132)) setSnoozeTime(p.x);
    //from 132 to 175 we have the station list
    if ((p.y > 132) && (p.y<175)) selectStation(p.x);
  }
}

// Set the gain for the x-position where the slider was clicked
void setGainValue(uint16_t value, const char* sliderPage) {
        char txt[10];
        float v;
        if(sliderPage != "WEBSITE"){
          // calculate gain from x-Position 0 to 100%
          int y_start = 11;  // 11 = Startlinie y
          int line_length = 298;  // 298 = Länge der Linie

          float start_slider = value - y_start;  // Bereich vor dem Linienstart abziehen
          float end_slider = line_length - y_start;  // Bereich vor dem Linienstart abziehen
          v = start_slider / end_slider * 100;  // Jetzt haben wir Prozent
          if (v > 100) v = 100;
          if (v < 0) v = 0;          
        }else{
          v = value;  
        }
        curGain = v;
        // Gain speichern und Schieberegler anpassen, Gain auf neuen Wert setzen
        pref.putUShort("gain", curGain);

        if (sliderPage == "MainPage" || (clockmode & sliderPage == "WEBSITE")) {
            showSlider(218, curGain, 100, COLOR_SLIDER_BG, COLOR_SLIDER);
        } else {// Standard Setting Page
            showSlider(27, curGain, 100);
            sprintf(txt, "%i %%", curGain);// Here we need also the Text on right Side
            displayMessage(231, 8, 80, 20, txt, ALIGNRIGHT, false, ILI9341_BLACK, ILI9341_LIGHTGREY, 1);
        }
        setGain(curGain);
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
    //reset Sleeptime
    snoozeWait = 0;
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
  //Serial.printf("Space = %i\n",sp);
  tft.getTextBounds(tmp,0,100,&xt,&yt,&wt,&ht);
  //Serial.printf("Text %s Länge %i\n",text,wt);
  h0 = 100 - yt;
  x0 = x;
  y = y + h0 +1;
  if (wt<w) { //enough space in one line
    if (align == ALIGNCENTER) x0 += (w-wt)/2;
    if (align == ALIGNRIGHT) x0 += (w-wt);
    //Serial.printf("x= %i, y= %i, ht= %i, text = %s\n",x0,y,ht,tmp);
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
    // Diese Variablen sind static, was bedeutet, dass sie beim ersten Aufruf initialisiert werden und ihre Werte behalten.
    static char lastdate[40] = "";
    static char lasttime[10] = "";

    if (getLocalTime(&ti)) {
        sprintf(tim,"%s %i. %s %i",days[ti.tm_wday],ti.tm_mday,months[ti.tm_mon],ti.tm_year + 1900);
        // Wenn redraw true ist oder das Datum sich geändert hat, wird die Datumszeile neu gezeichnet.
        if (redraw || (strcmp(tim,lastdate) != 0)) {
            strcpy(lastdate,tim);
            textInBox(0,90,320,25,tim,ALIGNCENTER,true,COLOR_DATE,COLOR_BG,1);
        }
        uint8_t z;
        strftime(tim, sizeof(tim), "%H:%M", &ti);
        Serial.printf("Zeit = %s\n",tim);
        // Wir durchlaufen den Zeit-String.
        // Wenn redraw true ist oder sich eine Ziffer geändert hat, wird diese Ziffer neu gezeichnet.
        for (uint8_t i = 0; i<5; i++) {
            z = (i==2)?10:tim[i]-'0';
            //Serial.printf("Ziffer %i %c = %c\n",z,tim[i],lasttime[i]);
            if ((z<11) && (redraw || (tim[i] != lasttime[i]))){
                // UHRZEIT auf der Hauptseite anzeigen, jetzt variabel in der Farbe.
                tft.drawBitmap(30+i*55,18,ziffern[z],50,70,COLOR_TIME,COLOR_BG);
            }
        }
        drawHeaderInfos();// Header Symbole und Text zeichnen
        strcpy(lasttime,tim);
    }
}

void drawHeaderInfos(){
  drawWifiInfo();// Wifi-Informationen zeichnen
  drawSnoozeInfo();// Einschlaf symbol zeichnen
  showNextAlarm();// Wecker symbol+text zeichnen 
}

void drawWifiInfo() {
    int rssi = WiFi.RSSI(); // Hole den RSSI-Wert
    char rssiChar[5]; // Char-Array für textInBox
    snprintf(rssiChar, sizeof(rssiChar), "%d", rssi);
    uint16_t color_wifi; // Farbvariable
    if (rssi <= -70) {// Schwacher Empfang (z.B. Rot)
        color_wifi = ILI9341_RED;
    } else if (rssi <= -50) {// Mittlerer Empfang (Z.B. Gelb)
        color_wifi = ILI9341_YELLOW;
    } else {// Starker Empfang (Z.B. Grün)
        color_wifi = ILI9341_GREEN;
    }
    tft.drawBitmap(303, 0, symbole[0], 17, 17, color_wifi, COLOR_BG);
    textInBox(273, 0, 30, 17, rssiChar, ALIGNCENTER, false, COLOR_WIFI_RSSI, COLOR_BG);
}

void drawSnoozeInfo(){
    uint16_t color_snooze; // Farbvariable
    if (snoozeWait != 0) {//Einschlaf aktiv
        color_snooze = COLOR_SLEEP_SYMBOL;
    } else {
        color_snooze = COLOR_BG;
    }  
    tft.drawBitmap(256, 0, symbole[1], 17, 17, color_snooze, COLOR_BG);
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
void showSlider(uint16_t y, float value, uint16_t vmax, uint16_t bgColor, uint16_t lineColor) {
    tft.fillRect(11, y, 298, 14, bgColor);
    tft.drawLine(15, y + 6, 305, y + 6, lineColor);
    tft.drawLine(15, y + 7, 305, y + 7, lineColor);
    uint16_t pos = value * 290 / vmax;
    tft.fillRect(12 + pos, y, 8, 14, lineColor);
}

//display the area to show the gain on config screen
void showGain() {
  char txt[20];
  tft.fillRect(0,0,320,44,COLOR_SETTING_BG);
  tft.drawRect(0,0,320,44,COLOR_SETTING_BORDER);
  sprintf(txt,"%i %%",curGain);
  textInBox(231,8,80,20,txt,ALIGNRIGHT,false,ILI9341_BLACK,COLOR_SETTING_BG,1);
  tft.setCursor(5,23);
  tft.setFont(FNT9);
  tft.setTextColor(ILI9341_BLACK);
  encodeUnicode("Lautstärke",txt);
  tft.print(txt);
  showSlider(27,curGain,100);
}

//display the area to show the gain on main screen
void showGainMain() {
  tft.fillRect(0,210,320,30,COLOR_SLIDER_BG);//Box
  tft.drawRect(0,210,320,30,COLOR_SLIDER_BORDER);// Rahmen
  showSlider(218,curGain,100,COLOR_SLIDER_BG,COLOR_SLIDER);
}

//display the area to show the brightness on config screen
void showBrigthness() {
  char txt[20];
  tft.fillRect(0,44,320,44,COLOR_SETTING_BG);
  tft.drawRect(0,44,320,44,COLOR_SETTING_BORDER);
  sprintf(txt,"%i %%",bright);
  textInBox(231,52,80,20,txt,ALIGNRIGHT,false,ILI9341_BLACK,COLOR_SETTING_BG,1);
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
  tft.fillRect(0,88,320,44,COLOR_SETTING_BG);
  tft.drawRect(0,88,320,44,COLOR_SETTING_BORDER);
  sprintf(txt,"%i min",snoozeTime);
  textInBox(231,96,80,20,txt,ALIGNRIGHT,false,ILI9341_BLACK,COLOR_SETTING_BG,1);
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
  textInBox(40,145,240,20,txt,ALIGNCENTER,false,ILI9341_BLACK,COLOR_SETTING_BG,1);
}

//display the area to show the stations on config screen
void showStationList() {
  tft.fillRect(0,132,320,44,COLOR_SETTING_BG);
  tft.drawRect(0,132,320,44,COLOR_SETTING_BORDER);
  tft.drawBitmap(0,132,symbole[4],50,44,COLOR_SETTING_UP_DOWN);
  tft.drawBitmap(270,132,symbole[5],50,44,COLOR_SETTING_UP_DOWN);  
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
    uint16_t color_temp; // Farbvariable
    //Power
    if (radio) {//Radio läuft
        color_temp = ILI9341_GREEN;
    } else {
        color_temp = COLOR_KNOEPFE;
    }      
    tft.drawBitmap(0,176,knopf_sym[0],64,64,color_temp,COLOR_KNOEPFE_BG);
    //Sleep
    if (snoozeWait != 0) {//Einschlaf aktiv
        color_temp = ILI9341_ORANGE;
    } else {
        color_temp = COLOR_KNOEPFE;
    }  
    tft.drawBitmap(64,176,knopf_sym[1],64,64,color_temp,COLOR_KNOEPFE_BG);
    //Alarm
    uint8_t symbol;  
    if (alarmday < 8){// Wecker aktiv
      color_temp = COLOR_KNOEPFE;
      symbol = 2;
    }else{// Wecker ausgeschalten
      color_temp = ILI9341_RED;
      symbol = 3;
    }      
    tft.drawBitmap(128,176,knopf_sym[symbol],64,64,color_temp,COLOR_KNOEPFE_BG);
    //Radio
    tft.drawBitmap(192,176,knopf_sym[4],64,64,COLOR_KNOEPFE,COLOR_KNOEPFE_BG);
    //Jump to Main (CLOCK SCREEN)
    tft.drawBitmap(256,176,knopf_sym[5],64,64,COLOR_KNOEPFE,COLOR_KNOEPFE_BG);

    start_conf = millis();
}

//display the complete radio screen
void showRadioPage() {
    setBGLight(100);
    tft.fillScreen(COLOR_KNOEPFE_BG);
    showGain();
    FavoriteButtons();
    showStationList();
    uint16_t color_temp; // Farbvariable
    //Power
    if (radio) {//Radio läuft
        color_temp = ILI9341_GREEN;
    } else {
        color_temp = COLOR_KNOEPFE;
    }      
    tft.drawBitmap(0,176,knopf_sym[0],64,64,color_temp,COLOR_KNOEPFE_BG);
    //Sleep
    if (snoozeWait != 0) {//Einschlaf aktiv
        color_temp = ILI9341_ORANGE;
    } else {
        color_temp = COLOR_KNOEPFE;
    }  
    tft.drawBitmap(64,176,knopf_sym[1],64,64,color_temp,COLOR_KNOEPFE_BG);
    //Alarm
    uint8_t symbol;  
    if (alarmday < 8){// Wecker aktiv
      color_temp = COLOR_KNOEPFE;
      symbol = 2;
    }else{// Wecker ausgeschalten
      color_temp = ILI9341_RED;
      symbol = 3;
    }      
    tft.drawBitmap(128,176,knopf_sym[symbol],64,64,color_temp,COLOR_KNOEPFE_BG);
    //Radio
    tft.drawBitmap(192,176,knopf_sym[4],64,64,COLOR_KNOEPFE,COLOR_KNOEPFE_BG);
    //jump to setting
    tft.drawBitmap(256,176,knopf_sym[6],64,64,COLOR_KNOEPFE,COLOR_KNOEPFE_BG);

    start_conf = millis();
}

//show name of active station on TFT display
void showStation() {
  textInBox(5,122,310,30,stationlist[actStation].name,ALIGNCENTER,true,COLOR_STATION_NAME, COLOR_STATION_BOX_BG);
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
void showNextAlarm(){
  uint16_t color_alarm; // Farbvariable
  uint8_t symbol;  
  char txt[50] = "";
  uint8_t h,m;
  if (clockmode) {
    if (alarmday < 8){// Wecker aktiv
      color_alarm = COLOR_ALARM_SYMBOL;
      symbol = 2;//bell_17      
      h = alarmtime / 60;
      m = alarmtime % 60;
      sprintf(txt,"%s %02i:%02i",days_short[alarmday],h,m);
    }else{// Wecker ausgeschalten
      color_alarm = ILI9341_RED;
      symbol = 3;//bell-slash_17      
      sprintf(txt,"AUS");
    }
    tft.drawBitmap(0, 0, symbole[symbol], 17, 17, color_alarm, COLOR_BG);// Symbol      
    textInBox(17,0,80,17,txt,ALIGNCENTER,false,color_alarm,COLOR_BG,1);// Text     
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
    //showNextAlarm moved to updateTime (only every minute) needed
    showGainMain();
}


//a cover function for text in the box to be used from other sub parts
void displayMessage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const char* text, uint8_t align, boolean big, uint16_t fc, uint16_t bg, uint8_t lines) {
  textInBox(x,y,w,h,text,align,big,fc,bg,lines);
}

// Create Favorite Buttons on Radio Page between y44 - y132 and switch directly
void FavoriteButtons(){
  int y = 44;
  bool active = false;
  int loopCount = 0;

  for (int i = 0; i < STATIONS && loopCount < 10; i++) {
    if (stationlist[i].enabled) {
      bool active = (actStation == i); // Aktiv für die aktuelle Station
      tft.drawBitmap((loopCount % 5) * 64, y, num_64_44[i], 64, 44, active ? COLOR_FAV_BUTTONS_AKTIV : COLOR_FAV_BUTTONS, COLOR_FAV_BUTTONS_BG);
      loopCount++;
      if (loopCount % 5 == 0) {
        y = 88;
      }
    }
  }
}
