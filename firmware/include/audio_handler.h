/* ***********************************************************************
 * This is used to configure and establish a WLAN-connection
 * AUDIO HANDLER
 * 
 * Use the predefined link to stream an audio file.
 * Deal wuth the buffer and the codec.
 *  
*/

#include "WiFi.h"
#include "Audio.h"
#include "FS.h"

namespace AudioPlayer {
  Audio audio(true, I2S_DAC_CHANNEL_RIGHT_EN);

  /**
   * @brief Start playing the new url from the station list. Create a buffer and start the decoder.
  */
  void play() {
    audio.stopSong();
    if (!audio.connecttohost(stationlist[actStation].url)) { 
      Log::log(Log::LEVEL_ERROR, Log::WLAN, "Failed connecting to the host!");
      ESP.restart();
    }
    Log::log(Log::LEVEL_VERBOSE, Log::AUDIO, "Starting playing: " + String(stationlist[actStation].url));
  }

  /**
   * @brief Set up the memory which will be used for the buffer of the stream file and of the decoder.
   * Setup the I2S output.s
  */
  void setup() {
    audio.forceMono(true);
    audio.setBufsize(BUFFER, 300000);
  }

  /**
   * @brief Loop through the audio to continue blasting it. If the file is done, or the buffer reached its end, we stop by emptying it.
  */
  void loop() {
    audio.loop();
  }

  /**
   * @brief Stop playing the current file
  */
  void stop() {
    audio.stopSong();
  }
}