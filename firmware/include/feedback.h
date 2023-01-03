/* ***********************************************************************
 * Handle some feedbacks to help the user understanding what is going one
 * For instance, the ESP32 internal led is used to decribe any malfunction
*/

namespace Feedback {

    /**
     * Private Namespace
    */
    namespace {
        enum STATUS {
            READY,
            USER_NEED,
            MALFUNCTION
        };

        STATUS status;

        unsigned long currentMillis;
        int ledState;
        unsigned long previousMillis = 0; 
    }   

    /**
     * @brief Setup the feedback handler.
     * Setup the LED by setting the ouput
    */
    void setup() {
        pinMode(LED_BUILTIN, OUTPUT);
        digitalWrite(LED_BUILTIN, LOW);
    }

    /** 
     * @brief Loop to check for the current status and to give a feedback to the user
    */
    void loop() {
        switch (status) {
        
        /**
         * Everything works well, the radio is currently streamed.
        */
        case READY:
            digitalWrite(LED_BUILTIN, HIGH);
            break;

        /**
         * The Radio can not find any wifi netword and is running an hotspot and a server.
        */
        case USER_NEED:
            currentMillis = millis();

            if (millis() - previousMillis >= BLINKING_P) {
                previousMillis = currentMillis;

                if (ledState == LOW) {
                ledState = HIGH;
                } else {
                ledState = LOW;
                }
                digitalWrite(LED_BUILTIN, ledState);
            }
            break;
        
        /**
         * Something is wrong with the wifi connection or with the selector.
        */
        case MALFUNCTION:
            digitalWrite(LED_BUILTIN, LOW);
            break;
        }
    }
}