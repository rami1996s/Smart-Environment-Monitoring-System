#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// Wi-Fi credentials
const char* ssid = "HUAWEI";
const char* password = "11223344";

// MQTT broker details
const char* mqtt_server = "192.168.43.53";
const int mqttPort = 1883;
const char* mqtt_user = "calab@uni";
const char* mqtt_pass = "uni123456";
const char* mqtt_topic = "home/sensors";

// DHT11 setup
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Sensor", mqtt_user, mqtt_pass)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  setup_wifi();
  client.setServer(mqtt_server, mqttPort);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Read temperature and humidity
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Publish data to MQTT
  String payload = "{\"temperature\":" + String(t) + ",\"humidity\":" + String(h) + "}";
  client.publish(mqtt_topic, payload.c_str());

  delay(30000); // Send data every 30 seconds
}
