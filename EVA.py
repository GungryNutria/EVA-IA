
from multiprocessing.pool import RUN
import sys
import time
import serial
import cv2
import RPi.GPIO as gpio
import model.material as m
import threading
import random
from tflite_support.task import core
from tflite_support.task import processor
from tflite_support.task import vision

# Visualization parameters
BTN_START = 17
BTN_CLOSE = 27

esp = serial.Serial('/dev/ttyUSB0',115200)
esp2 = serial.Serial('/dev/ttyUSB1',115200)

ruta_fondo = "materiales/fondo/"
ruta_plastico = "materiales/plastico/"
ruta_aluminio = "materiales/aluminio/"
ruta_hojalata = "materiales/hojalata/"
ruta_desconocido = "materiales/desconocido/"

caracteres = {"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z","1","2","3","4","5","6","7","8","9","0"}



def generateImageName(m):
    nombre_imagen += m+"_"
    
    for x in range(6):
        nombre_imagen += caracteres[random.randint(0,len(caracteres)-1)]
    return nombre_imagen


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
    nombre_imagen = ""
    
    while True:

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
                        nombre_imagen = nombre_imagen + material
                        nombre_imagen = nombre_imagen + "_"
                        for x in range(6):
                            nombre_imagen = nombre_imagen + caracteres[random.randint(0,len(caracteres)-1)]
                        
                        cv2.imwrite(ruta_aluminio+""+nombre_imagen+".jpg",image)

                        respuesta = 65
                        respuesta = 0
                        print(material.material)
                        break

                    elif material.material == "hojalata" and  material.score >= 60:
                        nombre_imagen = nombre_imagen + material
                        nombre_imagen = nombre_imagen + "_"
                        for x in range(6):
                            nombre_imagen = nombre_imagen + caracteres[random.randint(0,len(caracteres)-1)]
                        
                        cv2.imwrite(ruta_aluminio+""+nombre_imagen+".jpg",image)
                        hojalata = hojalata + 1
                        fondo = 0
                        respuesta = 72
                        #esp2.write([respuesta])
                        #esp.write([respuesta])
                        respuesta = 0
                        print(material.material)
                        break
                                    
                    elif material.material == "plastico" and  material.score >= 60:
                        nombre_imagen = nombre_imagen + material
                        nombre_imagen = nombre_imagen + "_"
                        for x in range(6):
                            nombre_imagen = nombre_imagen + caracteres[random.randint(0,len(caracteres)-1)]
                        
                        cv2.imwrite(ruta_aluminio+""+nombre_imagen+".jpg",image)
                        plastico = plastico + 1
                        fondo = 0
                        respuesta = 80
                        #esp2.write([respuesta])
                        #esp.write([respuesta])
                        respuesta = 0
                        print(material.material)
                        break

                    elif material.material == "fondo" and  material.score >= 50:
                        nombre_imagen = nombre_imagen + material
                        nombre_imagen = nombre_imagen + "_"
                        for x in range(6):
                            nombre_imagen = nombre_imagen + caracteres[random.randint(0,len(caracteres)-1)]
                        
                        cv2.imwrite(ruta_aluminio+""+nombre_imagen+".jpg",image)
                        fondo = fondo + 1
                        print(material.material)
                        respuesta = 70
                        #esp2.write([respuesta])
                        #esp.write([respuesta])
                        respuesta = 0
                    else:
                        nombre_imagen += "desconocido_"
                        for x in range(6):
                            nombre_imagen += caracteres[random.randint(0,len(caracteres)-1)]
                        
                        cv2.imwrite(ruta_aluminio+""+nombre_imagen+".jpg",image)
                        print("Desconocido")
                        break

                if IA_STATUS_OFF:
                    IA_STATUS_ON = False

                cap.release()
                cv2.waitKey(0) # waits until a key is pressed
                cv2.destroyAllWindows()
                time.sleep(2)
                    
        while IA_STATUS_OFF:
            print('RETIRE TARJETA')
            esp2.close()
            IA_STATUS_OFF = False
            
def readContainers() -> None:
    while True:
        contenedores = []
        operacion = ''
        esp_leido = str(esp.readline()).strip()
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