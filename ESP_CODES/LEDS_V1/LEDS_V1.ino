// DEFINIMOS PINES DE SALIDA
#define R 23
#define G 22
#define B 1
#define A 3

char material;
void setup() {

  pinMode(R,OUTPUT);
  pinMode(G,OUTPUT);
  pinMode(B,OUTPUT);
  pinMode(A,OUTPUT);

  Serial.begin(115200);
  // put your setup code here, to run once:

}

void loop() {
  if(Serial.available()){
    material = Serial.read();
    switch(material){
      case 'P':
        digitalWrite(R,LOW);
        digitalWrite(G,HIGH);
        digitalWrite(B,LOW);
        delay(500);
      break;
      case 'A':
        digitalWrite(R,LOW);
        digitalWrite(G,HIGH);
        digitalWrite(B,LOW);
        delay(500);
      break;
      case 'H':
        digitalWrite(R,LOW);
        digitalWrite(G,HIGH);
        digitalWrite(B,LOW);
        delay(500);
      break;
      default:
        digitalWrite(R,HIGH);
        digitalWrite(G,HIGH);
        digitalWrite(B,HIGH);
      break;      
    }
  }else{
    digitalWrite(R,HIGH);
    digitalWrite(G,HIGH);
    digitalWrite(B,HIGH);
    digitalWrite(A,HIGH);
  }
  // put your main code here, to run repeatedly:

}
