# Smart Environment Monitoring and Control System

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

- **MQTT over Wi-Fi**: For DHT11 and Window Servo control
- **Bluetooth**: For Gas Sensor and Fan control

## Control Logic

- Python code on Raspberry Pi compares sensor data with thresholds
- Sends actuator commands via MQTT and Bluetooth
- AWS Lambda handles cloud-side logic and notifications

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


