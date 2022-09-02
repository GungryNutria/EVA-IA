import serial
esp = serial.Serial('/dev/ttyUSB0',115200)
esp2 = serial.Serial('/dev/ttyUSB1',115200)
 
while True:
    operacion = ''
    esp_leido = str(esp.readline())
    for i in range(0,len(esp_leido)):
        if esp_leido[i] == "=":
            for x in range(i+1,len(esp_leido)):
                if esp_leido[x] == "\\":
                    break
                operacion = operacion + esp_leido[x]
    esp2.write(operacion.encode(encoding='UTF-8',errors='strict'))
    
    