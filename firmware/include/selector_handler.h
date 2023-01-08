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
      const byte inputPins[3]   = {39, 35, 34};
    #endif

    #ifdef RESET_SEQ
      int     resetSequence[5]  = RESET_SEQ;
    #else
      int     resetSequence[5]  = {0, 2, 4, 2, 0};
    #endif

    const int conversion[8]     = {7, 1, 7, 4, 0, 5, 3, 2};
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
    for (int p = 0; p < 3; p++)
    {
      pinMode(inputPins[p], INPUT_PULLUP);
      attachInterrupt(inputPins[p], readEncoderISR, CHANGE);
    }

    result = 0;
      for (int bit = 0; bit < 3; bit++) {
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
    if (radio_index != 7 and radio_index != curStation)
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

        Log::log(Log::LEVEL_VERBOSE, Log::SELECTOR, "Changing station to: " + String(actStation));
        AudioPlayer::play();
        lastchange = 0;
        radio_index = 7;
      } 
    }
  }
}