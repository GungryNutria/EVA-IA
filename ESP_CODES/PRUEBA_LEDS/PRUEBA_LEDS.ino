int VERDE = 13;
int AZUL = 11;
int ROJO = 12;
char material;
void showLed(char c){
  switch(c){
          case 'P':
            digitalWrite(ROJO, LOW);
            digitalWrite(AZUL,  LOW);
            digitalWrite(VERDE, HIGH);
            delay(500);
          break;
          case 'H':
            digitalWrite(ROJO, HIGH);
            digitalWrite(AZUL, LOW);
            digitalWrite(VERDE, LOW);
            delay(500);
           
          case 'A':
            digitalWrite(ROJO, LOW);
            digitalWrite(AZUL, HIGH);
            digitalWrite(VERDE, LOW);
            
            delay(500);
          break;
          case 'F':
            digitalWrite(VERDE, HIGH);
            digitalWrite(ROJO, HIGH);
            digitalWrite(AZUL, HIGH);
          break;
          case 'E':
            digitalWrite(VERDE, HIGH);
            digitalWrite(ROJO, HIGH);
            delay(500);
            digitalWrite(VERDE, LOW);
            digitalWrite(ROJO, LOW);
            delay(500);
          break; 
  }
}

void setup() {
  Serial.begin(9600);
  //pines led
  pinMode(AZUL, OUTPUT);
  pinMode(ROJO, OUTPUT);
  pinMode(VERDE, OUTPUT);

}

void loop() {
  if(Serial.available()){
      material = Serial.read();
    }else{
      digitalWrite(ROJO, HIGH);
      digitalWrite(AZUL, HIGH);
      digitalWrite(VERDE, HIGH);
    }
  showLed(material);
  

}
