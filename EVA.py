import argparse
from multiprocessing.pool import RUN
import sys
import time
import serial
import cv2
import RPi.GPIO as gpio
import model.material as m
import threading
from tflite_support.task import core
from tflite_support.task import processor
from tflite_support.task import vision

# Visualization parameters


BTN_START = 17
BTN_CLOSE = 27

esp = serial.Serial('/dev/ttyUSB0',115200)
esp2 = serial.Serial('/dev/ttyUSB1',115200)

    
def run() -> None:
    aluminio = 0
    plastico = 0
    hojalata = 0
    fondo = 0
    gpio.setmode(gpio.BCM)

    gpio.setup( BTN_START , gpio.IN)
    gpio.setup( BTN_CLOSE , gpio.IN)
    
    # Initialize the image classification model
    base_options = core.BaseOptions(file_name='model.tflite', use_coral=False, num_threads=4)
    
    # Enable Coral by this setting
    classification_options = processor.ClassificationOptions(max_results=3, score_threshold=0.0)
    options = vision.ImageClassifierOptions(base_options=base_options, classification_options=classification_options)
    
    classifier = vision.ImageClassifier.create_from_options(options)
    print("Esperando respuesta")
    IA_STATUS_ON = False
    IA_STATUS_OFF = False

    while True:
        
        # esp_leido = str(esp.read(30))
        # if operacion == "":
        #     for i in range(0,len(esp_leido)):
        #         if esp_leido[i] == "=":
        #             for x in range(i+1,len(esp_leido)):
        #                 if esp_leido[x] == "\\":
        #                     break
        #                 operacion = operacion + esp_leido[x]

        IA_STATUS_ON = gpio.input(BTN_START)
        

        while IA_STATUS_ON:
            IA_STATUS_OFF = gpio.input(BTN_CLOSE)
            
            cap = cv2.VideoCapture(0)
            cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
            cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)
            respuesta = 0
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
                materiales = []
                            
                for idx, category in enumerate(categories.classifications[0].categories):
                    category_name = category.category_name
                    score = round(category.score, 2) * 100
                    materiales.append(m.Material(category_name,score))
                            
                for material in materiales:
                    if material.material == "aluminio" and material.score >= 60:
                        aluminio = aluminio + 1
                        fondo =0
                        respuesta = 65
                        # cambiar led
                        # Mover Servo
                        # 
                        #esp2.write([respuesta])
                        #esp.write([respuesta])
                        respuesta = 0
                        print(material.material)
                        break

                    if material.material == "hojalata" and  material.score >= 60:
                        hojalata = hojalata + 1
                        fondo = 0
                        respuesta = 72
                        #esp2.write([respuesta])
                        #esp.write([respuesta])
                        respuesta = 0
                        print(material.material)
                        break
                                    
                    if material.material == "plastico" and  material.score >= 60:
                        plastico = plastico + 1
                        fondo = 0
                        respuesta = 80
                        #esp2.write([respuesta])
                        #esp.write([respuesta])
                        respuesta = 0
                        print(material.material)
                        break
                                
                    if material.material == "fondo" and  material.score >= 50:
                        fondo = fondo + 1
                        print(material.material+str(fondo))
                        respuesta = 70
                        #esp2.write([respuesta])
                        #esp.write([respuesta])
                        respuesta = 0
                        if fondo == 200:
                            cap.release()
                            fondo = 0                        
                            print("IA Cerrada")
                
                if IA_STATUS_OFF:
                    print('RETIRE TARJETA')

                cap.release()
                cv2.waitKey(0) # waits until a key is pressed
                cv2.destroyAllWindows()
                time.sleep(2)
            
def readContainers() -> None:
    while True:
        operacion = ''
        esp_leido = str(esp.readline())
        for i in range(0,len(esp_leido)):
            if esp_leido[i] == "=":
                for x in range(i+1,len(esp_leido)):
                    if esp_leido[x] == "\\":
                        break
                    operacion = operacion + esp_leido[x]
        print(operacion)
        esp2.write(operacion.encode(encoding='UTF-8',errors='strict'))
        
    
def main():
    # run()
    ia = threading.Thread(target=run)
    containers = threading.Thread(target=readContainers)
    
    ia.start()
    containers.start()

if __name__ == '__main__':
    main()
 