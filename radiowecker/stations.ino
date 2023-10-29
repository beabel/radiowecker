
//default stationlist
#define DEFAULTSTATIONS 24


//Definition of a set of tested default stations
Station defstations[DEFAULTSTATIONS] PROGMEM = {
{"http://stream01.zogl.net:8906/stream","Radino",1},
{"http://radio.toggo.de/live/mp3-192/radio.toggo.de/","Toggo",1},
{"http://streams.radiobob.de/bob-national/mp3-192/mediaplayer","BOB",1},
{"http://stream.laut.fm/hoerspiel","Hörspiele",1},
{"http://stream.laut.fm/jugend_radio","Jugend",1},
{"http://646.cloudrad.io:8234","Strobe",1},
{"http://646.cloudrad.io:8276","Pulse107",1},
{"http://646.cloudrad.io:8314","DJ Perry",1},
{"http://dispatcher.rndfnk.com/hr/hr3/live/mp3/48/stream.mp3","Hessen3",0},
{"http://stream.antenne.de/antenne","Antenne Bayern",0},
{"http://stream.1a-webradio.de/saw-deutsch/","Radio 1A Deutsche Hits",0},
{"http://stream.1a-webradio.de/saw-rock/","Radio 1A Rock",0},
{"http://streams.80s80s.de/ndw/mp3-192/streams.80s80s.de/","Neue Deutsche Welle",0},
{"http://dispatcher.rndfnk.com/br/brklassik/live/mp3/low","Bayern Klassik",0},
{"http://mdr-284280-1.cast.mdr.de/mdr/284280/1/mp3/low/stream.mp3","MDR",0},
{"http://icecast.ndr.de/ndr/njoy/live/mp3/128/stream.mp3","N-JOY",0},
{"http://dispatcher.rndfnk.com/rbb/rbb888/live/mp3/mid","RBB",0},
{"http://dispatcher.rndfnk.com/rbb/antennebrandenburg/live/mp3/mid","Antenne Brandenburg",0},
{"http://wdr-wdr3-live.icecastssl.wdr.de/wdr/wdr3/live/mp3/128/stream.mp3","WDR3",0},
{"http://wdr-wdr2-aachenundregion.icecastssl.wdr.de/wdr/wdr2/aachenundregion/mp3/128/stream.mp3","WDR 2",0},
{"http://rnrw.cast.addradio.de/rnrw-0182/deinschlager/low/stream.mp3","NRW Schlagerradio",0},
{"http://rnrw.cast.addradio.de/rnrw-0182/deinrock/low/stream.mp3","NRW Rockradio",0},
{"http://rnrw.cast.addradio.de/rnrw-0182/dein90er/low/stream.mp3","NRW 90er",0},
{"http://mp3.hitradiort1.c.nmdn.net/rt1rockwl/livestream.mp3","RT1 Rock",0}};

//fill the station list from preferences. If an entry not exists the default list will be used
void setup_senderList() {
  char nkey[4];
  char ukey[4];
  char fkey[4];
  for (uint8_t i = 0; i<STATIONS; i++) {
    sprintf(nkey,"n%i",i);
    sprintf(ukey,"u%i",i);
    sprintf(fkey,"f%i",i);
    if (i<DEFAULTSTATIONS) {
      strlcpy(stationlist[i].name,sender.getString(nkey,defstations[i].name).c_str(),32);
      strlcpy(stationlist[i].url,sender.getString(ukey,defstations[i].url).c_str(),150);
      stationlist[i].enabled = sender.getUChar(fkey,defstations[i].enabled);
    } else {
      strlcpy(stationlist[i].name,sender.getString(nkey,nkey).c_str(),32);
      strlcpy(stationlist[i].url,sender.getString(ukey,"").c_str(),150);
      stationlist[i].enabled = sender.getUChar(fkey,0);
    }
  }
}

//fill the station list from default list and save it into preferences
void restore() {
  char nkey[4];
  char ukey[4];
  char fkey[4];
  for (uint8_t i = 0; i<STATIONS; i++) {
    sender.clear();
    sprintf(nkey,"n%i",i);
    sprintf(ukey,"u%i",i);
    sprintf(fkey,"f%i",i);
    if (i<DEFAULTSTATIONS) {
      strlcpy(stationlist[i].name,defstations[i].name,32);
      sender.putString(nkey,defstations[i].name);
      strlcpy(stationlist[i].url,defstations[i].url,150);
      sender.putString(ukey,defstations[i].url);
      stationlist[i].enabled = defstations[i].enabled;
      sender.putUChar(fkey,defstations[i].enabled);
    } else {
      strlcpy(stationlist[i].name,nkey,32);
      sender.putString(nkey,nkey);
      strlcpy(stationlist[i].url,"",150);
      sender.putString(ukey,"");
      stationlist[i].enabled = 0;
      sender.putUChar(fkey,0);
    }
  }
}

//save stationlist into preferences
void saveList() {
  char nkey[4];
  char ukey[4];
  char fkey[4];
  for (uint8_t i = 0; i<STATIONS; i++) {
    sprintf(nkey,"n%i",i);
    sprintf(ukey,"u%i",i);
    sprintf(fkey,"f%i",i);
    sender.putString(nkey,stationlist[i].name);
    sender.putString(ukey,stationlist[i].url);
    sender.putUChar(fkey,stationlist[i].enabled);
  }
}

//move a station inside the station list from old position to newposition
void reorder(uint8_t oldpos, uint8_t newpos) {
  Station temp;
  if ((oldpos < STATIONS) && (newpos  < STATIONS)) {
    if (oldpos > newpos) {
      memcpy(&temp,&stationlist[oldpos],sizeof(Station));
      for (uint8_t i = oldpos; i>newpos; i--) {
        memcpy(&stationlist[i],&stationlist[i-1],sizeof(Station));
      }
      memcpy(&stationlist[newpos],&temp,sizeof(Station));
    } else {
      memcpy(&temp,&stationlist[oldpos],sizeof(Station));
      for (uint8_t i = oldpos; i<newpos; i++) {
        memcpy(&stationlist[i],&stationlist[i+1],sizeof(Station));
      }
      memcpy(&stationlist[newpos],&temp,sizeof(Station));
    }
  }
}
