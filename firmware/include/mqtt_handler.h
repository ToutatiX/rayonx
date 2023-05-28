#ifdef DEBUG

#include <PubSubClient.h>

namespace MQTT {
    WiFiClient espClient;
    PubSubClient client(espClient);

    enum STATE {
        GOT_ANSWER,
        WAITING,
        UNSET,
    };

    STATE state = UNSET;
    String last_msg = "";

    void callback(char* topic, byte* payload, unsigned int length) {
        last_msg = "";
        for (int i = 0; i < length; i++) {
            last_msg += (char)payload[i];
        }
        Serial.println("Message arrived on topic: '" + String(topic) + "', got: " + last_msg);
        state = GOT_ANSWER;
    }

    bool connect() {
        int nb_trials = 0;

        while (!client.connected() && nb_trials < MAX_MQTT_TRIALS) {

            Serial.print("Attempting MQTT connection...");

            if (client.connect("dummy_id", MQTT_USR, MQTT_PWD)) {
                Serial.println("MQTT Connected");
                client.subscribe(IN_TOPIC);

            } else {
                Serial.println("Failed, state: "+String(client.state()));
            delay(500);
            nb_trials ++;
            }
        }
        if (client.connected()) {
            state = WAITING;
            return true;
        }
        Serial.println("MQTT Failed to many times");
        state = UNSET;
        return false;
    }

    bool setup() {
        client.setServer(MQTT_BROKER, MQTT_PORT);
        client.setCallback(callback);
        return connect();
    }

    bool publish(String topic, String msg) {
        if( client.publish(topic.c_str(), msg.c_str())) {
            Serial.println("Published '"+ msg +"' on: '" + topic +"'");
            return true;
        }
        Serial.println("Failed to publish.");
        return false;
    }

    STATE loop() {
        if (connect()) {
            client.loop();
        }
        return state;
    }
}

#endif