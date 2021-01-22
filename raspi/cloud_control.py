import sys

from Adafruit_IO import MQTTClient
import paho.mqtt.client as mqtt
import json
from serial_control import SerialCom
from time import sleep

# adafruit credentials
ADAFRUIT_IO_URL = "io.adafruit.com"
ADAFRUIT_USERNAME = "sasilva"
ADAFRUIT_IO_KEY = "aio_FmzB567WayQFGBpBt14QwTxi6vAL"
ADAFRUIT_IO_FEEDNAME = "proyecto-embebidos"

# ubidots credentials
UBIDOTS_TOKEN = "BBFF-5KMGC7fNayiBgauZ9TbbFtAXYkYmTt"
UBIDOTS_TOPIC = "/v1.6/devices/proyectoembebidos"
UBIDOTS_ENDPOINT = "industrial.api.ubidots.com"


class CloudControl:
    def message(self, client, feed_id, payload):
        print("Feed {0} received new value: {1}".format(feed_id, payload))
        if payload == "3":
            print("-" * 15)
            print("ingreso de mascarilla")
            self.serial_com.com(1, 3)

        elif payload == "5":
            print("-" * 15)
            print("retiro de mascarilla")
            self.serial_com.com(2, 5, [1])

    def adafruit_publish(self, data):
        self.adafruit_mqtt.publish(ADAFRUIT_IO_FEEDNAME, data)

    def __init__(
        self,
        arduino_port="/dev/ttyS1",
        atmega_port="/dev/ttyS2",
        mqtt_client_id="raspberry",
    ):

        self.serial_com = SerialCom(arduino_port=arduino_port, atmega_port=atmega_port)
        self.mqtt_client_id = mqtt_client_id

        # adafruit communication
        self.adafruit_mqtt = MQTTClient(ADAFRUIT_USERNAME, ADAFRUIT_IO_KEY)
        self.adafruit_mqtt.on_message = self.message
        self.adafruit_mqtt.on_connect = connected

        self.data = {"numMascarillas": 1}
        self.data_json = json.dumps(self.data)

        try:
            self.adafruit_mqtt.connect()
        except Exception as e:
            print("Couldn't connect to MQTT server {}{}".format(type(e).__name__, e))
            sys.exit()
        self.adafruit_mqtt.loop_background()

        # ubidots communication
        self.ubidots_mqtt = mqtt.Client(self.mqtt_client_id)
        self.ubidots_mqtt.username_pw_set(UBIDOTS_TOKEN, password="")
        self.ubidots_mqtt.connect(UBIDOTS_ENDPOINT)

    def serial_handler(self, action):
        print("##############action###############")
        print(action)
        if action[2] == 0x06:
            print("accion banda")
            self.serial_com.com(2, 0x04, [1])
        if action[2] == 0x02:
            print("enviando numero de mascarillas a ubidots")
            self.data["numMascarillas"] = action[3]
            self.data_json = json.dumps(self.data)
            # self.ubidots_mqtt.publish(UBIDOTS_TOPIC, data)
            sleep(1)
        if action[2] == 0x07:
            print("comunicando a ubidots mascarillas agotadas")
            self.data["numMascarillas"] = 0
            self.data_json = json.dumps(self.data)
            # self.ubidots_mqtt.publish(UBIDOTS_TOPIC, data)
            sleep(1)

    def serial_listener(self):
        while True:
            arduino_inc_action = self.serial_com.listen(1)
            atmega_inc_action = self.serial_com.listen(0)

            if arduino_inc_action:
                self.serial_handler(arduino_inc_action)
            if atmega_inc_action:
                self.serial_handler(atmega_inc_action)

    def mqtt_publisher(self):
        while True:
            self.ubidots_mqtt.publish(UBIDOTS_TOPIC, self.data_json)
            sleep(1)


def connected(client):

    print(
        "Connected to Adafruit IO!  Listening for {0} changes...".format(
            ADAFRUIT_IO_FEEDNAME
        )
    )
    client.subscribe(ADAFRUIT_IO_FEEDNAME)
