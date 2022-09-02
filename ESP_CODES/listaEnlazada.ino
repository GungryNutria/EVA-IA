//PROGRAMA SERVOS*********************
char materiales[50];
int bandera=0;

void addMaterial(char material){
  for(int i = 0; i < materiales.lenght(); i++){
    if(materiales[i] == ''){
      materiales[i] = material;
      break;
    }
  }
  //revisar despues
  // if(materiales[materiales.lenght()-1] != ''){
    // print("cola llena");
  // }
}

boolean isMaterial(char material){
  bool result;
  for(int i = 0; i < materiales.lenght(); i++){
    if(materiales[i] == material){
      result = true;
      bandera = i;
      break;
    }else{
      result = false;
    }
  }
  return result;
}

void fitList(){
  for(int i = bandera; i < materiales.lenght(); i++){
    if(materiales[i]== ''){
      break;
    }else{
       materiales[i] = materiales[i+1];
    }
  }
}

void setup() {
  // put your setup code here, to run once:
}

void loop() {
  if(digitalRead(startSignal) == 1){

    do{
      // put your main code here, to run repeatedly:
      if(Serial.available()){
        material = Serial.read();
        addMaterial(material);
        }
      if(sensoraluminio == true){
        if(isMaterial('A')){
          fitList();
          //patada
        }
      }
      if(sensorplastico == true){
        if(isMaterial('P')){
          fitList();
        }
      }
      if(sensorhojalata == true){
        if(isMaterial('H')){
          fitList();
        }
      }
    }while(digitalRead(stopBtn)!= 1);
  }
}
