/*
 * ESP-Webradio
 * Following Parts:
 * - audio          : streaming and decoding audio data
 * - lcdisplay      : show station data an LCD
 * - rotary         : initialize and handle choosing senders
 * - senderlist     : senderlist by flash-values - or, if not present, by defaults
 * - senderconfig   : asynchronus webinterface to store sendernames und -urls
 *   senderconf.h   : html-template for above
 * - wlanconfig     : seperate program to get WLAN-config per webinterface
 *   wlanconf.h     : html-template for above
 *   
 *   Used libraries (included in their parts): 
 *   - ESPwebRadio  : Preferences.h
 *   - audio        : ESP8266audio (AudioFileSourceICYStream.h, AudioFileSourceBuffer.h, AudioGeneratorMP3.h, AudioOutputI2S.h)
 *   - rotary       : AiEsp32RotaryEncoder.h
 *   - senderconfig : ESPAsyncWebServer.h, Preferences.h
 *   - wlanconfig   : WiFi.h, Preferences.h, ESPAsyncWebServer.h
 */

#include <Arduino.h>
#include <Preferences.h>

#define DEBUG
#define LOG_LEVEL_PRINT   4

#define STATIONS 6 //number of available stations

struct Station{
  char url[100]; 
};

Station stationlist[STATIONS] = {
  {"http://ouifm5.ice.infomaniak.ch/ouifm5.mp3"},
  {"http://ais-live.cloud-services.paris:8000/europe1.mp3"},
  {"http://jazz.streamr.ru/jazz-64.mp3"},
  {"http://direct.franceinter.fr/live/franceinter-lofi.mp3"},
  {"http://direct.franceinter.fr/live/franceinter-midfi.mp3"},
  {"http://ouifm5.ice.infomaniak.ch/ouifm5.mp3"}
};



//global variables
uint8_t curStation = 0;   //index for current selected station in stationlist
uint8_t actStation = 0;   //index for current station in station list used for streaming 
uint32_t lastchange = 0;  //time of last selection change

Preferences preferences;

boolean play = true;

#include <log_handler.h>
#include <senderlist.h>
#include <audio_handler.h>
#include <rotary.h>
#include <wlanconfig.h>


//setup
void setup() {

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  #ifdef DEBUG
    Serial.begin(115200);
    delay(1000);
  #endif
  
  setup_senderList();
  setup_audio();
  setup_rotary();

  if (!makeWLAN()) {
    makeConfigAP();
  }

  curStation = 0;
  actStation = curStation;
  startUrl();
  initTime();
}

/**
 * Main loop of the program. If the buffer end is reached, we simply start again.
*/
void loop() {
  loop_audio();
  rotary_loop();
}