//Includes from ESP8266audio
#include "AudioFileSourceICYStream.h"  //input stream
#include "AudioFileSourceBuffer.h"     //input buffer
#include "AudioGeneratorMP3.h"         //decoder
#include "AudioOutputI2S.h"            //output stream


#define LRCLK 25
#define BCLK 26
#define DOUT 33

//buffer size for stream buffering
const int preallocateBufferSize = 82 * 1024;  //size of preallocated buffer
const int preallocateCodecSize = 29192;       // MP3 codec max mem needed
//pointer to preallocated memory
void *preallocateBuffer = NULL;
void *preallocateCodec = NULL;

//instances for audio components
AudioGenerator *decoder = NULL;         //MP3 decoder
AudioFileSourceICYStream *file = NULL;  //Input modul for ICY stream from web
AudioFileSourceBuffer *buff = NULL;     //Buffer for input stream
AudioOutputI2S *out;                    //Output module to produce I2S

//function to do all the required setup
void setup_audio() {
  //reserve buffer für for decoder and stream
  preallocateBuffer = malloc(preallocateBufferSize);  // allocate memory for stream-file-buffer
  preallocateCodec = malloc(preallocateCodecSize);    // allocate memory for decoder- buffer
  delay(1000);
  //check if we got the memory
  if (!preallocateBuffer || !preallocateCodec) {
    Serial.printf_P(PSTR("FATAL ERROR:  Unable to preallocate %d bytes for app\n"), preallocateBufferSize + preallocateCodecSize);
    while (1) {
      yield();  // Infinite halt
    }
  }
  //create I2S output do use with decoder
  out = new AudioOutputI2S();
  out->SetPinout(BCLK, LRCLK, DOUT);
}

//this function checks if decoder runs
void audio_loop() {
  //check if stream has ended normally not on ICY streams
  if (decoder->isRunning()) {
    if (!decoder->loop()) {
      decoder->stop();
    }
  } else {
    //we have an error decoder has stop
    Serial.printf("MP3 done\n");
    //to avoid running in the same problem after restart
    //the station will be set to the first station entry
    //The first station on the list should be a stable
    //working URL. Do not use the first entry in the table
    //for experiments
    pref.putUShort("station", 0);

    // Restart ESP when streaming is done or errored
    delay(10000);

    ESP.restart();
  }
}

//callback function will be called if meta data were found in input stream
void MDCallback(void *cbData, const char *type, bool isUnicode, const char *string) {
  // Sicherstellen, dass die Zeiger gültig sind
  if (type == nullptr || string == nullptr) {
    Serial.println("Error: Null pointer encountered in MDCallback.");
    return;
  }

  // Hinweis: Der Typ und der String können im PROGMEM sein, daher Kopie ins RAM für printf
  char s1[32], s2[64];
  strncpy_P(s1, type, sizeof(s1) - 1);
  s1[sizeof(s1) - 1] = 0;
  strncpy_P(s2, string, sizeof(s2) - 1);
  s2[sizeof(s2) - 1] = 0;

  // Verarbeiten von Metadaten-Typ "Title"
  if (strstr_P(s1, PSTR("Title"))) {
    strncpy(title, s2, sizeof(title));
    title[sizeof(title) - 1] = 0;
    Serial.printf("Title: %s\n", title);
    // Zeige die Nachricht auf dem Display an
    newTitle = true;
  } else {
    // Nur den Typ und den String für generische Metadaten anzeigen
    Serial.printf("METADATA '%s' = '%s'\n", s1, s2);
  }

  // Serial.flush(); // Optional: Zum Flushing der Serial-Ausgabe, falls notwendig
}

//stop playing the input stream release memory, delete instances
void stopPlaying() {
  if (decoder) {
    decoder->stop();  //stop playing
    delete decoder;   //free decoder an its memory
    decoder = NULL;
  }
  if (buff) {
    buff->close();  //do the same for buffer
    delete buff;
    buff = NULL;
  }
  if (file) {  //AND FINALLY FOR THE STREAM
    file->close();
    delete file;
    file = NULL;
  }
}

//start playing a stream from given station
bool startUrl(String url) {
  bool ret = true;
  stopPlaying();  //first close existing streams
  //open input file for selected url
  Serial.printf("Active station %s\n", url.c_str());
  file = new AudioFileSourceICYStream();
  ret = file->open(url.c_str());
  if (ret) {
    //register callback for meta data
    file->RegisterMetadataCB(MDCallback, NULL);
    //create a new buffer which uses the preallocated memory
    buff = new AudioFileSourceBuffer(file, preallocateBuffer, preallocateBufferSize);
    Serial.printf_P(PSTR("sourcebuffer created - Free mem=%d\n"), ESP.getFreeHeap());
    //create and start a new decoder
    decoder = (AudioGenerator *)new AudioGeneratorMP3(preallocateCodec, preallocateCodecSize);
    Serial.printf_P(PSTR("created decoder\n"));
    Serial.printf_P("Decoder start...\n");
    delay(1000);
    decoder->begin(buff, out);
  } else {
    delete file;  //if no success delete the file
    file = NULL;
  }
  return ret;
}

//change the loudness to current gain
void setGain(float gain) {
  float v = gain / 100 * 0.5;
  out->SetGain(v);  //the parameter is the loudness as percent
  Serial.printf("New volume = %4.2f\n", v);
}
