#Librerias
import serial
import time
import sys
import cv2

sys.path.append('../model')

import model.material as m

from tflite_support.task import core
from tflite_support.task import processor
from tflite_support.task import vision

#Conexiones usb
esp = serial.Serial('/dev/ttyUSB0',115200)
esp2 = serial.Serial('/dev/ttyACM0',9600)

#Clase EvaController
class EvaController:
    def __init__(self):
        self.operacion = ''
        self.aluminio = 0
        self.plastico = 0
        self.hojalata = 0
        self.fondo = 0
        self.materiales = []
        self.respuesta = 0
    
    def readTarjeta(self):
        esp_leido = str(esp.read(60))
        op = ''
        for i in range(0,len(esp_leido)):
            if esp_leido[i] == "=":
                for x in range(i+1,len(esp_leido)):
                    if esp_leido[x] == "\\":
                        break;
                    op = op + esp_leido[x]
        self.operacion = op
        
    def startCamera(self, model: str, max_results: int, score_threshold: float, num_threads: int, enable_edgetpu: bool, camera_id: int, width: int, height: int):
        
        # Initialize the image classification model
        base_options = core.BaseOptions(file_name=model, use_coral=enable_edgetpu, num_threads=num_threads)
            
        # Enable Coral by this setting
        classification_options = processor.ClassificationOptions(max_results=max_results, score_threshold=score_threshold)
        options = vision.ImageClassifierOptions(base_options=base_options, classification_options=classification_options)
                
        classifier = vision.ImageClassifier.create_from_options(options)
        cap = cv2.VideoCapture(camera_id)
        cap.set(cv2.CAP_PROP_FRAME_WIDTH, width)
        cap.set(cv2.CAP_PROP_FRAME_HEIGHT, height)
        
        while cap.isOpened():
            
            # Start capturing video input from the camera     
            success,image = cap.read()
            
            if not success:
                sys.exit('ERROR: Unable to read from webcam. Please verify your webcam settings.')
            rgb_image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
            # Create TensorImage from the RGB image
            tensor_image = vision.TensorImage.create_from_array(rgb_image)
            # List classification results
            categories = classifier.classify(tensor_image)
            
            for idx, category in enumerate(categories.classifications[0].classes):
                class_name = category.class_name
                score = round(category.score, 2) * 100
                self.materiales.append(m.Material(class_name,score))

            for material in self.materiales:
                if material.material == "aluminio":
                    self.aluminio = self.aluminio + 1
                    self.fondo =0
                    self.respuesta = 65
                    esp2.write([self.respuesta])
                    esp.write([self.respuesta])
                    print(material.material)
                    break
                elif material.material == "hojalata":
                    self.hojalata = self.hojalata + 1
                    self.fondo = 0
                    self.respuesta = 72
                    esp2.write([self.respuesta])
                    esp.write([self.respuesta])
                    print(material.material)
                    break
                                    
                elif material.material == "plastico":
                    self.plastico = self.plastico + 1
                    self.fondo = 0
                    self.respuesta = 80
                    esp2.write([self.respuesta])
                    esp.write([self.respuesta])
                    print(material.material)
                    break
                                
                elif material.material == "fondo":
                    self.fondo = self.fondo + 1
                    print(material.material+str(self.fondo))
                    self.respuesta = 70
                    esp2.write([self.respuesta])
                    esp.write([self.respuesta])
                
            cap.release()
            cv2.waitKey(0) # waits until a key is pressed
            cv2.destroyAllWindows()
            time.sleep(2)
    
    def run(self, model: str, max_results: int, score_threshold: float, num_threads: int, enable_edgetpu: bool, camera_id: int, width: int, height: int):
        while True:
            self.readTarjeta()
            if self.operacion == 'START':
                self.startCamera(model,max_results,score_threshold,num_threads,enable_edgetpu,camera_id,width,height)
            if self.operacion == 'STOP':
                print('RETIRE TARJETA')
                