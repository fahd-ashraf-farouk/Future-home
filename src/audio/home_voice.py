import pyttsx3 
import numpy as np

def say(sent): 
    """ Give the home ability to speak like human with diffrent voices(man and woman)
    goal is to change the user experince by interaction with home by voice """
    model = pyttsx3.init()  #get the text to speech engine
    random_voice = np.random.randint(0,2)  #the 2 voices for the home
    voices = model.getProperty("voices")   # get the voice property
    model.setProperty("voice",voices[random_voice].id)  #set the voice type randomly (man or woman)
    model.say(sent)          
    model.runAndWait() 
