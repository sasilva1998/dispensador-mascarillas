import sys

from Adafruit_IO import MQTTClient
import paho.mqtt.client as mqtt


from serial_control import SerialCom

ADAFRUIT_IO_URL = "io.adafruit.com"
ADAFRUIT_USERNAME = "sasilva"
ADAFRUIT_IO_KEY = "aio_aerb53eCcM7DnWlG5JLp7Id44SEn"
ADAFRUIT_IO_FEEDNAME = "proyecto-embebidos"

UBIDOTS_TOKEN = "BBFF-5KMGC7fNayiBgauZ9TbbFtAXYkYmTt1"
UBIDOTS_TOPIC = "/v1.6/devices/proyecto-embebidos"
UBIDOTS_ENDPOINT = "mqtt://things.ubidots.com"


class CloudControl:
    def message(self, client, feed_id, payload):
        print("Feed {0} received new value: {1}".format(feed_id, payload))
        if payload == "retiro_mascarilla":
            self.serial_com.com(1, 2, 500)
            print("envio a micro para retiro mascarilla")
        elif payload == "deposito_mascarilla":
            self.serial_com.com(2, 2, 500)
            print("envio a micro para deposito mascarilla")

    def adafruit_publish(self, data):
        self.adafruit_mqtt.publish(ADAFRUIT_IO_FEEDNAME, data)

    def __init__(self, mqtt_client_id="raspberry"):

        self.serial_com = SerialCom()
        self.mqtt_client_id = mqtt_client_id

        # adafruit communication
        self.adafruit_mqtt = MQTTClient(ADAFRUIT_USERNAME, ADAFRUIT_IO_KEY)
        self.adafruit_mqtt.on_message = self.message
        self.adafruit_mqtt.on_connect = connected

        try:
            self.adafruit_mqtt.connect()
        except Exception as e:
            print("Couldn't connect to MQTT server {}{}".format(type(e).__name__, e))
            sys.exit()
        self.adafruit_mqtt.loop_background()

        # ubidots communication

        self.ubidots_mqtt = mqtt.Client(mqtt_client_id)
        self.ubidots_mqtt.connect(UBIDOTS_ENDPOINT)

    def serial_handler(self, action):
        data = b'{"data": %s}' % action[0]
        self.ubidots_mqtt.publish(UBIDOTS_ENDPOINT, data)

    def serial_listener(self):
        while True:
            arduino_inc_action = self.serial_com.listen(0)
            atmega_inc_action = self.serial_com.listen(1)

            if arduino_inc_action:
                self.serial_handler(arduino_inc_action)
            if atmega_inc_action:
                self.serial_handler(atmega_inc_action)


def connected(client):

    print(
        "Connected to Adafruit IO!  Listening for {0} changes...".format(
            ADAFRUIT_IO_FEEDNAME
        )
    )
    client.subscribe(ADAFRUIT_IO_FEEDNAME)