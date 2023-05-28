/* ***********************************************************************
 * AUDIO HANDLER
 * 
 * Use the predefined link to stream an audio file.
 * Deal wuth the buffer and the codec.
 *  
*/

#include "Audio.h"
#include "FS.h"

// I2S Connections
#define I2S_DOUT      22
#define I2S_BCLK      26
#define I2S_LRC       25

namespace AudioPlayer {
  Audio audio;
  uint8_t volume; 

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
   * @brief Stop playing the current file
  */
  void stop() {
    audio.stopSong();
  }

  /**
   * @brief Set the output volume
   * @param volume from 0 0% to 21 100% 
  */
  void setVolume(uint8_t v) {
    // stop();
    if (abs(v-volume)>1) {
      volume = v;
      Log::log(Log::LEVEL_VERBOSE, Log::AUDIO, "Changing volume to " + String(volume));  
      audio.setVolume(volume);
      Log::log(Log::LEVEL_VERBOSE, Log::AUDIO, "Done");  
    }
    // loop();
  }

  /**
   * @brief Set up the memory which will be used for the buffer of the stream file and of the decoder.
   * Setup the I2S output.s
  */
  void setup() {
    audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    setVolume(1);
    audio.forceMono(true);
    audio.setBufsize(BUFFER, 300000);
  }

  /**
   * @brief Loop through the audio to continue blasting it. If the file is done, or the buffer reached its end, we stop by emptying it.
  */
  void loop() {
    audio.loop();
  }
}