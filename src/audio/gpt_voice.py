import speech_recognition as sp 


r = sp.Recognizer()   
break_word = ["bye", "goodbye", "thank you", "close"]  #break word of the program
def main(replay): 
    """ Custom function for detect the orders for the Home assistant(LLM)
    goal get the question from the user and return the answer using the LLM 
    and if the user say any word in break_word the program will exit"""                      
    with sp.Microphone() as sorce: 
        print("started")      
        while True:              
            audio = r.listen(sorce, phrase_time_limit=7) 
            try:
                text = r.recognize_google(audio, language="en_us")
                if text in break_word: 
                    break
                replay(text) #this is the api for the model to get the answer
            except:                  
                continue

if __name__ == "__main__":
    main()

