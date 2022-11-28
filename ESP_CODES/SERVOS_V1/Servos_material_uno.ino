#include <ESP32Servo.h>
#include <analogWrite.h>
#include <ESP32Tone.h>
#include <ESP32PWM.h>

char material;

/// RGB
//#define R 23
//#define G 22
//#define B 1
//#define A 3
// PINES DE SENSORA 15, BTN1 33
#define sensorA 4
#define btn1 33
Servo servoA; 

//// Servo B
#define sensorB 35
#define btn2 25
Servo servoB;

/// Servo C
#define sensorC 32
#define btn3 26
Servo servoC;


void setup() {
  // A
  pinMode(sensorA, INPUT);
  pinMode(btn1, INPUT);
  servoA.attach(14);
  //// B
  pinMode(sensorB, INPUT);
  pinMode(btn2, INPUT);
  servoB.attach(12);
  ///// C
  pinMode(sensorC, INPUT);
  pinMode(btn3, INPUT);
  servoC.attach(13);

  ///// leds
  //pinMode(R, OUTPUT);
  //pinMode(G, OUTPUT);
  //pinMode(B, OUTPUT);
  //pinMode(A, OUTPUT);

  //// Este es del serial
  Serial.begin(115200);
}

void loop() {
//  if(Serial.available()){
//    material = Serial.read();
//  }

//  digitalWrite(R, HIGH);
//  digitalWrite(G, HIGH);
//  digitalWrite(B, HIGH);
//  digitalWrite(A, HIGH);
  if(Serial.available()){
    material = Serial.read();
  }
  Serial.println(String(material));
  //Serial.println(String(Serial.read()));

  if (digitalRead(sensorA) == HIGH) {
    if (material == 'P') {
      while (digitalRead(btn1) != 1) {
        delay(1700);
        servoA.write(50);
      }
    }else{
        servoA.write(1);
      }
  } else {
    servoA.write(1);
  }
  /////////////////////////////// Aqui estara el otro servo y el otro sensor
  if (digitalRead(sensorB) == HIGH) {
    if (material == 'A') {
      while (digitalRead(btn2) != 1) {
        delay(1700);
        servoB.write(35);
      }
    }else{
      servoB.write(80);
    }
  } else {
    servoB.write(80);
  }
  //////////////////////////////// Aqui estara el otro servo y el otro sensor

  if (digitalRead(sensorC) == HIGH) {
    if (material == 'H') {
      while (digitalRead(btn3) != 1) {
        delay(1700);
        servoC.write(50);
      }
    }else{
      servoC.write(1);
    }
  } else {
    servoC.write(1);
  } 
}