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

TARJETA_UUID = ''
PESOS = ''
CONTENEDORES = ''

procesos = []
materiales = []

pesos = ''
asci = ''
aluminio = 0
plastico = 0
hojalata = 0
desconocido = 0
fondo = 0
material = ''



try:
    esp_master = serial.Serial('/dev/ttyUSB0',115200)
    esp_servos = serial.Serial('/dev/ttyUSB1',115200)
    esp_leds = serial.Serial('/dev/ttyUSB2',115200)
    esp_motores = serial.Serial('/dev/ttyUSB3',115200)
    esp_celdas = serial.Serial('/dev/ttyUSB4',115200)
    esp_ultras = serial.Serial('/dev/ttyUSB5',115200)
    esp_errores = serial.Serial('/dev/ttyUSB6',115200)
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
    global aluminio, plastico, hojalata, fondo, procesos, material, asci, bandas, material, PESOS,TARJETA_UUID
    

    #gpio.setup( BANDAS_INPUT , gpio.IN)
    # Initialize the image classification model
    base_options = core.BaseOptions(file_name='model.tflite', use_coral=False, num_threads=4)
    # Enable Coral by this setting
    classification_options = processor.ClassificationOptions(max_results=3, score_threshold=0.0)
    options = vision.ImageClassifierOptions(base_options=base_options, classification_options=classification_options)
    
    classifier = vision.ImageClassifier.create_from_options(options)

    print("HILO IA")
        
    
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
                            cv2.imwrite(saveImage('aluminio'),image)
                            aluminio+=1
                            material='A'
                            esp_master.write(material.encode())
                            esp_servos.write(material.encode())
                            esp_leds.write(material.encode())
                    
                            # esp_nextion.write(respuesta.encode(encoding='UTF-8',errors='strict'))
                            print('{} {}: {}%'.format(category.category_name,aluminio,score))
                            break
                        if category.category_name == 'plastico' and score >= 10:
                            cv2.imwrite(saveImage('plastico'),image)
                            plastico+=1
                            material='P'
                            esp_master.write(material.encode())
                            esp_servos.write(material.encode())
                            esp_leds.write(material.encode())
                            # esp_nextion.write(respuesta.encode(encoding='UTF-8',errors='strict'))
                            print('{} {}: {}%'.format(category.category_name,plastico,score))
                            break
                        if category.category_name == 'hojalata' and score >= 10:
                            cv2.imwrite(saveImage('hojalata'),image)
                            hojalata+=1
                            material= 'H'
                            esp_master.write(material.encode())
                            esp_servos.write(material.encode())
                            esp_leds.write(material.encode())
                            
                            # esp_nextion.write(respuesta.encode(encoding='UTF-8',errors='strict'))
                            print('{} {}: {}%'.format(category.category_name,hojalata,score))
                            break
                        if category.category_name == 'fondo' and score >= 50:
                            #cv2.imwrite(saveImage('fondo'),image)
                            fondo+=1
                            # esp_nextion.write(respuesta.encode(encoding='UTF-8',errors='strict'))
                            print(category.category_name + ': ' + str(fondo)+': '+ str(score) +'%')
                            break
                    
                    esp_master.flushOutput()
                    esp_servos.flushOutput()
                    esp_leds.flushOutput()
                    esp_master.flushInput()
                    esp_servos.flushInput()
                    esp_leds.flushInput()
                    esp_master.flush()
                    esp_servos.flush()
                    esp_leds.flush()                    
                    if IA_STATUS_OFF:
                        IA_STATUS_ON = False
                        TARJETA_UUID = esp_master.readline().decode('utf-8').strip()
                        print('{} {}'.format(TARJETA_UUID,PESOS))
                        gpio.output(BANDAS_OUTPUT,0)
                    
                    cap.release()
                    cv2.waitKey(0) # waits until a key is pressed
                    cv2.destroyAllWindows()
                time.sleep(1)
            except:
                logging.error("No se pudo prender la camara")
                #Mando Error de que la camara no funciona

        while IA_STATUS_OFF:
            print('RETIRE TARJETA')
            gpio.output(BANDAS_OUTPUT,0)  
            IA_STATUS_OFF = False               
       
def threadContainers() -> None:
    print('HILO CONTENEDORES')
    global CONTENEDORES
    while True:
        try:
            if CONTENEDORES == None:
                CONTENEDORES = esp_ultras.readline().decode('utf-8')
            else:
                print(CONTENEDORES)
        except:
            logging.error("ESP Contenedores mal conectada")
            #Mando error

def threadCeldas() -> None:
    global PESOS
    print('Hilo Celdas')
    while True:
        try:
            if PESOS == None:
                PESOS = esp_celdas.readline().decode('utf-8')
            else:
                print(PESOS)
        except:
            logging.error("ESP Celdas mal conectada")
            #Mando error

def threadMaster() -> None:
    global TARJETA_UUID
    print('HILO MASTER')
    while True:
        try:
            TARJETA_UUID = esp_master.readline().decode('utf-8')
            print(TARJETA_UUID)
        except:
            logging.error("ESP Contenedores mal conectada")
            #Mando error

def main():
    # run()
    gpio.setmode(gpio.BCM)
    gpio.setup( BTN_START , gpio.IN)
    gpio.setup( BTN_CLOSE , gpio.IN)
    gpio.setup( BANDAS_OUTPUT , gpio.OUT)
    
    ia = threading.Thread(target=run)
    #containers = threading.Thread(target=threadContainers)
    celdas = threading.Thread(target=threadCeldas)
    #master = threading.Thread(target=threadMaster)
    #servos = threading.Thread(target=moveServos)
    # containers = threading.Thread(target=readContainers)
    
    ia.start()
    #containers.start()
    celdas.start()
    #master.start()

if __name__ == '__main__':
    main()