#define relay1 22
#define relay2 23
#define relay3 1
//#define relay4 3
#define rasp_input 34
char status_bandas;
void setup() {
  pinMode(rasp_input, INPUT);
  Serial.begin(115200);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
//  pinMode(relay4, OUTPUT);

}

void loop() {
  
  
  switch(digitalRead(rasp_input)){
    case HIGH:
    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, HIGH);
    digitalWrite(relay3, HIGH);
    break;
    
    case LOW:
    digitalWrite(relay1, LOW);
    digitalWrite(relay2, LOW);
    digitalWrite(relay3, LOW);
    break;
  }







  //  status_inicio = digitalRead(inicio);
  //  while (status_inicio != 1) {
  //    status_final = digitalRead(finalBoton);
  //    digitalWrite(relay1, HIGH);
  //    digitalWrite(relay2, HIGH);
  //    digitalWrite(relay3, HIGH);
  ////    digitalWrite(relay4, HIGH);
  //    if (status_final != 1) {
  //      status_inicio = 0;
  //      break;
  //    }
  //  }
  //  if( digitalRead(inicio) != 1){
  //    Serial.println("Entra en el if");
  //    while(digitalRead(finalBoton) != 0){
  //      Serial.println("Entra al while");
  //      digitalWrite(relay1, HIGH);
  //      digitalWrite(relay2, HIGH);
  //      digitalWrite(relay3, HIGH);
  //      digitalWrite(relay4, HIGH);
  //    }
  //    Serial.println("Esta fuera del while");
  //      digitalWrite(relay1, LOW);
  //      digitalWrite(relay2, LOW);
  //      digitalWrite(relay3, LOW);
  //      digitalWrite(relay4, LOW);
  //  }

  //  if (status) {
  //    Serial.println("entra");
  //    digitalWrite(relay1, HIGH);
  //    digitalWrite(relay2, HIGH);
  //    digitalWrite(relay3, HIGH);
  //    digitalWrite(relay4, HIGH);
  //  } else {
  //    Serial.println("no entra");
  //    digitalWrite(relay1, LOW);
  //    digitalWrite(relay2, LOW);
  //    digitalWrite(relay3, LOW);
  //    digitalWrite(relay4, LOW);
  //  }

  //  Serial.println("Fuera del while");
  //  if(digitalRead(inicio) == 1 || digitalRead(finalBoton) == 1){
  //      digitalWrite(relay1, LOW);
  //      digitalWrite(relay2, LOW);
  //      digitalWrite(relay3, LOW);
  //      digitalWrite(relay4, LOW);
  //  }else{
  //    while(digitalRead(finalBoton) != 0){
  //      digitalWrite(relay1, HIGH);
  //      digitalWrite(relay2, HIGH);
  //      digitalWrite(relay3, HIGH);
  //      digitalWrite(relay4, HIGH);
  //    }
  //  }
}
