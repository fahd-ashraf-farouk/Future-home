import sys
import os

from vision.face_recognizer import start

from audio import voice_control
from audio import gpt_voice
from audio import control_voice
from audio.home_voice import say

from LLMs.home_assistant import homeassistant
from home_commands import home_commands

from communication.homey_tely_bot import home_gas_alert
from communication.pub import publish as pub
from communication.sub import subscriber

import time

def control(client, userdata, msg):
    print(msg.topic)
    print(msg.payload.decode())
    if msg.topic == "home/sensors/ultra_dist" :
        name = start()
        if name != "unknown" :
            pub("home/recognition", name, "recgnizer")
            time.sleep(3)
            say(f"welcome back {name}")
            
            
def gas_control(client, userdata, msg):
    
    home_gas_alert(msg.payload.decode())
    say(msg.payload.decode())    

subscriber("home/sensors/ultra_dist", control, rec_id="ultra_su")

subscriber("home/sensors/gas_detiction", gas_control, rec_id="gas_sub")


try:
    def process_command(command):
        """classifiy the user needs control home component or need help from the assistant 
    goal smothly change contol modes and if user just need to ask question he can 
    without change automatic mode of control """  

        if command in ["stop", "bye", "goodbye", "shutup", "shutdown"] :
            say("bye")
            control_voice.main(process_command, say) #to restart the lisntener
            
        if command in ["voice control", "home control"]:
            pub("home/voice_controll", "on") 
            say("home control on")
            voice_control.main(home_commands, say, pub) # control the home components
            
        elif ("ask" in command or "speak with" in command) :
            api_key = os.getenv("API_KEY")
            assistant = homeassistant(api_key)
            say("sure i am here for you") 
            def replay(question):
                answer = assistant.ask_question(question)
                say(answer)
            gpt_voice.main(replay)  # call the gpt voice function
        else:             
            say("sorry i did not understand you")
    control_voice.main(process_command, say) # call the control voice function
except:
    print("problem occured")

