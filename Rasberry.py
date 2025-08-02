import serial
import time
import json
import paho.mqtt.client as mqtt
from AWSIoTPythonSDK.MQTTLib import AWSIoTMQTTClient

# ===== AWS IoT Core Settings =====
MQTT_ENDPOINT = "aldxpq9gblv11-ats.iot.us-east-1.amazonaws.com"
CLIENT_ID = "RaspberryPi3"
TOPIC_AWS = "home/sensors"
ROOT_CA = "/boot/pri/AmazonRootCA1.pem"
PRIVATE_KEY = "/boot/pri/50476ee15bc04b95124802d9625f2144af01b60cd3408324521b76cca476b71e-private.pem.key"
CERTIFICATE = "/boot/pri/50476ee15bc04b95124802d9625f2144af01b60cd3408324521b76cca476b71e-certificate.pem.crt"

# ===== Local MQTT Broker Settings =====
local_mqtt_broker = "192.168.43.53"
local_mqtt_port = 1883
mqtt_topic_dht = "home/sensors/dht11"
mqtt_topic_fan = "home/actuator/fan"
mqtt_topic_servo = "home/actuator/servo"
mqtt_user = "calab@uni"
mqtt_pass = "uni123456"

# ===== Bluetooth Settings =====
bluetooth_serial_esp32_1 = '/dev/rfcomm0'  # ESP32 #1 (Gas Sensor)
bluetooth_serial_esp32_2 = '/dev/rfcomm1'  # ESP32 #2 (Fan)

# ===== Thresholds =====
temperature_threshold = 24.0
humidity_threshold = 60.0
gas_threshold = 1900

# ===== Initialize AWS IoT MQTT Client =====
def setup_aws_client():
    client = AWSIoTMQTTClient(CLIENT_ID)
    client.configureEndpoint(MQTT_ENDPOINT, 8883)
    client.configureCredentials(ROOT_CA, PRIVATE_KEY, CERTIFICATE)
    client.connect()
    return client

# ===== Initialize Bluetooth Communication =====
def setup_bluetooth(bluetooth_serial):
    try:
        ser = serial.Serial(bluetooth_serial, 115200, timeout=1)
        print(f"Connected to {bluetooth_serial}")
        return ser
    except serial.SerialException as e:
        print(f"Could not open serial port: {e}")
        return None

# ===== Local MQTT Callback for DHT11 Data =====
def on_dht_message(client, userdata, msg):
    payload_str = msg.payload.decode()
    try:
        payload = json.loads(payload_str)
        payload["device_id"] = "ESP32_1"
        print(f"Received DHT11 Data: {payload}")

        # Publish to AWS IoT
        aws_client.publish(TOPIC_AWS, json.dumps(payload), 1)
        print(f"Published DHT11 data to AWS IoT: {payload}")

        # Control ESP8266 Servo Motor
        temperature = payload.get("temperature")
        humidity = payload.get("humidity")
        if temperature and humidity:
            if temperature > temperature_threshold or humidity > humidity_threshold:
                local_mqtt_client.publish(mqtt_topic_servo, "ACTIVATE")
                print("Sent 'ACTIVATE' to Servo Motor")
            else:
                local_mqtt_client.publish(mqtt_topic_servo, "DEACTIVATE")
                print("Sent 'DEACTIVATE' to Servo Motor")
    except json.JSONDecodeError:
        print("Failed to decode MQTT message.")

# ===== Setup Local MQTT Client =====
def setup_local_mqtt():
    client = mqtt.Client()
    client.username_pw_set(mqtt_user, mqtt_pass)
    client.on_connect = lambda client, userdata, flags, rc: client.subscribe(mqtt_topic_dht)
    client.on_message = on_dht_message
    client.connect(local_mqtt_broker, local_mqtt_port, 60)
    client.loop_start()
    return client

# ===== Main Function =====
def main():
    global aws_client, local_mqtt_client
    aws_client = setup_aws_client()
    local_mqtt_client = setup_local_mqtt()

    # Setup Bluetooth communication
    bt_esp32_1 = setup_bluetooth(bluetooth_serial_esp32_1)
    bt_esp32_2 = setup_bluetooth(bluetooth_serial_esp32_2)

    while True:
        # Handle ESP32 #1 (Gas Sensor) Data
        if bt_esp32_1 and bt_esp32_1.in_waiting > 0:
            data_esp32_1 = bt_esp32_1.readline().decode('utf-8').strip()
            print(f"Received from ESP32 #1: {data_esp32_1}")

            if "Gas Value" in data_esp32_1:
                gas_value = int(data_esp32_1.split(":")[1].strip())
                gas_data = {
                    "device_id": "ESP32_1",
                    "gas_value": gas_value,
                    "timestamp": int(time.time())
                }

                # Publish to AWS IoT and Local MQTT Broker
                aws_client.publish(TOPIC_AWS, json.dumps(gas_data), 1)
                print(f"Published to AWS IoT: {gas_data}")
                local_mqtt_client.publish(mqtt_topic_dht, json.dumps(gas_data), qos=1)
                print(f"Published to Local MQTT Broker: {gas_data}")

                # Send Control Commands to ESP32 #2
                if bt_esp32_2:
                    if gas_value > gas_threshold:
                        bt_esp32_2.write("ACTIVATE\n".encode())
                        print("Sent 'ACTIVATE' to ESP32 #2 (Fan)")
                    else:
                        bt_esp32_2.write("DEACTIVATE\n".encode())
                        print("Sent 'DEACTIVATE' to ESP32 #2 (Fan)")

        time.sleep(2)

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("Program terminated.")
