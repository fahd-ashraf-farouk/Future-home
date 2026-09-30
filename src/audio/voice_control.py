import speech_recognition as sp



r = sp.Recognizer()
break_words = ["bye", "goodbye", "done", "close", "off"]
def main(home_command, say, pub):
    print("started")
    with sp.Microphone() as sorce:
        while True:
            audio = r.listen(sorce, phrase_time_limit = 5)
            try:
                word = r.recognize_google(audio, language = "en_us")
                print(word)
                if word in break_words:
                    pub("home/voice_controll", "off")
                    break
                home_command(word, say, pub)
                
            except:
                continue



if __name__ == "__main__":
    main()

