import numpy as np
import speech_recognition as sp #google spech recognition library

r = sp.Recognizer()  
wake_word = "hey home" #wake word of the program

def listner():       
    """This function is used to listen to the user's voice and return the text
    goal to process the comands for control the home components"""
    with sp.Microphone() as sorce: #microfone starting 
        
        audio = r.listen(sorce, phrase_time_limit=5) #start deticting the user voice
        try: 
            text = r.recognize_google(audio, language="en_us") #recognize the voice
            return text.lower()     
        except:                  
            return ""

def main(process_command, say):     
    """ Continuosly listens for the wake word and process commands 
    goal to check if wake up word is detected and save the microphone always open for listening""" 
          
    with sp.Microphone() as sorce: 
        print("started")         
        while True:              #loop for ever to check if we need anything
            audio = r.listen(sorce, phrase_time_limit=3) 
            try:
                responses =["hello", "hey", "what", "listening"] # AI respones 
                response = np.random.choice(responses)
                text = r.recognize_google(audio, language="en_us")
                print(text)
                if wake_word in text: #wake word of the program
                    say(response)  #speak AI response 
                    command = listner()                    
                    process_command(command) #process human orders
            except:                   
                "problem"
                continue



if __name__ == "__main__":
    main()

