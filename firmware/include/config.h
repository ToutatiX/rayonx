// #define DEBUG
#define BAUDRATE            115200
#define LOG_LEVEL_PRINT     4
#define BUFFER              1024*80
#define BLINKING_P          500 //ms
#define DELAY_SELECTOR      500  //ms
#define WIFI_CHECK_DELAY    1000  //ms
#define WIFI_TIMEOUT        10000 //ms
#define RESET_SEQ           {0, 2, 4, 2, 0}
#define INPUT_PINS          {9, 5, 2}
#define MQTT_BROKER         "broker.emqx.io"
#define IN_TOPIC            "cmd"
#define OUT_TOPIC           "output"
#define MQTT_USR            "rayonx"
#define MQTT_PWD            "12345678"
#define MQTT_PORT           1883
#define MAX_MQTT_TRIALS     3