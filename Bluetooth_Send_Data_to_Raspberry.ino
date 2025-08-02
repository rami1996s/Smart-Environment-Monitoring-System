#include "BluetoothSerial.h"

// Create Bluetooth Serial object
BluetoothSerial SerialBT;

// Define the MQ-2 sensor pin and fan pin
const int mq2Pin = 34;  // Analog input pin connected to the MQ-2 sensor
const int fanPin = 5;   // Digital pin connected to the fan

// Define gas threshold to turn on the fan (adjust based on your sensor and needs)
const float gasThreshold = 1.0;  // Example threshold voltage for gas concentration

void setup() {
  // Start the serial communication with the host computer
  Serial.begin(115200);
  
  // Initialize Bluetooth with a device name
  if (!SerialBT.begin("ESP32_MQ2")) {  // Bluetooth device name
    Serial.println("Bluetooth initialization failed!");
    return;
  }
  
  Serial.println("Bluetooth initialized and ready to pair.");
  
  // Set the fan pin as output
  pinMode(fanPin, OUTPUT);
  digitalWrite(fanPin, LOW);  // Initially, the fan is off
}

void loop() {
  // Read the analog value from the MQ-2 sensor
  int sensorValue = analogRead(mq2Pin);
  
  // Convert the sensor value to a voltage (assuming a 12-bit ADC and 3.3V reference)
  float voltage = sensorValue * (3.3 / 4095.0);
  
  // Print the sensor reading to the Serial Monitor (for debugging)
  Serial.print("MQ-2 Sensor Value: ");
  Serial.println(voltage);

  // Control the fan based on the gas concentration
  if (voltage > gasThreshold) {
    digitalWrite(fanPin, HIGH);  // Turn on the fan
    SerialBT.println("Fan Status: ON");
    Serial.println("Fan ON");
  } else {
    digitalWrite(fanPin, LOW);   // Turn off the fan
    SerialBT.println("Fan Status: OFF");
    Serial.println("Fan OFF");
  }

  // Send the gas concentration and fan status over Bluetooth to the Raspberry Pi
  SerialBT.print("MQ2 Gas Concentration: ");
  SerialBT.println(voltage);  // Send the gas concentration
  
  // Delay before sending the next reading (adjust as needed)
  delay(1000);  // Send data every 1 second
}
