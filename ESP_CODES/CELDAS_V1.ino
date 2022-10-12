//32 Inicio, 33 Alto

#include "HX711.h"
#define calibration_factor 43000.00 //This value is obtained using the SparkFun_HX711_Calibration sketch
#define DOUT4  5 //BASCULA 4
#define CLK4  18

#define DOUT3  19 //BASCULA 3
#define CLK3 21

#define DOUT2  22 //BASCULA 2
#define CLK2  23

#define DOUT 32 //BASCULA 1 
#define CLK 26

//#define START  32
//#define STOP  33
#define SERVO 27 // Verde
#define SERVO2 4 // Naranja con blanco
#define SERVO3 15 // Naranja

//VARIABLES PARA CADA BASCULA
float peso = 0.0;
float peso2 = 0.0;
  float peso3 = 0.0;
float peso4 = 0.0;

float newPeso = 0.0;
float newPeso2 = 0.0;
float newPeso3 = 0.0;
float newPeso4 = 0.0;

float PesoPet = 0.5;
float PesoAlu = 0.5;
float PesoHoj = 0.5;

float PesoTotal = 0.0; 
float PesoTotal2 = 0.0; 
float PesoTotal3 = 0.0; 



//DECLARA 1 OBJETO POR BASCULA
HX711 scale, scale2, scale3, scale4;
//HX711 scale2;

   
void setup() {
  Serial.begin(115200);
  Serial.println("HX711 scale demo");
    
  scale.begin(DOUT, CLK);
  scale.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
  scale.tare(); //Assuming there is no weight on the scale at start up, reset the scale to 0

  scale2.begin(DOUT2, CLK2);
  scale2.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
  scale2.tare(); //Assuming there is no weight on the scale at start up, reset the scale to 0

  scale3.begin(DOUT3, CLK3);
  scale3.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
  scale3.tare(); //Assuming there is no weight on the scale at start up, reset the scale to 0

  scale4.begin(DOUT4,CLK4);
  scale4.set_scale(calibration_factor);
  scale4.tare();
    //pinMode(START, INPUT);
    //pinMode(STOP, INPUT);
    pinMode(SERVO, OUTPUT);
    pinMode(SERVO2, OUTPUT);
    pinMode(SERVO3, OUTPUT);
    //pinMode(SERVO4, OUTPUT);
}
    
void loop() { 
  

//if(digitalRead(START) == 1){
  //do{
    peso = scale.get_units();
    peso2 = scale2.get_units();
    peso3 = scale3.get_units();  
    peso4 = scale4.get_units();

    delay(1000);

    
    newPeso = scale.get_units();
    newPeso2 = scale2.get_units();
    newPeso3 = scale3.get_units();
    newPeso4 = scale4.get_units();
    
    if(peso+0.01  <  newPeso){
      //newPeso = 0;
      digitalWrite(SERVO, HIGH);
      delay(1000);
      digitalWrite(SERVO, LOW);
      
      newPeso = newPeso - peso; 
      
      if (newPeso >= PesoPet){
        PesoTotal = PesoTotal;
      }else{
        PesoTotal = PesoTotal + newPeso;        
      }
   }
    
     if(peso2+0.01  <  newPeso2){
      newPeso2 = 0;
      digitalWrite(SERVO2, HIGH);
      delay(1000);
      digitalWrite(SERVO2, LOW);

       newPeso2 = newPeso2 - peso2; 
      if (newPeso2 >= PesoAlu){
        PesoTotal2 = PesoTotal2;
      }else{
        PesoTotal2 = PesoTotal2 + newPeso2;
      }
    }
       
     if(peso3+0.01  <  newPeso3){
      newPeso3 = 0;
      digitalWrite(SERVO3, HIGH);
      delay(1000);
      digitalWrite(SERVO3, LOW);

       newPeso3 = newPeso3 - peso3; 
      if (newPeso3 >= PesoHoj){
        PesoTotal3 = PesoTotal3;
      }else{
        PesoTotal3 = PesoTotal3 + newPeso3;
      }
    }

    if(peso4+0.01  <  newPeso4){
      newPeso4 = 0;
      //digitalWrite(SERVO4, HIGH);
      delay(1000);
      //digitalWrite(SERVO4   , LOW);
      }

    Serial.print('=');
    Serial.println(String(PesoTotal)+' '+String(PesoTotal2)+' 'String(PesoTotal3));
      
      
  //}while(digitalRead(STOP) != 1);
  /*Serial.print("Reading: ");
  Serial.print(scale.get_units(), 2.3); //scale.get_units() returns a float
  Serial.print(" Kg"); //You can change this to kg but you'll need to refactor the calibration_factor
  Serial.println();*/
  //}
}
