from multiprocessing.pool import RUN
import sys
import time

from numpy import mat
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

SERVO_PLASTICO = 5
SERVO_ALUMINIO = 6
SERVO_HOJALATA = 26

FOTO_PLASTICO = 16
FOTO_ALUMINIO = 20
FOTO_HOJALATA = 21

BANDAS_OUTPUT = 23
BANDAS_INPUT = 24

procesos = []
materiales = []

asci = ''
aluminio = 0
plastico = 0
hojalata = 0
desconocido = 0
fondo = 0
material = 0



try:
    esp_nextion = serial.Serial('/dev/ttyUSB0',115200)
    #esp_servos = serial.Serial('/dev/ttyUSB2',115200)
    esp_servos = serial.Serial('/dev/ttyUSB1',115200)
    #esp_celdas = serial.Serial('/dev/ttyUSB4',115200)
    #esp_ultras = serial.Serial('/dev/ttyUSB0',115200)
    #esp_errores = serial.Serial('/dev/ttyUSB3',115200)
    
    logging.info("Las conexiones son correctas")
except:
    logging.error("Esp32 Desconectada")
    # mando error a el servidor

def saveImage(material):
    try:
        image_id = str(random.randint(0,10000))
        ruta = "materiales/{}/{}_{}.jpg".format(material,material,image_id)
        if exists(ruta):
            return saveImage(material)
        else:
            return ruta
    except:
        logging.error("No se pudo generar la imagen")

def run() -> None:
    global aluminio, plastico, hojalata, fondo, procesos, material, asci, bandas, material
    gpio.setup( BTN_START , gpio.IN)
    gpio.setup( BTN_CLOSE , gpio.IN)
    gpio.setup( BANDAS_OUTPUT , gpio.OUT)

    #gpio.setup( BANDAS_INPUT , gpio.IN)
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
        bandas = 0
        IA_STATUS_ON = gpio.input(BTN_START)        
        
        while IA_STATUS_ON:
            
            gpio.output(BANDAS_OUTPUT,1)
            

            IA_STATUS_OFF = gpio.input(BTN_CLOSE)
                
            cap = cv2.VideoCapture(0)
            cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
            cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)
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

                    for idx, category in enumerate(categories.classifications[0].categories):
                        score = round(category.score, 2) * 100
                        if category.category_name == 'aluminio' and score >= 10:
                            #cv2.imwrite(saveImage('aluminio'),image)
                            aluminio+=1
                            material=65
                            esp_servos.write([material])
                            # esp_nextion.write(respuesta.encode(encoding='UTF-8',errors='strict'))
                            print('{} {}: {}%'.format(category.category_name,aluminio,score))
                            break
                        if category.category_name == 'plastico' and score >= 10:
                            #cv2.imwrite(saveImage('plastico'),image)
                            plastico+=1
                            material=80
                            esp_servos.write([material])
                            # esp_nextion.write(respuesta.encode(encoding='UTF-8',errors='strict'))
                            print('{} {}: {}%'.format(category.category_name,plastico,score))
                            break
                        if category.category_name == 'hojalata' and score >= 10:
                            #cv2.imwrite(saveImage('hojalata'),image)
                            hojalata+=1
                            material=72
                            esp_servos.write([material])                            
                            # esp_nextion.write(respuesta.encode(encoding='UTF-8',errors='strict'))
                            print('{} {}: {}%'.format(category.category_name,hojalata,score))
                            break
                        if category.category_name == 'fondo' and score >= 50:
                            #cv2.imwrite(saveImage('fondo'),image)
                            fondo+=1
                            # esp_nextion.write(respuesta.encode(encoding='UTF-8',errors='strict'))
                            print(category.category_name + ': ' + str(fondo)+': '+ str(score) +'%')
                            break
                    
                    if IA_STATUS_OFF:
                        IA_STATUS_ON = False
                        gpio.output(BANDAS_OUTPUT,0)
                    
                    cap.release()
                    cv2.waitKey(0) # waits until a key is pressed
                    cv2.destroyAllWindows()
                time.sleep(2)
            except:
                logging.error("No se pudo prender la camara")
                #Mando Error de que la camara no funciona

        while IA_STATUS_OFF:
            print('RETIRE TARJETA')
            gpio.output(BANDAS_OUTPUT,0)  
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

def moveServos() -> None:

    gpio.setup(FOTO_PLASTICO, gpio.IN)
    gpio.setup(FOTO_ALUMINIO, gpio.IN)
    gpio.setup(FOTO_HOJALATA, gpio.IN)

    

    global procesos
    print('Esperando respuesta')
    while True:
        if gpio.input(FOTO_PLASTICO):
            print("PLASTICO")
            for i in range(0,len(procesos)):
                if procesos[i] == 65:
                    gpio.output(SERVO_ALUMINIO,1)
                    time.sleep(1)
                    gpio.output(SERVO_ALUMINIO,0)
                    procesos.pop()
                    break
        if gpio.input(FOTO_ALUMINIO):
            print("ALUMINIO")
            for i in range(0,len(procesos)):
                if procesos[i] == 72:
                    gpio.output(SERVO_PLASTICO,1)
                    time.sleep(1)
                    gpio.output(SERVO_PLASTICO,0)
                    procesos.pop()
                    break
        if gpio.input(FOTO_HOJALATA):
            print("HOJALATA")
            for i in range(0,len(procesos)):
                if procesos[i] == 80:
                    gpio.output(SERVO_HOJALATA,1)
                    time.sleep(1)
                    gpio.output(SERVO_HOJALATA,0)
                    procesos.pop()
                    break        
            
            
    
def main():
    # run()
    gpio.setmode(gpio.BCM)
    ia = threading.Thread(target=run)
    #servos = threading.Thread(target=moveServos)
    # containers = threading.Thread(target=readContainers)
    
    ia.start()
    #servos.start()
    # containers.start()

if __name__ == '__main__':
    main()