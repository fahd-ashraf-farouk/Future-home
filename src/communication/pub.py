import paho.mqtt.client as mqtt
from paho.mqtt.enums import CallbackAPIVersion
import yaml

with open("configs/IOTconfig.yaml", "r") as f:
    IOTconfig = yaml.safe_load(f)



def publish(topic, msgs, pub_id="sender", br_ip=IOTconfig['IOT_broker_ip'], port=1883, duri=60):
    """IOT publisher to publish messages to the mqtt broker """
    pub = mqtt.Client(CallbackAPIVersion.VERSION2, client_id=pub_id)
    pub.connect(br_ip, port)
    pub.loop_start()
    pub.publish(topic, msgs).wait_for_publish()
    pub.loop_stop()
    pub.disconnect()

