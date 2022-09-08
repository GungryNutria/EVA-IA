#include <ESP32Servo.h>

#define R 22
#define G 23
#define B 21
//#define relay4 3

#define sense1 25
#define sense2 26
#define sense3 27

#define stopSignal 32
#define startSignal 33

Servo servo1;
Servo servo2;
Servo servo3;

char material;
char materiales[200];
int bandera = 0;

void initMateriales() {
  for (int i = 0; i < 200; i++) {

    materiales[i] = 'N';

  }
}
void addMaterial(char material) {
  for (int i = 0; i < 200; i++) {
    if (materiales[i] == 'N') {
      materiales[i] = material;
      break;
    }
  }
  //revisar despues
  // if(materiales[materiales.lenght()-1] != ''){
  // print("cola llena");
  // }
}

boolean isMaterial(char material) {
  bool result = false;
  for (int i = 0; i < 200; i++) {
    if (materiales[i] == material) {
      result = true;
      bandera = i;
      break;
    } else {
      result = false;
    }
  }
  fitList();
  return result;
}

void fitList() {
  for (int i = bandera; i < 200; i++) {
    if (materiales[i] == '0') {
      break;
    } else {
      materiales[i] = materiales[i + 1];
    }
  }
  bandera = 0;
}

void especialFit(char F){
  for(int i = 0; i < 200; i++){
    if(materiales[i] == F){
      materiales[i] = 'D';
    }
  }
}

void moveServo(char material) {
  switch (material) {
    case 'A':
      // Desplazamos a la posición 0º
      digitalWrite(R, LOW);
      digitalWrite(G, HIGH);
      digitalWrite(B, LOW);
      servo1.write(220);
      delay(500);
      servo1.write(130);
      delay(500);
      digitalWrite(R, HIGH);
      digitalWrite(G, HIGH);
      digitalWrite(B, HIGH);
      break;
    case 'P':
      digitalWrite(R, LOW);
      digitalWrite(G, HIGH);
      digitalWrite(B, LOW);
      servo2.write(158);
      delay(500);
      servo2.write(90);
      delay(500);

      digitalWrite(R, HIGH);
      digitalWrite(G, HIGH);
      digitalWrite(B, HIGH);
      break;
    case 'H':
      digitalWrite(R, LOW);
      digitalWrite(G, HIGH);
      digitalWrite(B, LOW);
      servo3.write(180);
      delay(500);
      servo3.write(95);
      delay(500);
      digitalWrite(R, HIGH);
      digitalWrite(G, HIGH);
      digitalWrite(B, HIGH);
      break;

  }
}
void setup() {
  initMateriales();

  Serial.begin(115200);

  //PinMode(relay4, OUTPUT);

  pinMode(startSignal, INPUT);
  pinMode(stopSignal, INPUT);

  pinMode(sense1, INPUT_PULLUP);
  pinMode(sense2, INPUT_PULLUP);
  pinMode(sense3, INPUT_PULLUP);

  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);

  servo2.attach(17);
  servo3.attach(18);
  servo1.attach(16);

  // put your setup code here, to run once:

}

void loop() {
  initMateriales();
  if (digitalRead(startSignal) == 1) {
    do {

      if (Serial.available()) {
        material = Serial.read();
         switch(material){
          case 'a':
            // remplazo aluminio por desconocido
            especialFit(material);
          break;
          case 'p':
            // remplazo plastico por desconocido
            especialFit(material);
          break;
          case 'h':
            // remplazo hojalata por desconocido
            especialFit(material);
          break;
          case 'A':
            addMaterial(material);
          break;
          case 'P':
            addMaterial(material);
          break;
          case 'H':
            addMaterial(material);
          break;
          default:
            addMaterial('D');
          break;
        }
      } else {
        digitalWrite(R, HIGH);
        digitalWrite(G, HIGH);
        digitalWrite(B, HIGH);
      }

      if (digitalRead(sense1) == LOW) {
        if (isMaterial('P')) {
          moveServo('P');
        }
      } else if (digitalRead(sense2) == LOW) {
        if (isMaterial('A')) {
          moveServo('A');
        }
      } else if (digitalRead(sense3) == LOW) {
        if (isMaterial('H')) {
          moveServo('H');
        }
      }

    } while (digitalRead(stopSignal) != 1);
    ESP.restart();
  }

  // put your main code here, to run repeatedly:

}
