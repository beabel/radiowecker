//default stationlist
#define DEFAULTSTATIONS 24

//Definition of a set of tested default stations
Station defstations[DEFAULTSTATIONS] PROGMEM = {
{"http://icecast.ndr.de/ndr/ndr2/niedersachsen/mp3/128/stream.mp3","NDR2 Niedersachsen",1},
{"http://icecast.ndr.de/ndr/ndr1niedersachsen/hannover/mp3/128/stream.mp3","NDR1 Hannover",1},
{"http://wdr-1live-live.icecast.wdr.de/wdr/1live/live/mp3/128/stream.mp3","WDR1",1},
{"http://wdr-cosmo-live.icecast.wdr.de/wdr/cosmo/live/mp3/128/stream.mp3","WDR COSMO",1},
{"http://radiohagen.cast.addradio.de/radiohagen/simulcast/high/stream.mp3","Radio Hagen",1},
{"http://st01.sslstream.dlf.de/dlf/01/128/mp3/stream.mp3","Deutschlandfunk",1},
{"http://dispatcher.rndfnk.com/br/br1/nbopf/mp3/low","Bayern1",1},
{"http://dispatcher.rndfnk.com/br/br3/live/mp3/low","Bayern3",1},
{"http://dispatcher.rndfnk.com/hr/hr3/live/mp3/48/stream.mp3","Hessen3",1},
{"http://stream.antenne.de/antenne","Antenne Bayern",1},
{"http://stream.1a-webradio.de/saw-deutsch/","Radio 1A Deutsche Hits",1},
{"http://stream.1a-webradio.de/saw-rock/","Radio 1A Rock",1},
{"http://streams.80s80s.de/ndw/mp3-192/streams.80s80s.de/","Neue Deutsche Welle",1},
{"http://dispatcher.rndfnk.com/br/brklassik/live/mp3/low","Bayern Klassik",1},
{"http://mdr-284280-1.cast.mdr.de/mdr/284280/1/mp3/low/stream.mp3","MDR",1},
{"http://icecast.ndr.de/ndr/njoy/live/mp3/128/stream.mp3","N-JOY",1},
{"http://dispatcher.rndfnk.com/rbb/rbb888/live/mp3/mid","RBB",1},
{"http://dispatcher.rndfnk.com/rbb/antennebrandenburg/live/mp3/mid","Antenne Brandenburg",1},
{"http://wdr-wdr3-live.icecastssl.wdr.de/wdr/wdr3/live/mp3/128/stream.mp3","WDR3",1},
{"http://wdr-wdr2-aachenundregion.icecastssl.wdr.de/wdr/wdr2/aachenundregion/mp3/128/stream.mp3","WDR 2",1},
{"http://rnrw.cast.addradio.de/rnrw-0182/deinschlager/low/stream.mp3","NRW Schlagerradio",1},
{"http://rnrw.cast.addradio.de/rnrw-0182/deinrock/low/stream.mp3","NRW Rockradio",1},
{"http://rnrw.cast.addradio.de/rnrw-0182/dein90er/low/stream.mp3","NRW 90er",1},
{"http://mp3.hitradiort1.c.nmdn.net/rt1rockwl/livestream.mp3","RT1 Rock",1}};

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
