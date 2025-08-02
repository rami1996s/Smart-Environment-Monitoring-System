# Smart Environment Monitoring and Control System
implemented with Hussein Mohammad in 2024

An IoT-based solution that automates monitoring and control of indoor air quality, temperature, and humidity using microcontrollers, sensors, actuators, and AWS cloud services.

##  System Overview

- **ESP32 (x3)**: Reads sensor data (MQ-2, DHT11) and controls fan
- **ESP8266**: Controls window using a servo motor
- **Raspberry Pi 3**: Central controller and gateway
- **AWS Cloud**:
  - **IoT Core**: Receives and manages device messages
  - **Lambda**: Processes data and triggers actions
  - **DynamoDB**: Stores sensor logs
  - **SNS**: Sends alert emails

##  Hardware Components

- MQ-2 Gas Sensor
- DHT11 Temperature & Humidity Sensor
- Fan (Actuator)
- Servo Motor (Window Control)
- ESP32, ESP8266
- Raspberry Pi 3

##  Communication Protocols

Communication Architecture

- **ESP32 (Gas Sensor):**  
  Sends gas data via **Bluetooth** to Raspberry Pi.
- **ESP32 (Temp/Humidity):**  
  Sends data via **MQTT over Wi-Fi**.
- **ESP8266 (Window Control):**  
  Receives **MQTT commands** from Raspberry Pi to open/close window.
- **ESP32 (Fan Control):**  
  Receives **Bluetooth commands** from Raspberry Pi.
- **Raspberry Pi:**  
  Compares data to thresholds and:
  - Publishes to AWS IoT Core and then send the data to the Lambda service which :
                            -  Sends alerts via AWS SNS
                            - Stores logs in DynamoDB

## Control Logic

1. **Data Collection:**
   - Gas → ESP32 → Bluetooth → Raspberry Pi
   - Temp/Humidity → ESP32 → Wi-Fi (MQTT) → Raspberry Pi

2. **Data Processing:**
   - Raspberry Pi compares values to thresholds
   - Sends control commands:
     - Fan ON (via Bluetooth) if gas level is high
     - Window OPEN (via MQTT) if temp/humidity exceeds limits

3. **Cloud Actions:**
   - Raspberry Pi publishes all data to AWS IoT
   - Lambda function:
     - Stores data in DynamoDB
     - Sends alert via SNS (email)
     - 

##  Folder Structure

- `Arduino_Code/`: C++ code for ESP32 and ESP8266
- `Raspberry_Pi_Code/`: Python controller script
- `AWS_Lambda/`: Lambda function code
- `presentation/`: Project slides
- `.gitignore`: Untracked files
- `README.md`: Documentation

##  Future Enhancements

- Add CO2 and PM2.5 sensors
- Mobile app for remote control
- Machine learning for prediction
