import argparse
import sys
import time
import serial
import cv2
import tkinter as tk
from tkinter import Menu, Text, ttk
from tkinter.constants import W
import model.material as m
from tflite_support.task import core
from tflite_support.task import processor
from tflite_support.task import vision

# Visualization parameters

esp = serial.Serial('/dev/ttyUSB0',115200)
esp2 = serial.Serial('/dev/ttyACM0',9600)
plastico_ruta = "materiales/plastico"

def run(model: str, max_results: int, score_threshold: float, num_threads: int, enable_edgetpu: bool, camera_id: int, width: int, height: int) -> None:
    
    
    # Initialize the image classification model
    base_options = core.BaseOptions(file_name=model, use_coral=enable_edgetpu, num_threads=num_threads)
    
    # Enable Coral by this setting
    classification_options = processor.ClassificationOptions(max_results=max_results, score_threshold=score_threshold)
    options = vision.ImageClassifierOptions(base_options=base_options, classification_options=classification_options)
    
    classifier = vision.ImageClassifier.create_from_options(options)
    
    # Variables to calculate FPS
    counter, fps = 0, 0
    start_time = time.time()
    
    aluminio = 0
    plastico = 0
    hojalata = 0
    fondo = 0
    operacion = ""
    while True:
        
        esp_leido = str(esp.read(30))
        if operacion == "":
            for i in range(0,len(esp_leido)):
                if esp_leido[i] == "=":
                    for x in range(i+1,len(esp_leido)):
                        if esp_leido[x] == "\\":
                            break
                        operacion = operacion + esp_leido[x]
                        
        
        if(operacion == 'START'):
            
            print(operacion)
            cap = cv2.VideoCapture(camera_id)
            cap.set(cv2.CAP_PROP_FRAME_WIDTH, width)
            cap.set(cv2.CAP_PROP_FRAME_HEIGHT, height)
            respuesta = 0
            while cap.isOpened():
                    
                # Start capturing video input from the camera     
                success,image = cap.read()
                    
                if not success:
                    sys.exit('ERROR: Unable to read from webcam. Please verify your webcam settings.')
                counter += 1
                rgb_image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
                # Create TensorImage from the RGB image
                tensor_image = vision.TensorImage.create_from_array(rgb_image)
                # List classification results
                categories = classifier.classify(tensor_image)
                materiales = []
                    
                            
                for idx, category in enumerate(categories.classifications[0].classes):
                    class_name = category.class_name
                    score = round(category.score, 2) * 100
                    materiales.append(m.Material(class_name,score))
                            
                for material in materiales:
                    if material.material == "aluminio" and material.score >= 60:
                        aluminio = aluminio + 1
                        fondo =0
                        respuesta = 65
                        esp2.write([respuesta])
                        esp.write([respuesta])
                        respuesta = 0
                        print(material.material)
                        break
                    if material.material == "hojalata" and  material.score >= 60:
                        hojalata = hojalata + 1
                        fondo = 0
                        respuesta = 72
                        esp2.write([respuesta])
                        esp.write([respuesta])
                        respuesta = 0
                        print(material.material)
                        break
                                    
                    if material.material == "plastico" and  material.score >= 60:
                        plastico = plastico + 1
                        fondo = 0
                        respuesta = 80
                        esp2.write([respuesta])
                        esp.write([respuesta])
                        respuesta = 0
                        print(material.material)
                        break
                                
                    if material.material == "fondo" and  material.score >= 50:
                        if operacion == "STOP":
                            cap.release()
                        fondo = fondo + 1
                        print(material.material+str(fondo))
                        respuesta = 70
                        esp2.write([respuesta])
                        esp.write([respuesta])
                        respuesta = 0
                        if fondo == 200:
                            cap.release()
                            fondo = 0
                            operacion = "STOP"
                            print("IA Cerrada")
                
                cap.release()
                cv2.waitKey(0) # waits until a key is pressed
                cv2.destroyAllWindows()
                time.sleep(2)
                
        if operacion == "STOP":
            print('RETIRE TARJETA')
            
                
        
    
def main():
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument(
        '--model',
        help='Name of image classification model.',
        required=False,
        default='model.tflite')
    parser.add_argument(
        '--maxResults',
        help='Max of classification results.',
        required=False,
        default=3)
    parser.add_argument(
        '--scoreThreshold',
        help='The score threshold of classification results.',
        required=False,
        type=float,
        default=0.0)
    parser.add_argument(
        '--numThreads',
        help='Number of CPU threads to run the model.',
        required=False,
        default=4)
    parser.add_argument(
        '--enableEdgeTPU',
        help='Whether to run the model on EdgeTPU.',
        action='store_true',
        required=False,
        default=False)
    parser.add_argument(
        '--cameraId', help='Id of camera.', required=False, default=0)
    parser.add_argument(
        '--frameWidth',
        help='Width of frame to capture from camera.',
        required=False,
        default=1024)
    parser.add_argument(
        '--frameHeight',
        help='Height of frame to capture from camera.',
        required=False,
        default=600)
    args = parser.parse_args()
    run(args.model, int(args.maxResults),args.scoreThreshold, int(args.numThreads), bool(args.enableEdgeTPU),int(args.cameraId), args.frameWidth, args.frameHeight)

if __name__ == '__main__':
    main()
 