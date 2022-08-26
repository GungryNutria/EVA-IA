#Librerias
import serial

#Conexiones usb
esp = serial.Serial('/dev/ttyUSB0',115200)

#Clase EvaController
class EvaController:
    def __init__(self):
        self.operacion
    
    def readTarjeta(self):
        esp_leido = str(esp.read(60))
        for i in range(0,len(esp_leido)):
            if esp_leido[i] == "=":
                for x in range(i+1,len(esp_leido)):
                    if esp_leido[x] == "\\":
                        break;
                    self.operacion = self.operacion + esp_leido[x]
        return self.operacion
        