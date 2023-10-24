//Includes from ESP8266audio
#include "AudioFileSourceICYStream.h" //input stream
#include "AudioFileSourceBuffer.h"    //input buffer
#include "AudioGeneratorMP3.h"        //decoder
#include "AudioOutputI2S.h"           //output stream


#define LRCLK 25
#define BCLK 26
#define DOUT 33

//buffer size for stream buffering
const int preallocateBufferSize = 82*1024;  //size of preallocated buffer
const int preallocateCodecSize = 29192;     // MP3 codec max mem needed
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
  preallocateBuffer = malloc(preallocateBufferSize);          // allocate memory for stream-file-buffer
  preallocateCodec = malloc(preallocateCodecSize);            // allocate memory for decoder- buffer
  delay(1000);
  //check if we got the memory
  if (!preallocateBuffer || !preallocateCodec)
  {
    Serial.printf_P(PSTR("FATAL ERROR:  Unable to preallocate %d bytes for app\n"), preallocateBufferSize+preallocateCodecSize);
    while(1){
      yield(); // Infinite halt
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
    pref.putUShort("station",0);

    // Restart ESP when streaming is done or errored
    delay(10000);

    ESP.restart();
  }  
}

//callback function will be called if meta data were found in input stream
void MDCallback(void *cbData, const char *type, bool isUnicode, const char *string)
{
  if (strstr_P(type, PSTR("Title"))) { 
    strncpy(title, string, sizeof(title));
    title[sizeof(title)-1] = 0;
    Serial.printf(" %s\n", title);
    //show the message on the display
  newTitle = true;
  } else {
    // Who knows what to do?  Not me!
  };
}

//stop playing the input stream release memory, delete instances
void stopPlaying() {
  if (decoder)  {
    decoder->stop(); //stop playing
    delete decoder; //free decoder an its memory
    decoder = NULL; 
  }
  if (buff)  {
    buff->close(); //do the same for buffer
    delete buff;
    buff = NULL;
  }
  if (file)  {    //AND FINALLY FOR THE STREAM
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
  Serial.printf("Active station %s\n",url.c_str());
  file = new AudioFileSourceICYStream();
  ret = file->open(url.c_str());
  if (ret) {
    //register callback for meta data
    file->RegisterMetadataCB(MDCallback, NULL); 
    //create a new buffer which uses the preallocated memory
    buff = new AudioFileSourceBuffer(file, preallocateBuffer, preallocateBufferSize);
    Serial.printf_P(PSTR("sourcebuffer created - Free mem=%d\n"), ESP.getFreeHeap());
    //create and start a new decoder
    decoder = (AudioGenerator*) new AudioGeneratorMP3(preallocateCodec, preallocateCodecSize);
    Serial.printf_P(PSTR("created decoder\n"));
    Serial.printf_P("Decoder start...\n");
    delay(1000);
    decoder->begin(buff, out);
  } else {
    delete file; //if no success delete the file
    file = NULL;
  }
  return ret;
}

//change the loudness to current gain
void setGain(float gain){
  float v = (gain * gain) / 10000.0;
  out->SetGain(v);  //the parameter is the loudness as percent
  Serial.printf("New volume = %4.2f\n",v);
}
