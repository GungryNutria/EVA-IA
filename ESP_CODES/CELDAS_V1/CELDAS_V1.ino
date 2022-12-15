//32 Inicio, 33 Alto

#include "HX711.h"

// ESTE VALOR ES OBTENIDO USANDO EL SparkFun_HX711_Calibration sketch
#define calibration_factor 43000.00 

// PINES DE BASCULA 1
#define DOUT 32 
#define CLK 26

// PINES DE BASCULA 2
#define DOUT2  22
#define CLK2  23

// PINES DE BASCULA 3
#define DOUT3  19
#define CLK3 21

// PINES DE BASCULA 4
#define DOUT4  5
#define CLK4  18

// PINES PARA REGRESAR PINES
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
float PesoOrg = 0.5;

float PesoTotal = 0.0; 
float PesoTotal2 = 0.0; 
float PesoTotal3 = 0.0; 
float PesoTotal4 = 0.0; 


//DECLARA 1 OBJETO POR BASCULA
HX711 scale, scale2, scale3, scale4;
   
void setup() {
  // INICIALIZAMOS SERIALES
  Serial.begin(115200);

  // CONFIGURACION DE LA PRIMERA BASCULA
  scale.begin(DOUT, CLK);
  scale.set_scale(calibration_factor); 
  scale.tare(); 
  // CONFIGURACION DE LA SEGUNDA BASCULA 
  scale2.begin(DOUT2, CLK2);
  scale2.set_scale(calibration_factor); 
  scale2.tare();
  // CONFIGURACION DE LA TERCERA BASCULA
  scale3.begin(DOUT3, CLK3);
  scale3.set_scale(calibration_factor);
  scale3.tare(); 
  // CONFIGURACION DE PINES DE SALIDA
  pinMode(SERVO, OUTPUT);
  pinMode(SERVO2, OUTPUT);
  pinMode(SERVO3, OUTPUT);
}
    
void loop() { 
  

    // CONSULTAR PESO INICIAL
    peso = scale.get_units();
    peso2 = scale2.get_units();
    peso3 = scale3.get_units();  
    Serial.println(peso);
    Serial.println(peso2);
    Serial.println(peso3);
    delay(1000);

    // CONSULTAR PESO DESPUES DE 1 SEGUNDO
    newPeso = scale.get_units();
    newPeso2 = scale2.get_units();
    newPeso3 = scale3.get_units();
    Serial.println(newPeso);
    Serial.println(newPeso2);
    Serial.println(newPeso3);
    
    // SI EL PESO INICIAL  DE PLASTICO ES MENOR AL NUEVO
    if(peso+0.005 < newPeso){
      // MANDAR PULSO ALTO A EL SERVO DE PLASTICO
      digitalWrite(SERVO, HIGH);
      delay(1000);
      digitalWrite(SERVO, LOW);
    }
    
    // SI EL PESO INICIAL DE ALUMINIO ES MENOR AL NUEVO
    if(peso2+0.005 < newPeso2){
      // MANDAR PULSO ALTO A EL SERVO DE ALUMINIO
      digitalWrite(SERVO2, HIGH);
      delay(1000);
      digitalWrite(SERVO2, LOW);
    }
    
    // SI EL PESO INICIAL DE HOJALATA ES MENOR AL NUEVO
    if(peso3+0.005 < newPeso3){
      // MANDAR PULSO ALTO A EL SERVO DE HOJALATA
      digitalWrite(SERVO3, HIGH);
      delay(1000);
      digitalWrite(SERVO3, LOW);
    }


}
