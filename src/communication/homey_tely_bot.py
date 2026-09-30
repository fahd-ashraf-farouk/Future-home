import requests
import os

BOT_TOKEN = os.getenv("TELEGRAM_BOT_TOKEN")
CHAT_ID = os.getenv("TELEGRAM_CHAT_ID")

def home_gas_alert(message):
    """ Send alert to telegram bot"""
    
    bot_url = f"https://api.telegram.org/bot{BOT_TOKEN}/sendMessage"

    payload = {
        "chat_id": CHAT_ID,
        "text": message,
        "parse_mode": "Markdown"
    }

    try:
        responce = requests.post(bot_url, payload)

        if responce.status_code == 200 :
            return "done"
        else : 
            return f"{responce.text}"
    

    except Exception as e:
        print(f" there is an {e} error occured")
        

if "__name__" == "__main__":
    home_gas_alert()




