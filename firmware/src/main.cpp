/*
 * ESP-Webradio
 * Following Parts:
 * - audio_handler  : streaming and decoding audio data
 * - rotary         : initialize and handle choosing senders
 * - interface      : html of the captive portal
 * - wlanconf       : handle the wifi connection and the access point
 * - log_handler    : log debug in serial
 * 
 *   
 *   Used libraries (included in their parts): 
 *   - ESP32-audioI2S     (https://github.com/schreibfaul1/ESP32-audioI2S/wiki)
 *   - ESPAsyncWebServer
 */

#include <Arduino.h>

struct Station{
  char url[256]; 
};

Station stationlist[6] = {
  {"http://windu.radionotredame.net/RadioNotreDame-Fm.mp3"},
  {"http://ais-live.cloud-services.paris:8000/europe1.mp3"},
  {"http://icecast.radiofrance.fr/franceinfo-midfi.mp3"},
  {"http://radioclassique.ice.infomaniak.ch/radioclassique-high.mp3"},
  {"http://stream.live.vc.bbcmedia.co.uk/bbc_radio_three"},
  {"http://icecast.radiofrance.fr/franceculture-lofi.mp3"}
};

uint8_t curStation = -1;   
uint8_t actStation = -1;   
uint32_t lastchange = 0; 

#include <config.h>
#include <utils.h>
#include <log_handler.h>
#include <feedback.h>
#include <memory.h>
#include <audio_handler.h>
#include <wlan_handler.h>
#include <mqtt_handler.h>
#include <selector_handler.h>
#include <volume_handler.h>

/**
 * @brief MainSetup
*/
void setup() {
  Log::setup();
  Feedback::setup();

  if (Wlan::makeWLAN()) {
    Selector::setup();
    AudioPlayer::setup();
    Volume::setup();
    Feedback::status = Feedback::READY;
    #ifdef DEBUG
      MQTT::setup();
    #endif
  }
  else {
    Wlan::createAP();
    Feedback::status = Feedback::USER_NEED;
  }
}

/**
 * @brief Main loop of the program. If the buffer end is reached, we simply start again.
 * Run the feedback loop which updates the LED status
 * Then:
 * - if the radio is in AP mode, run the Wlan AP loop which will check for DNS updates
 * - if the radio is in STA mode, check the selector status and update the current radio if needed
 *   loop through the audio,
*/
void loop() {
  Feedback::loop();

  if (Wlan::mode == Wlan::AP) {
    Wlan::loopAP();
  }
  else {
    if (Wlan::loopSTA()) {
      Selector::loop();
      AudioPlayer::loop();

      if (millis() % 20 == 0) {
        AudioPlayer::setVolume(Volume::get());
      }
    } 
  }
}