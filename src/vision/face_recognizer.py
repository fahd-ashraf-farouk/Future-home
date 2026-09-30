from facenet_pytorch import MTCNN, InceptionResnetV1
import warnings
import numpy as np
from PIL import Image
import cv2 as cv
import os 
import time
import yaml
warnings.filterwarnings("ignore", category=FutureWarning)

with open("configs/faceRecgnitionTraining.yaml", "r") as f:
    config = yaml.safe_load(f)

mtcnn = MTCNN(image_size=160, margin=0) 
facenet = InceptionResnetV1(pretrained="vggface2").eval()
path = config["HOME_OWNERS_DATA"]


if not os.path.exists(config["FEATURES_SAVED_PATH"]): 
    home_owners = []
    owners_features = []
    for folder in os.listdir(path):
        img_folder = os.path.join(path, folder)
        if not os.path.isdir(img_folder):
            continue
        for img_path in os.listdir(img_folder):
            if img_path.lower().endswith(('.jpg', '.jpeg', '.png')):
                img = cv.imread(os.path.join(img_folder, img_path))
                img_rgb = cv.cvtColor(img, cv.COLOR_BGR2RGB)
                face = mtcnn(img_rgb)
                if face is not None:
                    embedding = facenet(face.unsqueeze(0)).detach().numpy()[0]
                    #فوق احنا المودل بيحناج يعرف كام صوره داخله ليه ف احنا عملنا unsqueeze(0)علشان يضيف 1 فى اول خانه خالص 
                    #detach() دى علشان احنا جايبين موديل مدرب جاهز بتخليه يطلع الناتج علطول ميحسبش الخطا gradinet اللى هنعمل بيه backpropagation
                    #وف الاخر يحولهالنا ل feature array by .numpy()
                    #[0] دى علشان راجعلنا features (512, 1) احنا كحتاجين بس التمثيلات اللى هى 512 الواحد دا معناه صوره واحده
                    owners_features.append(embedding)
                    home_owners.append(folder)
    np.save(config["FEATURES_SAVED_PATH"],{"features": owners_features, "names": home_owners})
    
features = np.load(config["FEATURES_SAVED_PATH"], allow_pickle=True).item()
owners_features = features["features"]
home_owners = features["names"]



def sim(img1, img2) -> float:
    """Return the simialrty between the features of the two imgs"""
    return np.dot(img1, img2) / (np.linalg.norm(img1)*np.linalg.norm(img2)) 
def face_rec() -> str:
    """Return the name of the person if recognized"""
    cap = cv.VideoCapture(0)
    text = "UNKNOWN"
    while True:
        ret, frame = cap.read()
            break
        img = Image.fromarray(cv.cvtColor(frame, cv.COLOR_BGR2RGB)) 
        face = mtcnn(img)
        if face is not None:
            emb = facenet(face.unsqueeze(0)).detach().numpy()[0]
            best_siml = 0 
            named = "UNKNOWN"
            for embs, names in zip(owners_features, home_owners): 
                smil = sim(embs, emb)
                if smil>best_siml:
                    best_siml = smil
                    named = names   
            if best_siml > 0.7:
                text = f"{named}"
                color = (0, 255, 0)
            else:
                text = "unknown"
                color = (0, 0, 255)
            cv.putText(frame, text, (30, 50), cv.FONT_HERSHEY_SIMPLEX, 1, color, 2)
        cv.imshow("face_recognition", frame)
        time.sleep(5)
        stop_var = 3
        if cv.waitKey(1) & stop_var == 3 :
            cap.release()
            cv.destroyAllWindows()
            return text.lower()
            break

def start() -> str:
    """Start the face recognition process"""
    faces = face_rec()
    return str(faces)



if __name__ == "__main__":
    """Run the face recognition process"""
    start()