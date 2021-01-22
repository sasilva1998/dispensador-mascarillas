import paho.mqtt.client as mqtt
import time
import json

# ubidots credentials
UBIDOTS_TOKEN = "BBFF-5KMGC7fNayiBgauZ9TbbFtAXYkYmTt"
UBIDOTS_TOPIC = "/v1.6/devices/proyectoembebidos"
UBIDOTS_ENDPOINT = "industrial.api.ubidots.com"
mqtt_client_id = "raspberry"

# ubidots communication
ubidots_mqtt = mqtt.Client(mqtt_client_id)
ubidots_mqtt.username_pw_set(UBIDOTS_TOKEN, password="")
ubidots_mqtt.connect(UBIDOTS_ENDPOINT, port=1883)

while True:
    # data = b'{"data": %s}' % bytes(1)
    data = {"nummascarillas": 1}
    data["nummascarillas"] = 10
    payload = json.dumps(data)
    ubidots_mqtt.publish(UBIDOTS_TOPIC, payload, qos=0)
    time.sleep(1)
