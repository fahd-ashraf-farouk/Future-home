import time
from src.vision.face_recognizer import start

start_time = time.time()
end_time = 0
while (end_time-start_time) <= 120:
    name = start()
    print(name)
    time.sleep(2)
    end_time = time.time()