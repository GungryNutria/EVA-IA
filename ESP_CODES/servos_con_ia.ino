#include <ESP32Servo.h>
#include <analogWrite.h>
#include <ESP32Tone.h>
#include <ESP32PWM.h>

///// Servo A
#define sensorA 34
#define btn1 35
#define raspA 6
Servo servoA;

//// Servo B
#define sensorB 25
#define btn2 26
#define raspB 7
Servo servoB;

/// Servo C
#define sensorC 32
#define btn3 27
#define raspC 8
Servo servoC;

char servo = '';

void setup() {
  // A
  pinMode(sensorA, INPUT);
  pinMode(btn1, INPUT);
  pinMode(raspA, OUTPUT);
  servoA.attach(13);
//// B
  pinMode(sensorB, INPUT);
  pinMode(btn2, INPUT);
  pinMode(raspB, OUTPUT);
  servoB.attach(12);
///// C
  pinMode(sensorC, INPUT);
  pinMode(btn3, INPUT);
  pinMode(raspC, OUTPUT);
  servoC.attach(14);
  
//// Inicializamos serial
  Serial.begin(115200);
}

void loop() {
  
  // Condicion si el fotoelectrico detecta algo enfrente de el
  if(digitalRead(sensorA) == HIGH){
    // Mando una señal alta a la rasp para que busque un proceso para el servo A
    digitalWrite(raspA,HIGH);
    delay(500);
    if(Serial.available()){servo = Serial.read()}
    if (servo == 'A'){
      while(digitalRead(btn1) != 1){
      //Mantendra el servo A a 75 grados mientras el reciclador no tenga cambios
      servoA.write(75);
    }
    }
  }else{servoA.write(1);/* Devolvemos el servo a su estado de origen */}


  // Condicion si el fotoelectrico detecta algo enfrente de el
  if(digitalRead(sensorB) == HIGH){
    // Mando una señal alta a la rasp para que busque un proceso para el servo B
    digitalWrite(raspB,HIGH);
    delay(500);
    if(Serial.available()){servo = Serial.read()}
    if (servo == 'P'){
      while(digitalRead(btn2) != 1){
      //Mantendra el servo A a 75 grados mientras el reciclador no tenga cambios
      servoB.write(75);
    }
    }
  }else{servoB.write(1);/* Devolvemos el servo a su estado de origen */}


  // Condicion si el fotoelectrico detecta algo enfrente de el
  if(digitalRead(sensorC) == HIGH){
    // Mando una señal alta a la rasp para que busque un proceso para el servo C
    digitalWrite(raspC,HIGH);
    delay(500);
    if(Serial.available()){servo = Serial.read()}
    if (servo == 'H'){
      while(digitalRead(btn3) != 1){
      //Mantendra el servo A a 75 grados mientras el reciclador no tenga cambios
      servoC.write(75);
    }
    }
  }else{servoC.write(1);/* Devolvemos el servo a su estado de origen */}

}