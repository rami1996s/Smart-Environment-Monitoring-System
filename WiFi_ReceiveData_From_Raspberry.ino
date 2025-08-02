#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Servo.h>

Servo servoMotor;
const int servoPin = D1; // GPIO pin for the servo motor

// WiFi and MQTT settings
const char* ssid = "YourWiFiSSID";
const char* password = "YourWiFiPassword";
const char* mqtt_server = "192.168.43.53"; // Raspberry Pi IP
const char* topic = "home/actuator/servo";

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  servoMotor.attach(servoPin);
  servoMotor.write(0); // Default position

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi!");

  client.setServer(mqtt_server, 1883);
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect("ESP8266_ServoMotor")) {
      client.subscribe(topic);
      Serial.println("Connected to MQTT and subscribed to topic!");
    } else {
      delay(1000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  String command = "";
  for (int i = 0; i < length; i++) {
    command += (char)payload[i];
  }
  command.trim();

  if (command == "ACTIVATE") {
    servoMotor.write(90); // Move servo to 90 degrees
    Serial.println("Servo motor activated!");
  } else if (command == "DEACTIVATE") {
    servoMotor.write(0); // Move servo back to 0 degrees
    Serial.println("Servo motor deactivated!");
  }
}

void loop() {
  client.loop();
}
