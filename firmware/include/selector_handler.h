/* ***********************************************************************
 *  SELECTOR ENCODER
 * 
 *  Handler for the custum selector encoder using interuptions.
 *  Each a change is notices by an interruption,
 *  The sequence of inputs (the set of all the end switchs) is saved.
 *  If it is different from the actual one and if the last change was more that 0.5 seconds ago
 *  then the value is sent to change the radio.
 * 
*/

namespace Selector {

  /**
   * Private namespace
  */
  namespace {
    #ifdef INPUT_PINS
      const byte inputPins[3]   = INPUT_PINS;
    #else 
      const byte inputPins[3]   = {13, 5, 2};
    #endif

    #ifdef RESET_SEQ
      int     resetSequence[5]  = RESET_SEQ;
    #else
      int     resetSequence[5]  = {0, 2, 4, 2, 0};
    #endif

    const int conversion[8]     = {8, 8, 3, 2, 4, 1, 5, 0};
    int       lastSelections[5] = {1, 1, 1, 1, 1};
    int       radio_index;
    byte      result;

    /**
     * @brief Interuption, read the inputs from the 3 end switchs and write the corresding number to radio_index.
    */
    void IRAM_ATTR readEncoderISR()
    {
        result = 0;
        for (int bit = 0; bit < 3; bit++) {
          bitWrite(result, bit, digitalRead(inputPins[bit]));
        }
        radio_index = conversion[result];
    }
  }

  /**
   * @brief Setup the selector encoder by defining the inputs as inputs.
   * Check the index of the radio at boot and update it.
  */
  void setup()
  {
    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);

    for (int p = 0; p < 3; p++)
    {
      pinMode(inputPins[p], INPUT_PULLDOWN);
      attachInterrupt(inputPins[p], readEncoderISR, CHANGE);
    }

    result = 0;
      for (int bit = 0; bit < 3; bit++) {
        Log::log(Log::LEVEL_WARNING, Log::SELECTOR, "Bit input: " + String(digitalRead(inputPins[bit])));
        bitWrite(result, bit, digitalRead(inputPins[bit]));
      }
    radio_index = conversion[result];
    lastchange = 0;
  }

  /**
   * @brief Handle the updates of the selector encoder by checking its state.
  */
  void loop()
  {
    if (radio_index != 8 && radio_index != curStation)
    {
      curStation = radio_index;
      lastchange = millis();
      Feedback::status = Feedback::MALFUNCTION;
    }

    if ((millis()-lastchange) > DELAY_SELECTOR) {
      
      Feedback::status = Feedback::READY;

      if (curStation!=actStation) {
        actStation = curStation;
        AudioPlayer::stop();

        insertLeftAndPush(lastSelections, 5, actStation);

        if (isEqual(lastSelections, resetSequence)) {
          Log::log(Log::LEVEL_WARNING, Log::SELECTOR, "Reset sequence triggered!");
          Log::log(Log::LEVEL_VERBOSE, Log::ESP, "Restarting ESP.");
          Memory::clear();
          ESP.restart();
        }
        #ifdef DEBUG
          MQTT::publish(OUT_TOPIC, "Station selected " + String(curStation));
        #endif
        Log::log(Log::LEVEL_VERBOSE, Log::SELECTOR, "Changing station to: " + String(actStation));
        AudioPlayer::play();
        lastchange = millis();
        radio_index = 8;
      } 
    }
  }
}