//PROGRAMACIÓN DE ULTRASONICOS (ESP 3)-------
//DECLARACION PINES HC-SR0---------------------
#define trigerA 4
#define echoA 13

#define trigerB 14
#define echoB 16

#define trigerC 17
#define echoC 18

#define trigerD 19
#define echoD 21

//DECLARACION DE VARIABLES---------------------
long duracionA;
long distanciaA;
int porcentajeA;
long duracionB;
long distanciaB;
int porcentajeB;
long duracionC;
long distanciaC;
int porcentajeC;
long duracionD;
long distanciaD;
int porcentajeD;

//FUNCIONES-------------------------------------
void inicializar(){
  Serial.begin(115200);
  pinMode(trigerA,OUTPUT);
  pinMode(echoA,INPUT);
  pinMode(trigerB,OUTPUT);
  pinMode(echoB,INPUT);
  pinMode(trigerC,OUTPUT);
  pinMode(echoC,INPUT);
  pinMode(trigerD,OUTPUT);
  pinMode(echoD,INPUT);
  }

//LECTURA
void HC_A(){
  digitalWrite(trigerA,LOW);
  delayMicroseconds(4);
  digitalWrite(trigerA,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigerA,LOW);

  duracionA = pulseIn(echoA,HIGH);
  duracionA = duracionA/2;  
  distanciaA = duracionA/29; //1 CM DE RECORRIDO A LA VELOCIDAD DEL SONIDO TOMA 29.2 MICROSEGUNDOS

  //PorcentajeA = (m)*(distanciaA)+100; //DEFINIR LA PENDIENTE CUANDO LOS CONTENEDORES ESTEN LISTOS

  // delay(1000);  //REVISAR EL TIEMPO DE ESPERA? COLOCAR EN VOID LOOP
  
  }

void HC_B(){
  digitalWrite(trigerB,LOW);
  delayMicroseconds(4);
  digitalWrite(trigerB,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigerB,LOW);

  duracionB = pulseIn(echoB,HIGH);
  duracionB = duracionB/2;  
  distanciaB = duracionB/29; //1 CM DE RECORRIDO A LA VELOCIDAD DEL SONIDO TOMA 29.2 MICROSEGUNDOS

  //PorcentajeB = (m)*(distanciaB)+100; //DEFINIR LA PENDIENTE CUANDO LOS CONTENEDORES ESTEN LISTOS
  // delay(1000);  //REVISAR EL TIEMPO DE ESPERA? COLOCAR EN VOID LOOP
  
  }

void HC_C(){
  digitalWrite(trigerC,LOW);
  delayMicroseconds(4);
  digitalWrite(trigerC,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigerC,LOW);

  duracionC = pulseIn(echoC,HIGH);
  duracionC = duracionC/2;  
  distanciaC = duracionC/29; //1 CM DE RECORRIDO A LA VELOCIDAD DEL SONIDO TOMA 29.2 MICROSEGUNDOS

  //PorcentajeC = (m)*(distanciaC)+100; //DEFINIR LA PENDIENTE CUANDO LOS CONTENEDORES ESTEN LISTOS

  // delay(1000);  //REVISAR EL TIEMPO DE ESPERA? COLOCAR EN VOID LOOP
  
  }

void HC_D(){
  digitalWrite(trigerD,LOW);
  delayMicroseconds(4);
  digitalWrite(trigerD,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigerD,LOW);

  duracionD = pulseIn(echoD,HIGH);
  duracionD = duracionD/2;  
  distanciaD = duracionD/29; //1 CM DE RECORRIDO A LA VELOCIDAD DEL SONIDO TOMA 29.2 MICROSEGUNDOS

  //PorcentajeD = (m)*(distanciaD)+100; //DEFINIR LA PENDIENTE CUANDO LOS CONTENEDORES ESTEN LISTOS
  
  // delay(1000);  //REVISAR EL TIEMPO DE ESPERA? COLOCAR EN VOID LOOP
  
  }



void mandarDatos(){
  // FUNCION PARA MANDAR DATOS, RECOLECTAR VALORES
  
  
  
  }


void setup() {
  inicializar();
}

void loop() {
  
  //OBTENCIÓN DE LECTURA
  HC_A();
  HC_B();
  HC_C();
  HC_D();
  mandarDatos();
  
  



  
}
