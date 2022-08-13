#include "HX711.h"

#define DEBUG_HX711

// Parámetro para calibrar el peso y el sensor
#define CALIBRACION -46128.59782608696

// Pin de datos y de reloj
const int LOADCELL_DOUT_PIN = 21;
const int LOADCELL_SCK_PIN = 18;
float p = 0.0;



// Objeto HX711
HX711 bascula;

float calPlasticoBase(){
  float plasticoBase = 0.0;
  #ifdef DEBUG_HX711
    plasticoBase = (bascula.get_units();
  #endif
  return plasticoBase;
}
float calAluminioBase(){
  float aluminioBase = 0.0;
  #ifdef DEBUG_HX711
    aluminioBase = (bascula.get_units();
  #endif
  return aluminioBase;
}
float calHojalataBase(){
  float hojalataBase = 0.0;
  #ifdef DEBUG_HX711
    hojalataBase = bascula.get_units();
  #endif
  return hojalataBase;
}

void sendWeight(float peso){
  Serial.print("=");
  Serial.println(String(peso));
}

void getWeight(char c){
  switch(c){
          case 'P':
            float base = calPlasticoBase();
            delay(2000);
            p = bascula.get_units();
            sendWeight(p - base);
          break;
            calHojalataBase();
            delay(2000);
          break;
          case 'A':
            calAluminioBase();
            delay(2000);
          break;
          case 'F':
            
          break;
  }
}

void setup() {

#ifdef DEBUG_HX711
  // Iniciar comunicación serie
  Serial.begin(115200);
#endif

  // Iniciar sensor
  bascula.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  // Aplicar la calibración
  bascula.set_scale(CALIBRACION);
  // Iniciar la tara
  // No tiene que haber nada sobre el peso
  bascula.tare();
}

void loop() {
  if(Serial.available()){material = Serial.read();}
  
}
