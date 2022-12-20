/*
ROTARY ENCODER

Handler for the custum rotary encoder using interuptions.
Each a change is notices by an interruption, the sequence of inputs (the set of all the end switchs) is saved. If it is different from the actual one and if the last change was more that 0.5 seconds ago, then the value is sent to change the radio.

*/

//used pins for rotary encoder
#define ROTARY_ENCODER_A_PIN 33
#define ROTARY_ENCODER_B_PIN 32
#define ROTARY_ENCODER_BUTTON_PIN 34
#define ROTARY_ENCODER_VCC_PIN -1 /* 27 put -1 of Rotary encoder Vcc is connected directly to 3,3V; else you can use declared output pin for powering rotary encoder */


const byte inputPins[3] = {34, 35, 39};
const int conversion[8] = {7, 1, 7, 4, 0, 5, 3, 2};

int lastSelections[5] = {1, 1, 1, 1, 1};
int resetSequence[5]  = {0, 2, 4, 2, 0};

byte result;
int radio_index;

boolean rotary_value[3] = {false, false, false};

/**
 * Shift all elements of an array to the right and insert the element
*/
void insertLeftAndPush(int arr[], int n, int x)
{
  for (int i = n - 1; i > 0; i--) {
    arr[i] = arr[i - 1];
  }
  arr[0] = x;
}

/**
 * Check if 2 arrays  are equals
*/
template <typename T, int m, int n>
bool isEqual(T (&first)[m], T (&second)[n]) {
    if (m != n) {
        return false;
    }
 
    for (int i = 0; i < m; i++){
        if (first[i] != second[i]) {
            return false;
        }
    }
    return true;
}

/**
 * Interuption, read the inputs from the 3 end switchs and write the corresding number to radio_index.
*/
void IRAM_ATTR readEncoderISR()
{
    result = 0;
    for (int bit = 0; bit < 3; bit++) {
      bitWrite(result, bit, digitalRead(inputPins[bit]));
    }
    radio_index = conversion[result];
}

/**
 * Setup the rotary encoder by defining the inputs as inputs.
 * Check the index of the radio at boot and update it.
*/
void setup_rotary()
{
  for (int p = 0; p < 3; p++)
  {
    pinMode(inputPins[p], INPUT);
    attachInterrupt(inputPins[p], readEncoderISR, CHANGE);
  }

  result = 0;
    for (int bit = 0; bit < 3; bit++) {
      bitWrite(result, bit, digitalRead(inputPins[bit]));
    }
  curStation = conversion[result];
  actStation = curStation;
}

void initTime() {
  lastchange = millis();
}

/**
 * Handle the updates of the rotary encoder by checking its state.
*/
void rotary_loop()
{
  if (radio_index != 7 and radio_index != curStation)
  {
    curStation = radio_index;
    lastchange = millis();
  }
  //if no change happened within 10s set active station as current station
  if (curStation!=actStation && ((millis()-lastchange) > 500)){
    actStation = curStation;
    audio.stopSong();

    insertLeftAndPush(lastSelections, 5, actStation);

    if (isEqual(lastSelections, resetSequence)) {
      Log::log(Log::LEVEL_WARNING, Log::ROTARY, "Reset sequence triggered!");
      Log::log(Log::LEVEL_VERBOSE, Log::ESP, "Restarting ESP.");
      preferences.begin("credentials", false);
      preferences.putString("ssid", ""); 
      preferences.putString("password", ""); 
      preferences.putInt("bootAP", 1); 
      preferences.end();
      delay(100);
      ESP.restart();
    }

    //Serial.print("Changing radio to: ");
    //Serial.println(actStation);
    Log::log(Log::LEVEL_VERBOSE, Log::ROTARY, "Changing station to: " + String(actStation));
    startUrl();
    lastchange = 0;
    radio_index = 7;
  }
}