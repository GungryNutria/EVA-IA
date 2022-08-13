int servo1 =12;
int servo2 =14;
int servo3 =27;
char servo;
void setup() {
 pinMode (servo1, OUTPUT);
 pinMode (servo2, OUTPUT);
 pinMode (servo3, OUTPUT);
 Serial.begin(115200);
}
void loop() {
  while (true){
     if (Serial.available()){servo= Serial.read();}    
  switch(servo){
    case 'P':
    digitalWrite (servo1,HIGH);
    delayMicroseconds(1700);   
    digitalWrite (servo1,LOW);
    delay(19);
    digitalWrite (servo1,HIGH);
    delayMicroseconds(800);  
    digitalWrite (servo1,LOW);
    delay(20);
    break;
    case 'A':
     digitalWrite (servo2,HIGH);
    delayMicroseconds(1700);   
    digitalWrite (servo2,LOW);
    delay(19);
    digitalWrite (servo2,HIGH);
    delayMicroseconds(800);  
    digitalWrite (servo2,LOW);
    delay(20);
    break;
    case 'H':
     digitalWrite (servo3,HIGH);
    delayMicroseconds(1700);   
    digitalWrite (servo3,LOW);
    delay(19);
    digitalWrite (servo3,HIGH);
    delayMicroseconds(800);  
    digitalWrite (servo3,LOW);
    delay(20);
    break;
    }   
                 
}
    }
 
