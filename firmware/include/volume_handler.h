/* ***********************************************************************
 * VOLUME HANDLER
 * 
 * Use to deal with the potentiometer and set the volume
 *  
*/

#define POTENTIOMETER_Vin 10
#define POTENTIOMETER_Val 35
#define POTENTIOMETER_Gnd 27


namespace Volume {
    
    void setup() {
        pinMode(POTENTIOMETER_Val, INPUT);
        pinMode(POTENTIOMETER_Vin, OUTPUT);
        pinMode(POTENTIOMETER_Gnd, OUTPUT);

        digitalWrite(POTENTIOMETER_Vin, HIGH);
        digitalWrite(POTENTIOMETER_Gnd, LOW);

        // attachInterrupt(POTENTIOMETER_Val, VolumeInterrupt, CHANGE);
    }

    uint8_t get() {
        return map(analogRead(POTENTIOMETER_Val), 0, 4095, 21, 0);
    }

}