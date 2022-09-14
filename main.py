
from multiprocessing.pool import RUN
import sys
import time
import serial
import cv2
import logging
import RPi.GPIO as gpio
import model.material as m
import threading
import random
from os.path import exists
from tflite_support.task import core
from tflite_support.task import processor
from tflite_support.task import vision

logging.basicConfig(filename='eva.log', filemode='w', format='%(name)s - %(levelname)s - %(message)s')

# Visualization parameters
BTN_START = 17
BTN_CLOSE = 27

try:
    esp_nextion = serial.Serial('/dev/ttyUSB0',115200)
    esp_contenedores = serial.Serial('/dev/ttyUSB1',115200)
    esp_servos = serial.Serial('/dev/ttyUSB2',115200)
    logging.info("Las conexiones son correctas")
except:
    logging.error("Esp32 Desconectada")
    # mando error a el servidor

def saveImage(material,path):
    try:
        image_id = str(random.randint(0,10000))
        ruta = "materiales/{}/{}_{}.jpg".format(material,material,image_id)
        if exists(ruta):
            saveImage(material,path)
        else:
            cv2.imwrite(ruta,path)
    except:
        logging.error("No se pudo generar la imagen")
        



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

        IA_STATUS_ON = gpio.input(BTN_START)        

        while IA_STATUS_ON:
                
            IA_STATUS_OFF = gpio.input(BTN_CLOSE)
                
            cap = cv2.VideoCapture(0)
            cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
            cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)
            respuesta = ''
            try:
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
                            saveImage(material.material,image) 
                            aluminio+=1
                            respuesta = 'A'+str(aluminio)
                            esp_servos.write(respuesta.encode(encoding='UTF-8',errors='strict'))
                            esp_nextion.write(respuesta.encode(encoding='UTF-8',errors='strict'))
                            respuesta = ''
                            print(material.material)
                            break

                        elif material.material == "hojalata" and  material.score >= 60:
                            saveImage(material.material,image)
                            hojalata +=1
                            respuesta = 'H'+str(hojalata)
                            esp_servos.write(respuesta.encode(encoding='UTF-8',errors='strict'))
                            esp_nextion.write(respuesta.encode(encoding='UTF-8',errors='strict'))                            
                            respuesta = ''
                            print(material.material)
                            break
                        elif material.material == "plastico" and  material.score >= 60:
                            saveImage(material.material,image)
                            plastico += 1
                            respuesta = 'A'+str(plastico)
                            esp_servos.write(respuesta.encode(encoding='UTF-8',errors='strict'))
                            esp_nextion.write(respuesta.encode(encoding='UTF-8',errors='strict'))
                            respuesta = ''
                            print(material.material)
                            break

                        elif material.material == "fondo" and  material.score >= 50:
                            saveImage(material.material,image)                     
                            fondo += 1
                            print(material.material)
                            respuesta = 70
                            esp_servos.write([respuesta])
                            esp_nextion.write([respuesta])
                            respuesta = 0
                        else:
                            saveImage("desconocido",image)
                            print("desconocido")
                            break

                    if IA_STATUS_OFF:
                        IA_STATUS_ON = False

                    cap.release()
                    cv2.waitKey(0) # waits until a key is pressed
                    cv2.destroyAllWindows()
                    time.sleep(2)
            except:
                logging.error("No se pudo prender la camara")
                #Mando Error de que la camara no funciona    
        while IA_STATUS_OFF:
            print('RETIRE TARJETA')
            IA_STATUS_OFF = False
    
            
def readContainers() -> None:
    while True:
        try:
            operacion = ''
            esp_leido = str(esp_contenedores.readline()).strip()
            print(esp_leido)
            for i in range(0,len(esp_leido)):
                if esp_leido[i] == "=":
                    for x in range(i+1,len(esp_leido)):
                        if esp_leido[x] == "\\":
                            break
                        operacion = operacion + esp_leido[x]
            print(operacion)
            # esp_nextion.write(operacion.encode(encoding='UTF-8',errors='strict'))
        except:
            logging.error("Alguna ESP esta mal conectada")
            #Mando error
        
    
def main():
    # run()
    ia = threading.Thread(target=run)
    containers = threading.Thread(target=readContainers)
    
    ia.start()
    containers.start()

if __name__ == '__main__':
    main()