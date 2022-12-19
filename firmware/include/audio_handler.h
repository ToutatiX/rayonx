/*
AUDIO HANDLER

Use the predefined link to stream an audio file.
Deal wuth the buffer and the codec.

*/
#include "WiFi.h"
#include "Audio.h"
#include "FS.h"

Audio audio(true, I2S_DAC_CHANNEL_RIGHT_EN); //

/**
 * Start playing the new url from the station list. Create a buffer and start the decoder.
*/
void startUrl() {
  audio.stopSong();
  //Serial.printf("Starting playing: %s\n", stationlist[actStation].url);
  audio.connecttohost(stationlist[actStation].url);
  Log::log(Log::LEVEL_VERBOSE, Log::AUDIO, "Starting playing: " + String(stationlist[actStation].url));
}

/**
 * Set up the memory which will be used for the buffer of the stream file and of the decoder.
 * Restart the ESP if a memore problem is encountered.
 * Setup the I2S output.s
*/
void setup_audio()
{
  //audio.setVolume(12); // 0...21
  audio.forceMono(true);
  //audio.setI2SCommFMT_LSB(true);
}

/**
 * Loop through the audio to continue blasting it. If the file is done, or the buffer reached its end, we stop by emptying it.
*/
void loop_audio()
{
  audio.loop();
  //return true;
}

  
