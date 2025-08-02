#include "BluetoothSerial.h"

// Create Bluetooth Serial object
BluetoothSerial SerialBT;

// Define the fan pin
const int fanPin = 5;  // Digital pin connected to the fan

void setup() {
  // Start serial communication for debugging
  Serial.begin(115200);

  // Initialize Bluetooth with a device name
  if (!SerialBT.begin("ESP32_Fan_Control")) {  // Bluetooth device name
    Serial.println("Bluetooth initialization failed!");
    return;
  }

  Serial.println("Bluetooth initialized and ready to pair.");

  // Set the fan pin as output
  pinMode(fanPin, OUTPUT);
  digitalWrite(fanPin, LOW);  // Initially, the fan is off
}

void loop() {
  // Check if there are any incoming messages
  if (SerialBT.available()) {
    String command = SerialBT.readStringUntil('\n');
    command.trim();  // Remove any extra whitespace

    // Control the fan based on the received command
    if (command == "ACTIVATE") {
      digitalWrite(fanPin, HIGH);  // Turn on the fan
      Serial.println("Fan Status: ON");
    } else if (command == "DEACTIVATE") {
      digitalWrite(fanPin, LOW);  // Turn off the fan
      Serial.println("Fan Status: OFF");
    }
  }
}
