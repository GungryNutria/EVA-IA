#define relay1 22
#define relay2 23
#define relay3 1
//#define relay4 3
#define led 2
#define pir 25
#define rasp_input 34
#define rasp_output 35

int status_bandas = 0;

char status_bandas;
void setup() {
  Serial.begin(115200);
  
  //BANDAS
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  
  //SENSOR
  pinMode(pir, INPUT);
  pinMode(led, OUTPUT);
  
  //COMUNICACION RASP
  pinMode(rasp_input,INPUT);
  pinMode(rasp_output,OUTPUT);


//  pinMode(relay4, OUTPUT);

}

void loop() {
  if (digitalRead(rasp_input) == HIGH) {
    
    status_bandas = 1;

    while(status_bandas == 1){

      if (digitalRead(rasp_input) == HIGH){
        status_bandas = 0;
        break;
      }
    
      if (digitalRead(pir) == HIGH){
        
        digitalWrite(rasp_output,HIGH);
        
        digitalWrite(relay1, LOW);
        digitalWrite(relay2, LOW);
        digitalWrite(relay3, LOW);
        
        delay(1000);

      }else{
        
        digitalWrite(relay1, HIGH);
        digitalWrite(relay2, HIGH);
        digitalWrite(relay3, HIGH);
      
      }
    }
  }else{
    digitalWrite(relay1, LOW);
    digitalWrite(relay2, LOW);
    digitalWrite(relay3, LOW);
  }
}
