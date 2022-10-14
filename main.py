from curses.ascii import NUL
from multiprocessing.pool import RUN
import sys
import time
from numpy import mat
import serial, os
# os.system(comando para los servicios)
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
    esp_celdas = serial.Serial('/dev/ttyUSB3',115200)
    esp_ultras = serial.Serial('/dev/ttyUSB4',115200)
    #esp_motores = serial.Serial('/dev/ttyUSB3',115200)
    #esp_errores = serial.Serial('/dev/ttyUSB6',115200)
    logging.info("Las conexiones son correctas")
except:
    logging.error("Esp32 Desconectada")
    # mando error a el servidor

def closeSerial():
    esp_master.close()
    esp_servos.close()
    esp_leds.close()

def openSerial():
    esp_master.open()
    esp_servos.open()
    esp_leds.open()

def getTarjeta():
    while True:
        TARJETA_UUID = ''
        esp_leido = str(esp_master.readline()).strip()
        for i in range(0,len(esp_leido)):
            if esp_leido[i] == "=":
                for x in range(i+1,len(esp_leido)):
                    if esp_leido[x] == "\\":
                        break
                    TARJETA_UUID = TARJETA_UUID + esp_leido[x]
        if len(TARJETA_UUID >= 10):
            break
        else:
            print('No detecto tarjeta')
        
def getCeldas():
    palabra = ""
    rawString = str(esp_celdas.readline().strip())
    for i in rawString:
        if i.isalpha():
            continue
        else:
            if i == '=':
                continue
            else:
                if i != "'":
                    palabra += i
    return palabra
    
def getUltras():
    palabra = ""
    rawString = str(esp_ultras.readline().strip())
    for i in rawString:
        if i.isalpha():
            continue
        else:
            if i == '=':
                continue
            else:
                if i != "'":
                    palabra += i
    return palabra

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

    print("ESPERANDO RESPUESTA")
    TARJETA_UUID = getTarjeta()
    
    IA_STATUS_ON = False
    IA_STATUS_OFF = False
        
    while True:
        bandas = 0
        IA_STATUS_ON = gpio.input(BTN_START)
        
        while IA_STATUS_ON:
            if TARJETA_UUID == None:
                getTarjeta()
            else:
                print(TARJETA_UUID)   
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
                        #openSerial()
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

                    #closeSerial() 
                    if IA_STATUS_OFF:
                        IA_STATUS_ON = False
                        gpio.output(BANDAS_OUTPUT,0)
                    
                    cap.release()
                    cv2.waitKey(0) # waits until a key is pressed
                    cv2.destroyAllWindows()
                time.sleep(1)
            except:
                logging.error("No se pudo prender la camara")
                gpio.output(BANDAS_OUTPUT,0)
                IA_STATUS_ON = False
                IA_STATUS_OFF = True
                os.system(f'java -jar Reciclador-comando.jar saldo {TARJETA_UUID} {0.05*plastico} {0.03*aluminio} {0.05*hojalata}')
                TARJETA_UUID = '';
                #Mando Error de que la IA no funciona

        while IA_STATUS_OFF:
            print('RETIRE TARJETA')
            gpio.output(BANDAS_OUTPUT,0)
            PESOS = getCeldas().split(sep=" ")
            
            print(f"java -jar Reciclador-comando.jar saldo {TARJETA_UUID} {0.02*plastico} {0.03*aluminio} {0.05*hojalata}")
            os.system(f'java -jar Reciclador-comando.jar saldo {TARJETA_UUID} {0.02*plastico} {0.03*aluminio} {0.05*hojalata}')
            TARJETA_UUID = ''
            IA_STATUS_OFF = False

def main():
    # run()
    gpio.setmode(gpio.BCM)
    gpio.setup( BTN_START , gpio.IN)
    gpio.setup( BTN_CLOSE , gpio.IN)
    gpio.setup( BANDAS_OUTPUT , gpio.OUT)
    
    ia = threading.Thread(target=run)
    
    ia.start()

if __name__ == '__main__':
    main()