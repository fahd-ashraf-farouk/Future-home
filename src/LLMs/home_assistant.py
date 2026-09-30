import requests  
import json     
import os 
import yaml
with open("Configs/home_assistant.yaml", "r") as f:
    home_assistant_config = yaml.safe_load(f)
api_key = os.getenv("API_KEY")


class homeassistant:
    """Home assistant for the embedded LLMs"""
    def __init__(self, api_key):
        self.api_key = api_key
        self.url = "https://api.groq.com/openai/v1/chat/completions"
        self.header = { 
            "authorization": f"bearer {api_key}",
            "content-type": "application/json"
        }
        self.model = home_assistant_config["MODEL_NAME"]
        self.temperature = home_assistant_config["TEMPERATURE"]
        self.max_tokens = home_assistant_config["MAX_TOKENS"]
        
    def ask_question(self, question:str) -> str: 
        """get user question and return the answer"""
        try:
            payload = {
                "messages": [{"role": "user", "content": question}], 
                "model": self.model, 
                "temperature": self.temperature, 
                "max_tokens": self.max_tokens 
            }
            response = requests.post(self.url, headers=self.header, json=payload, timeout=30)
            if response.status_code == 200:
                data = response.json()  
                answer = data['choices'][0]['message']['content'] 
            else:
                return f"response status: {response.status_code}
        except Exception as e:
            return f"error {e}"






