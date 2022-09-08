//PROGRAMA ESP32 (CELDAS DE CARGA)------
//PIN D34 ENTRADA GENERAL

//LIBRERIAS-----------------------------
#include "soc/rtc.h"
#include "HX711.h"
HX711 balanzaA;
HX711 balanzaP;
HX711 balanzaH;
HX711 balanzaO;

//DECLARACION DE PINES----------------
#define stopSignal 32
#define startSignal 33
#define CLK 18
#define DTA 19 //REVISAR COMPATIBILIDAD DE PINES
#define DTP 21
#define DTH 22
#define DTO 23

//DECLARACION DE VARIABLES
float peso0A; //PESO INICIAL
float peso0P;
float peso0H;
float peso0O;

float pesoFA; //PESO FINAL
float pesoFP;
float pesoFH;
float pesoFO;

float pesoA; //DIFERENCIA DE PESO
float pesoP;
float pesoH;
float pesoO;


//FUNCIONES----------------------------- 

//INICIO BALANZAS

void inicioBalanzaA(){
  
    balanzaA.begin(DTA, CLK);
    balanzaA.set_scale(-230.6); // Establecemos la escala
    balanzaA.tare(20);  //El peso actual es considerado Tara.
    
  }

void inicioBalanzaP(){
  
    balanzaP.begin(DTP, CLK);
    balanzaP.set_scale(-230.6); // Establecemos la escala
    balanzaP.tare(20);  //El peso actual es considerado Tara.
    
  }

void inicioBalanzaH(){
  
    balanzaH.begin(DTH, CLK);
    balanzaH.set_scale(-230.6); // Establecemos la escala
    balanzaH.tare(20);  //El peso actual es considerado Tara.
    
  }

void inicioBalanzaO(){
  
    balanzaO.begin(DTO, CLK);
    balanzaO.set_scale(-230.6); // Establecemos la escala
    balanzaO.tare(20);  //El peso actual es considerado Tara.
    
  }  
//CALCULAR PESO INICIAL

float pesoInicialA(){
  
  peso0A = balanzaA.get_units(20);
  return peso0A;
  
  }
  
float pesoInicialP(){
  
  peso0P = balanzaP.get_units(20);
  return peso0P;
  
  }

float pesoInicialH(){
  
  peso0H = balanzaH.get_units(20);
  return peso0H;
  
  }

float pesoInicialO(){
  
  peso0O = balanzaO.get_units(20);
  return peso0O;
  
  }

//CALCULAR PESO FINAL

float pesoFinalA(){
  
  pesoFA = balanzaA.get_units(20);
  return pesoFA;
  
  }

float pesoFinalP(){
  
  pesoFP = balanzaP.get_units(20);
  return pesoFP;
  
  }

float pesoFinalH(){
  
  pesoFH = balanzaH.get_units(20);
  return pesoFH;
  
  }

float pesoFinalO(){
  
  pesoFO = balanzaO.get_units(20);
  return pesoFO;
  
  }

  
void setup() {
  //INICIALIZAR
  Serial.begin(115200);
  //rtc_clk_cpu_freq_set(RTC_CPU_FREQ_80M);
  pinMode(startSignal,INPUT); //PIN INICIO
  pinMode(stopSignal,INPUT);
  inicioBalanzaA();
  inicioBalanzaP();
  inicioBalanzaH();
  inicioBalanzaO();
  
  }

void loop() {
  //SE LEERÁ EL ESTADO DEL BTN_INICIO, MIENTRAS SEA 1, ESTE EJECUTARA EL CICLO. SI CAMBIA A 0, SE IMPRIMIRAN LOS RESULTADOS
  if(digitalRead(startSignal) == 1){
    //LEER PESO INICIAL
    peso0A = pesoInicialA();
    peso0P = pesoInicialP();
    peso0H = pesoInicialH();
    peso0O = pesoInicialO();
  }

  if(digitalRead(stopSignal) == 1){
  //PESO FINAL
  pesoFA = pesoFinalA();
  pesoFP = pesoFinalP();
  pesoFH = pesoFinalH();
  pesoFO = pesoFinalO();

  //PESO TOTAL - DIFERENCIA
  pesoA = pesoFA - peso0A;
  pesoP = pesoFP - peso0P;
  pesoH = pesoFH - peso0H;
  pesoO = pesoFO - peso0O;

  }
 }
