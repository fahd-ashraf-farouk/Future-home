from src.LLMs.home_assistant import homeassistant
model = homeassistant()
while True:
    user_quetion = input("ask the assistant: ")
    if user_quetion == "stop":
        break
    answer = model.ask_question(user_quetion)
    print(f"assistant: {answer}")