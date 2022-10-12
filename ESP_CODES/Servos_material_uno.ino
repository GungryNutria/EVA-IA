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

  if (digitalRead(sensorA) == HIGH) {
    switch (Serial.read()){
    case 'P':
      while (digitalRead(btn1) != 1) {
        delay(1000);
        servoA.write(50);
      }
    break;
    default:
        servoA.write(1);
    break;
    }
  } else if (digitalRead(sensorB) == HIGH) {
    switch(Serial.read()) {
      case 'A':
      while (digitalRead(btn2) != 1) {
        delay(1000);
        servoB.write(1);
      }
      break;
      default:
        servoB.write(80);
      break;
    }
  } else  if (digitalRead(sensorC) == HIGH) {
      switch(Serial.read()) {
        case 'A':
      while (digitalRead(btn3) != 1) {
        servoC.write(50);
      }
      break;
      default:
      servoC.write(1);
      break;
  } else {
    servoA.write(1);
    servoB.write(80);
    servoC.write(1);
  }
}