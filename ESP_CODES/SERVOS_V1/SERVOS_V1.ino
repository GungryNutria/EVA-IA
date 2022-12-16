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
#define sensorA 34
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


    if (material == 'P') {
      
        delay(2000);
        servoA.write(60);
        delay(2000);
        servoA.write(1);
        material = 'N';
      
    }else{
        servoA.write(1);
      }
  ///////////////////////////////  Aqui estara el otro servo y el otro sensor
    if (material == 'A') {
      
        delay(4000);
        servoB.write(35);
        delay(2000);
        servoB.write(80);
        material = 'N';
    }else{
      servoB.write(80);
    }

  
  //////////////////////////////// Aqui estara el otro servo y el otro sensor
    if (material == 'H') {
      
        delay(6000);
        servoC.write(50);
        delay(1000);
        servoC.write(1);        
        material = 'N';
    }else{
      servoC.write(1);
    } 
}