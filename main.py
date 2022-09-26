
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

FOTO_PLASTICO = 16
FOTO_ALUMINIO = 20
FOTO_HOJALATA = 21

procesos = []
materiales = []
bandas = ''
asci = ''
aluminio = 0
plastico = 0
hojalata = 0
desconocido = 0
fondo = 0




try:
    esp_nextion = serial.Serial('/dev/ttyUSB1',115200)
    esp_bandas = serial.Serial('/dev/ttyUSB2',115200)
    esp_servos = serial.Serial('/dev/ttyUSB3',115200)
    
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
    global bandas, aluminio, plastico, hojalata, fondo, procesos, material, asci

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
            bandas = 65
            esp_bandas.write([bandas])
            bandas = 0

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
                        if category.category_name == 'aluminio' and score >= 60:
                            aluminio+=1
                            procesos.append(65)
                            print(category.category_name + ': ' + str(aluminio)+': '+ str(score) +'%')
                            break
                        elif category.category_name == 'plastico' and score >= 60:
                            aluminio+=1
                            procesos.append(72)
                            print(category.category_name + ': ' + str(plastico)+': '+ str(score) +'%')
                            break
                        elif category.category_name == 'hojalata' and score >= 60:
                            aluminio+=1
                            procesos.append(80)
                            print(category.category_name + ': ' + str(hojalata)+': '+ str(score) +'%')
                            break
                        elif category.category_name == 'fondo' and score >= 60:
                            aluminio+=1
                            procesos.append(65)
                            print(category.category_name + ': ' + str(fondo)+': '+ str(score) +'%')
                            break
                        else:
                            desconocido+=1
                            procesos.append(68)
                            print('desconocido: '+str(desconocido))

                                    
                    for material in materiales:
                        if material.material == "aluminio" and material.score >= 60:
                            # saveImage(material.material,image) 
                            aluminio+=1
                            procesos.append(65)
                            # respuesta = 'A'+str(aluminio)
                            # esp_servos.write([asci])
                            # esp_nextion.write(respuesta.encode(encoding='UTF-8',errors='strict'))
                            # respuesta = ''
                            print(material.material)
                            break

                        elif material.material == "hojalata" and  material.score >= 60:
                            # saveImage(material.material,image)
                            hojalata +=1
                            procesos.append(72)
                            # respuesta = 'H'+str(hojalata)
                            # esp_servos.write([asci])
                            # esp_nextion.write(respuesta.encode(encoding='UTF-8',errors='strict'))                            
                            # respuesta = ''
                            print(material.material)
                            break
                        elif material.material == "plastico" and  material.score >= 60:
                            # saveImage(material.material,image)
                            plastico += 1
                            procesos.append(80)
                            # respuesta = 'P'+str(plastico)
                            # esp_servos.write([asci])
                            # esp_nextion.write(respuesta.encode(encoding='UTF-8',errors='strict'))
                            # respuesta = ''
                            print(material.material)
                            break

                        elif material.material == "fondo" and  material.score >= 50:
                            # saveImage(material.material,image)                     
                            fondo += 1
                            print(material.material)
                            procesos.append(65)
                            # respuesta = 'A'+str(aluminio)
                            # esp_servos.write([asci])
                            # esp_nextion.write(respuesta.encode(encoding='UTF-8',errors='strict'))
                            break
                        else:
                            
                            # saveImage("desconocido",image)
                            print('desconocido')
                            # asci = 80
                            # respuesta = 'P'+str(plastico)
                            # esp_servos.write([asci])
                            # esp_nextion.write(respuesta.encode(encoding='UTF-8',errors='strict'))
                            # respuesta = ''
                            # asci = 0
                            break

                    if IA_STATUS_OFF:
                        bandas = 80
                        IA_STATUS_ON = False
                        esp_servos.bandas([bandas])
                        bandas = 0

                    cap.release()
                    cv2.waitKey(0) # waits until a key is pressed
                    cv2.destroyAllWindows()
                    time.sleep(2)
            except:
                logging.error("No se pudo prender la camara")
                #Mando Error de que la camara no funciona

        while IA_STATUS_OFF:
            print('RETIRE TARJETA')
            bandas = 80
            esp_servos.write([bandas])
            bandas = 0   
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
    bandera = 0

    while True:
        if gpio.input(FOTO_PLASTICO):
            for i in len(procesos):
                if procesos[i] == 80:
                    esp_servos.write([procesos[i]])
                    bandera = i
                    break
        elif gpio.input(FOTO_ALUMINIO):
            for i in len(procesos):
                if procesos[i] == 65:
                    esp_servos.write([procesos[i]])
                    bandera = i
                    break
        elif gpio.input(FOTO_HOJALATA):
            for i in len(procesos):
                if procesos[i] == 72:
                    esp_servos.write([procesos[i]])
                    bandera = i
                    break
        else:
            bandera = 0
            print('Esperando respuesta')

        for j in range(bandera,len(procesos)):
            if procesos[j]:
                procesos[j] = procesos[j+1]
            else:
                bandera = 0
                break
            
            
    
def main():
    # run()
    gpio.setmode(gpio.BCM)
    ia = threading.Thread(target=run)
    servos = threading.Thread(target=moveServos)
    # containers = threading.Thread(target=readContainers)
    
    ia.start()
    servos.start()
    # containers.start()

if __name__ == '__main__':
    main()