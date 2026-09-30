#ifndef CONNECTION_H
#define CONNECTION_H
#include <WiFi.h>
#include <PubSubClient.h>
#include <Arduino.h>

typedef void (*MqttCallback)(char*, uint8_t*, unsigned int);

class connection_manager {
private:
    WiFiClient espclient;
    PubSubClient mqttclient;

    const char* wifi_ssid;
    const char* wifi_password;
    const char* mqtt_server;
    uint16_t mqtt_port;
    MqttCallback message_callback;
    const char* mqtt_clientid;
    const char* topics[24] = {
    "home/voice_controll",
    "home/home_agent",
    "home/control/awning",
    "home/control/ac",
    "home/control/room1/light",
    "home/control/room2/light",
    "home/control/room4/light",
    "home/recognition",
    "home/control/kitchen/light",
    "home/control/curtain",
    "home/control/sensors/ac",
    "home/control/sensors/temperature_humidity",
    "home/control/sensors/gases",
    "home/control/sensors/raining",
    "home/control/sensors/lightlevel",
    "home/control/sensors/awning",
    "home/control/sensors/curtain",
    "home/agentControl/room1/light",
    "home/agentControl/room3/light", 
    "home/agentControl/room2/light",
    "home/agentControl/ac",
    "home/agentControl/awning", 
    "home/agentControl/curtain", 
    "home/agentControl/kitchen/light"
    };
    static void default_callback(char* topic, byte* payload, unsigned int length) {
        Serial.print("MQTT msg [");
        Serial.print(topic);
        Serial.print("]: ");
        for (unsigned int i = 0; i < length; i++) Serial.write(payload[i]);
        Serial.println();
    }

    void reconnect_mqtt() {
        while (!mqttclient.connected()) {
            Serial.print("Attempting MQTT connection...");
            if (mqttclient.connect(mqtt_clientid)) {
                Serial.println("connected!");
                for (int i=0; i<24; i++){
                    Serial.println(topics[i]);
                    subscribe_topic(topics[i]); 
                }
            } else {
                Serial.print("failed, rc=");
                Serial.print(mqttclient.state());
                Serial.println(" Retrying in 5 seconds...");
                delay(5000);
            }
        }
    }

public:
    connection_manager(
        const char* ssid, 
        const char* password, 
        const char* clientid,
        const char* server = "PUT YOUR BROKER IP HERE", 
        MqttCallback callback = default_callback, 
        uint16_t port = 1883
    ) : 
        mqttclient(espclient),
        wifi_ssid(ssid), 
        wifi_password(password), 
        mqtt_clientid(clientid),
        mqtt_server(server), 
        mqtt_port(port),
        message_callback(callback)
    {}

    void wifi_connect() {
        WiFi.begin(wifi_ssid, wifi_password);
        Serial.print("Connecting to WiFi");
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
        }
        Serial.println();
        Serial.print("WiFi connected, IP: ");
        Serial.println(WiFi.localIP());
    }

    void mqtt_setup() {
        mqttclient.setServer(mqtt_server, mqtt_port);
        if (message_callback) {
            mqttclient.setCallback(message_callback);
        } else {
            mqttclient.setCallback(default_callback);
        }
    }

    void loop_check() {
        if (!mqttclient.connected()) {
            reconnect_mqtt();
        }
        mqttclient.loop();
    }

    bool publish_message(const char* topic, const char* message, bool retained = false) {
        if (!mqttclient.connected()){reconnect_mqtt();}
        return mqttclient.publish(topic, message, retained);
    }

    bool subscribe_topic(const char* topic) {
        if (!mqttclient.connected()) {
            reconnect_mqtt();
        }
        bool result = mqttclient.subscribe(topic);
        Serial.print("Subscribed to topic: ");
        Serial.print(topic);
        Serial.print(" -> Result: ");
        Serial.println(result ? "SUCCESS" : "FAILED");
        return result;
    }
};

#endif
