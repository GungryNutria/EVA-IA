# IMPOAR LIBRERIAS
from curses.ascii import NUL
from multiprocessing.pool import RUN
import sys
import time
from numpy import mat
import serial, os
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

#CONFIGURAR UN LOG BASICO PARA LA IA
logging.basicConfig(filename='eva.log', filemode='w', format='%(name)s - %(levelname)s - %(message)s')

# PINES DEL BOTON DE START Y STOP
BTN_START = 17
BTN_CLOSE = 27

# PIN PARA MANDAR PULSO ALTO A LAS BANDAS
BANDAS_OUTPUT = 23

# VARIABLE  PARA GUARDAR LA TARJETA
TARJETA_UUID = ''

# CONTADORES DE MATERIAL
aluminio = 0
plastico = 0
hojalata = 0
desconocido = 0
fondo = 0

# VARIABLE PARA GUARDAR CARACTER QUE MANDA INSTRUCCIONES
material = ''



try:
    # CONFIGURAR SERIALES
    esp_master = serial.Serial('/dev/ttyUSB0',115200)
    esp_servos = serial.Serial('/dev/ttyUSB1',115200)
    esp_leds = serial.Serial('/dev/ttyUSB2',115200)
    logging.info("Las conexiones son correctas")
except:
    logging.error("Esp32 Desconectada")
    # MANDAR ERROR AL SERVIDOR

def getTarjeta():
    # METODO PARA OBTENER EL ID DEL ESP MAESTRO
    palabra = ""
    # LECTURA DE SERIAL MAESTRO
    rawString = str(esp_master.readline().strip())
    # EXTRACCION DE ID TARJETA
    for i in rawString:
        if i.isalpha():
            continue
        else:
            if i == '=':
                continue
            else:
                if i != "'":
                    palabra += i
    # DEVOLUCION DE ID
    return palabra
        
def saveImage(material):
    # CODIGO PARA GENERAR EL NOMBRE DE UNA IMAGEN PARA CADA MATERIAL DETECTADO
    try:
        # GENERADOR DE ID UNICO PARA LA IMAGEN
        image_id = str(random.randint(0,10000))
        # RUTA CON EL ID Y EL TIPO DE MATERIAL
        ruta = "materiales/{}/{}_{}.jpg".format(material,material,image_id)
        # SI EXISTE LA RUTA VUELVE A GENERAR OTRO NOMBRE
        if exists(ruta):
            return saveImage(material)
        # SINO DEVULVE LA RUTA
        else:
            return ruta
    except:
        logging.error("No se pudo generar la imagen")
# METODO QUE CORRE LA INTELIGENCIA ARTIFICIAL
def run() -> None:
    global aluminio, plastico, hojalata, fondo, procesos, material, asci, bandas, material, PESOS,TARJETA_UUID
    
    # INICIALIZACION DEL MODELO DE CLASIFICACION DE IMAGENES
    # ASIGNACION DEL MODELO DE APRENDICAJE Y NUMERO DE HILOS CORRERA
    base_options = core.BaseOptions(file_name='model.tflite', use_coral=False, num_threads=4)
    # ASIGNACION DE NUMERO DE RESULTADOS QUE SE QUIEREN MOSTRAR Y SU PROMEDIO DE ACEPTACION
    classification_options = processor.ClassificationOptions(max_results=3, score_threshold=0.0)
    # ASIGNACION DE CONFIGURACIONES PASADAS
    options = vision.ImageClassifierOptions(base_options=base_options, classification_options=classification_options)

    #CREACION DEL CLASIFICADOR
    classifier = vision.ImageClassifier.create_from_options(options)

    print("ESPERANDO RESPUESTA")
    # BANDERAS PARA SABER SI LA IA ESTA PRENDIDA O APAGADA
    IA_STATUS_ON = False
    IA_STATUS_OFF = False
        
    while True:
        bandas = 0
        IA_STATUS_ON = gpio.input(BTN_START)
        # SI SE RECIBE UN PULSO ALTO DEL ESP MAESTRO SE MANDARA A ACTUALIZAR EL ESTATUS DE LA MAQUINA A EL SERVIDOR
        #if IA_STATUS_ON:
            #os.system("java -jar Reciclador-comando.jar status 1")
        #MIENTRAS EL ESTATUS SEA DE ENCENDIDO 
        while IA_STATUS_ON:
            # MANDAR A CORRER LAS BANDAS  
            gpio.output(BANDAS_OUTPUT,1)

            # POR CADA CICLO SE MONITOREARA CUANDO DEN CLIC AL BOTON DE STOP PARA PARAR EL SISTEMA
            IA_STATUS_OFF = gpio.input(BTN_CLOSE)
            
            #SELECCION DE CAMARA Y LE ASIGNAREMOS SUS DIMENSIONES PARA QUE LA CAMARA TENGA SU VISION
            cap = cv2.VideoCapture(0)
            cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
            cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)
            try:
                #MIENTRAS LA CAMARA ESTE PRENDIDA
                while cap.isOpened():
                    # INICIAMOS CAPTURANDO LA ENTRADA DE VIDEO DE LA CAMARA
                    success,image = cap.read()
                    # SI LA CAMARA NO FUNCIONA CERRARA TODO EL SISTEMA
                    if not success:
                        sys.exit('ERROR: Unable to read from webcam. Please verify your webcam settings.')
                    
                    # VECTORIZAMOS LA CALIDAD DE IMAGEN A RGB
                    rgb_image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
                    #INICIAR UN TENSORIMAGE CON EL RGB DE LA IMAGEN
                    tensor_image = vision.TensorImage.create_from_array(rgb_image)
                    # RECIBIR UNA LISTA DE RESULTADOS DE LA CLASIFICACION
                    categories = classifier.classify(tensor_image)

                    # EVALUACION EN UN FOR LAS CATEGORIAS
                    for idx, category in enumerate(categories.classifications[0].categories):
                        # GENERAMOS EL SCORE
                        score = round(category.score, 2) * 100
                        # SI EL MATERIAL ES ALUMINIO Y SU PORCENTAJE ES MAYOR AL 60% DE ACERTACION
                        if category.category_name == 'aluminio' and score >= 60:
                            # TOMAR FOTO AL MATERIAL
                            cv2.imwrite(saveImage('aluminio'),image)
                            # INCREMENTAR EL CONTADOR DE ALUMINIO
                            aluminio+=1
                            fondo = 0
                            # MANDAR UN CARACTER A LAS ESP PARA DARLES INSTRUCCIONES DE QUE COMPONENTES MOVER
                            material='A'
                            # esp_master.write(material.encode())
                            # esp_servos.write(material.encode())
                            # esp_leds.write(material.encode())
                            break
                        # SI EL MATERIAL ES PLASTICO Y SU PORCENTAJE ES MAYOR AL 60% DE ACERTACION
                        if category.category_name == 'plastico' and score >= 10:
                            # TOMAR FOTO AL MATERIAL
                            cv2.imwrite(saveImage('plastico'),image)
                            # INCREMENTAR EL CONTADOR DE PLASTICO
                            plastico+=1
                            fondo = 0
                            # MANDAR UN CARACTER A LAS ESP PARA DARLES INSTRUCCIONES DE QUE COMPONENTES MOVER
                            material='P'
                            # esp_master.write(material.encode())
                            # esp_servos.write(material.encode())
                            # esp_leds.write(material.encode())
                            break
                        # SI EL MATERIAL ES PLASTICO Y SU PORCENTAJE ES MAYOR AL 60% DE ACERTACION
                        if category.category_name == 'hojalata' and score >= 10:
                            # TOMAR FOTO AL MATERIAL
                            cv2.imwrite(saveImage('hojalata'),image)
                            # INCREMENTAR EL CONTADOR DE HOJALATA
                            hojalata+=1
                            fondo = 0
                            # MANDAR UN CARACTER A LAS ESP PARA DARLES INSTRUCCIONES DE QUE COMPONENTES MOVER
                            material= 'H'
                            # esp_master.write(material.encode())
                            # esp_servos.write(material.encode())
                            # esp_leds.write(material.encode())
                            break
                        # SI LA IA DETECTA EL FONDO OSEASE LA BANDA GIRANDO
                        if category.category_name == 'fondo' and score >= 50:
                            fondo+=1
                            if fondo == 10:
                                IA_STATUS_OFF = True
                                fondo = 0
                            # IMPRIMIR EN CONSOLA SOLO PARA VISUALIZAR
                            print(category.category_name + ': ' + str(fondo)+': '+ str(score) +'%')
                            break
                    # LIMPIAR EL BUFFER DE LOS SERIALES
                    # esp_master.flushOutput()
                    # esp_servos.flushOutput()
                    # esp_leds.flushOutput()
                    # esp_master.flushInput()
                    # esp_servos.flushInput()
                    # esp_leds.flushInput()
                    # esp_master.flush()
                    # esp_servos.flush()
                    # esp_leds.flush()

                    # SI ES PRESIONADO EL BOTON ROJO 
                    if IA_STATUS_OFF:
                        # CAMBIAR EL ESTATUS DE LA IA PRENDIDA A FALSO
                        IA_STATUS_ON = False
                        # MANDAR A APAGAR LAS BANDAS
                        gpio.output(BANDAS_OUTPUT,0)
                    # APAGAR CAMARA
                    cap.release()
                    cv2.waitKey(0)
                    cv2.destroyAllWindows()
                # EL SISTEMA ESPRA 1 SEGUNDO PARA QUE VUELVA A PRENDERSE LA IA
                time.sleep(1)
            except:
                # EN CASO DE QUE LA CAMARA NO PRENDA EL ERROR SE GUARDARA EN EL LOG
                logging.error("No se pudo prender la camara")
                # APAGAR DE EMERGENCIA LAS BANDAS
                gpio.output(BANDAS_OUTPUT,0)
                # APAGAR EL ESTATUS DE ENCENDIDO
                IA_STATUS_ON = False
                # CAMBIAR EL ESTATUS DE APAGADO DE LA IA A VERDADERO
                IA_STATUS_OFF = True
                # OBTENER EL ID DE LA TARJETA
                TARJETA_UUID = getTarjeta()
                # PUBLICAR SALDOS QUE SE ALCANZARON A ESCANEAR
                # os.system(f'java -jar Reciclador-comando.jar saldo {TARJETA_UUID} {0.05*plastico} {0.03*aluminio} {0.05*hojalata}')
                # CAMBIAR EL ESTATUS DE LA MAQUINA
                #os.system("java -jar Reciclador-comando.jar status 0")
                TARJETA_UUID = ''

        # MIENTRAS EL ESTATUS DE LA IA SEA APAGADO
        while IA_STATUS_OFF:
            #AMNTENER LAS BANDAS APAGADAS
            gpio.output(BANDAS_OUTPUT,0)
            # OBTENER EL ID DE LA TARJETA
            TARJETA_UUID = getTarjeta()
            # MANDAR A PUBLICAR SALDOS
            #os.system(f'java -jar Reciclador-comando.jar saldo {TARJETA_UUID} {0.02*plastico} {0.03*aluminio} {0.05*hojalata}')
            # CAMBIAR EL ESTATUS DE LA MAQUINA
            #os.system("java -jar Reciclador-comando.jar status 0")
            # LIMPIAR LA VARIABLE TARJETA
            TARJETA_UUID = ''
            # CAMBIAR EL ESTATUS DE IA CERRADA PARA QUE NO CORRA OTRO CICLO
            IA_STATUS_OFF = False

def main():
    # run()
    #CONFIGURAR EL GPIO
    gpio.setmode(gpio.BCM)
    # CONFIGURAR QUE RECIBIRE UN PULSO ALTO POR EL PIN DE BTN_START Y BTN_CLOSE
    gpio.setup( BTN_START , gpio.IN)
    gpio.setup( BTN_CLOSE , gpio.IN)
    # CONFIGURAR QUE MANDARE UN PULSO ALTO AL ESP DE LAS BANDAS
    gpio.setup( BANDAS_OUTPUT , gpio.OUT)
    
    # CORRER LA IA EN UN HILO
    ia = threading.Thread(target=run)
    # INICIAR EL HILO
    ia.start()

if __name__ == '__main__':
    #INICIO DE LA IA
    main()