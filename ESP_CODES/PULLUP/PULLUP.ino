//PULLUP

#define ON 13
#define OFF 33
#define LED 2

void setup() {
  // put your setup code here, to run once:
  pinMode(ON,INPUT_PULLUP);
  pinMode(OFF, INPUT_PULLUP);
  pinMode(LED,OUTPUT);
  Serial.begin(115200);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  
  do{
      digitalWrite(LED,HIGH);
      Serial.print("ON");
      
  }while(digitalRead(OFF) != 0);
  
  do{

    digitalWrite(LED,LOW);
    Serial.print("OFF");

    
  }while(digitalRead(ON) != 0);
  
  



  
  /*
  if(digitalRead(ON) == 0){
    
    digitalWrite(LED,HIGH);
    
  }
  if(digitalRead(OFF) == 0){
      
      digitalWrite(LED,LOW);
     
      }
      */
}
