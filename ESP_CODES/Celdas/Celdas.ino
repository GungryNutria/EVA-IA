//PROGRAMA ESP32 (CELDAS DE CARGA)------
//PIN D34 ENTRADA GENERAL

//LIBRERIAS-----------------------------
#include "soc/rtc.h"
#include "HX711.h"
HX711 balanzaA;
HX711 balanzaB;
HX711 balanzaC;
HX711 balanzaD;

//DECLARACION DE PINES----------------
const int BTN_INICIO = 34;
const int BTN_FIN = 33;

const int CLK= 18;
const int DTA = 19; //REVISAR COMPATIBILIDAD DE PINES
const int DTB = 21;
const int DTC = 22;
const int DTD = 23;

//DECLARACION DE VARIABLES
float peso0A; //PESO INICIAL
float peso0B;
float peso0C;
float peso0D;

float pesoFA; //PESO FINAL
float pesoFB;
float pesoFC;
float pesoFD;

float pesoA; //DIFERENCIA DE PESO
float pesoB;
float pesoC;
float pesoD;


//FUNCIONES----------------------------- 

//INICIO BALANZAS

void inicioBalanzaA(){
    balanzaA.begin(DTA, CLK);
    Serial.print("Lectura A:  ");
    Serial.println(balanzaA.read());
    Serial.println("No ponga ningun  objeto sobre la balanza");
    Serial.println("Destarando...");
    Serial.println("...");
    balanzaA.set_scale(-230.6); // Establecemos la escala
    balanzaA.tare(20);  //El peso actual es considerado Tara.
    Serial.println("BALANZA A - LISTO");
  }

void inicioBalanzaB(){
    balanzaB.begin(DTB, CLK);
    Serial.print("Lectura B:  ");
    Serial.println(balanzaB.read());
    Serial.println("No ponga ningun  objeto sobre la balanza");
    Serial.println("Destarando...");
    Serial.println("...");
    balanzaB.set_scale(-230.6); // Establecemos la escala
    balanzaB.tare(20);  //El peso actual es considerado Tara.
    Serial.println("BALANZA B - LISTO");
  }

void inicioBalanzaC(){
    balanzaC.begin(DTC, CLK);
    Serial.print("Lectura C:  ");
    Serial.println(balanzaC.read());
    Serial.println("No ponga ningun  objeto sobre la balanza");
    Serial.println("Destarando...");
    Serial.println("...");
    balanzaC.set_scale(-230.6); // Establecemos la escala
    balanzaC.tare(20);  //El peso actual es considerado Tara.
    Serial.println("BALANZA C - LISTO");
  }

void inicioBalanzaD(){
    balanzaD.begin(DTD, CLK);
    Serial.print("Lectura D:  ");
    Serial.println(balanzaD.read());
    Serial.println("No ponga ningun  objeto sobre la balanza");
    Serial.println("Destarando...");
    Serial.println("...");
    balanzaD.set_scale(-230.6); // Establecemos la escala
    balanzaD.tare(20);  //El peso actual es considerado Tara.
    Serial.println("BALANZA D - LISTO");
  }  
//CALCULAR PESO INICIAL

void pesoInicialA(){
  Serial.print("Peso A: ");
  peso0A = balanzaA.get_units(20);
  Serial.println(" gr");
  }
  
void pesoInicialB(){
  Serial.print("Peso B: ");
  peso0B = balanzaB.get_units(20);
  Serial.println(" gr");
  
  }

void pesoInicialC(){
  Serial.print("Peso C: ");
  peso0C = balanzaC.get_units(20);
  Serial.println(" gr");
  
  }

void pesoInicialD(){
  Serial.print("Peso D: ");
  peso0D = balanzaD.get_units(20);
  Serial.println(" gr");
  
  }

//CALCULAR PESO FINAL

void pesoFinalA(){
  Serial.print("Peso AF: ");
  pesoFA = balanzaA.get_units(20);
  Serial.println(" gr");
  }

void pesoFinalB(){
  Serial.print("Peso BF: ");
  pesoFB = balanzaB.get_units(20);
  Serial.println(" gr");
  }

void pesoFinalC(){
  Serial.print("Peso CF: ");
  pesoFC = balanzaC.get_units(20);
  Serial.println(" gr");
  }

void pesoFinalD(){
  Serial.print("Peso DF: ");
  pesoFD = balanzaD.get_units(20);
  Serial.println(" gr");
  }

//FUNCION UART


  
//FUNCION DE PESOS TOTALES
void pesosTotales(){
  pesoA = pesoFA - peso0A;
  pesoB = pesoFB - peso0B;
  pesoC = pesoFC - peso0C;
  pesoD = pesoFD - peso0D;

  //COLOCAR FUNCION QUE ENVIA LOS PESOS
  
  }
  
void setup() {
  //INICIALIZAR
  Serial.begin(115200);
  rtc_clk_cpu_freq_set(RTC_CPU_FREQ_80M);
  pinMode(BTN_INICIO,INPUT); //PIN INICIO
  inicioBalanzaA();
  inicioBalanzaB();
  inicioBalanzaC();
  inicioBalanzaD();
  Serial.print("Basculas listas");
  }

void loop() {
  //SE LEERÁ EL ESTADO DEL BTN_INICIO, MIENTRAS SEA 1, ESTE EJECUTARA EL CICLO. SI CAMBIA A 0, SE IMPRIMIRAN LOS RESULTADOS
  
  if(BTN_INICIO == HIGH){ //SEÑAL DE INICIO
    pesoInicialA();
    pesoInicialB();
    pesoInicialC();
    pesoInicialD();
    }

  if(BTN_FIN == HIGH){ //SEÑAL DE FIN
    pesoFinalA();
    pesoFinalB();
    pesoFinalC();
    pesoFinalD();
    
    pesosTotales();
    
    
    }

  
  }
