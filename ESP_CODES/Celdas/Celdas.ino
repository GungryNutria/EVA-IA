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
const int stopSignal = 32;
const int startSignal = 33;

const int CLK= 18;
const int DTA = 19; //REVISAR COMPATIBILIDAD DE PINES
const int DTP = 21;
const int DTH= 22;
const int DTO = 23;

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

void inicioBalanzaB(){
  
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

void pesoInicialA(){
  
  peso0A = balanzaA.get_units(20);
  return peso0A;
  
  }
  
void pesoInicialP(){
  
  peso0P = balanzaP.get_units(20);
  return peso0P;
  
  }

void pesoInicialH(){
  
  peso0H = balanzaH.get_units(20);
  return peso0H;
  
  }

void pesoInicialO(){
  
  peso0O = balanzaO.get_units(20);
  return peso0O;
  
  }

//CALCULAR PESO FINAL

void pesoFinalA(){
  
  pesoFA = balanzaA.get_units(20);
  return pesoFA;
  
  }

void pesoFinalP(){
  
  pesoFP = balanzaP.get_units(20);
  return pesoFP;
  
  }

void pesoFinalH(){
  
  pesoFC = balanzaH.get_units(20);
  return pesoFH;
  
  }

void pesoFinalO(){
  
  pesoFO = balanzaO.get_units(20);
  return pesoFO;
  
  }

  
void setup() {
  //INICIALIZAR
  Serial.begin(115200);
  rtc_clk_cpu_freq_set(RTC_CPU_FREQ_80M);
  pinMode(startSignal,INPUT); //PIN INICIO
  pinMode(stopSignal);
  inicioBalanzaA();
  inicioBalanzaB();
  inicioBalanzaC();
  inicioBalanzaD();
  
  }

void loop() {
  //SE LEERÁ EL ESTADO DEL BTN_INICIO, MIENTRAS SEA 1, ESTE EJECUTARA EL CICLO. SI CAMBIA A 0, SE IMPRIMIRAN LOS RESULTADOS
  if(digitalRead(startSignal) == 1){
    //LEER PESO INICIAL
    peso0A = pesoInicialA();
    peso0P = pesoInicialP();
    peso0H = pesoInicialH();
    peso0O = pesoInicialO();

    do{
    //NADA DE NADA
    }while(digitalRead(stopSignal)==0);
  //PESO FINAL
  pesoFA = pesoFinalA();
  pesoFP = pesoFinalP();
  pesoFH = pesoFinalH();
  pesoFO = pesoFinalO();

  //PESO TOTAL - DIFERENCIA
  pesoA = pesoFA - peso0A;
  pesoB = pesoFB - peso0B;
  pesoC = pesoFC - peso0C;
  pesoD = pesoFD - peso0D;
    
  }

  


  
  }
