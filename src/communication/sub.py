import paho.mqtt.client as mqtt
from paho.mqtt.enums import CallbackAPIVersion
import yaml

with open("configs/IOTconfig.yaml", "r") as f:
    IOTconfig = yaml.safe_load(f)

def subscriber(topic,on_message,rec_id="reciver", br_ip=IOTconfig['IOT_broker_ip'], port=1883):
    """IOT subscriber to subscribe to the mqtt broker"""
    sub = mqtt.Client(CallbackAPIVersion.VERSION2, client_id=rec_id)
    sub.on_message = on_message
    sub.connect(br_ip, port)
    sub.subscribe(topic)
    sub.loop_start() 
    return sub

