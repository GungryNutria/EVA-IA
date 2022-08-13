#include "HX711.h"
const int LOADCELL_DOUT_PIN = 21;   //-4243831
const int LOADCELL_SCK_PIN = 18;

HX711 bascula;
 
// Parámetro para calibrar el peso y el sensor
float factor_calibracion = -46128.59782608696; //Este valor del factor de calibración funciona para mi. El tuyo probablemente será diferente.
 
void setup() {
  Serial.begin(115200);
  
  // Iniciar sensor
  bascula.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
 
  // Aplicar la calibración
  bascula.set_scale();
  // Iniciar la tara
  // No tiene que haber nada sobre el peso
  bascula.tare();
}
 
void loop() {
 
  // Aplicar calibración
  bascula.set_scale(factor_calibracion);
 
  // Mostrar la información para ajustar el factor de calibración
  Serial.print("Leyendo: ");
  Serial.print(bascula.get_units(),3);
  Serial.print(" kgs");
  Serial.print(" factor_calibracion: ");
  Serial.print(factor_calibracion);
  Serial.println();
 
  // Obtener información desde el monitor serie
  if (Serial.available())
  {
    char temp = Serial.read();
    if (temp == '+')
      factor_calibracion += 10;
    else if (temp == '-')
      factor_calibracion -= 10;
  }
}
