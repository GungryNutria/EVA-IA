//******************************ESP-ESCLAVO ULTRASONICOS************************
//DECLARACION PINES HC-SR0---------------------
#define stopSignal 33
#define startSignal 32


#define trigerA 13
#define echoA 14

#define trigerP 18
#define echoP 19

#define trigerH 21
#define echoH 22

#define trigerD 25
#define echoD 26

#define trigerO 32
#define echoO 33             

#define LED 2



//DECLARACION DE VARIABLES---------------------
long duracionA;
long distanciaA;
int porcentajeA;

long duracionP;
long distanciaP;
int porcentajeP;

long duracionH;
long distanciaH;
int porcentajeH;

long duracionD;
long distanciaD;
int porcentajeD;

long duracionO;
long distanciaO;
int porcentajeO;

int A;
int P;
int H;
int D;
int O;

//FUNCIONES-------------------------------------
void inicializar() {


  pinMode(trigerA, OUTPUT);
  pinMode(echoA, INPUT);
  pinMode(trigerP, OUTPUT);
  pinMode(echoP, INPUT);
  pinMode(trigerH, OUTPUT);
  pinMode(echoH, INPUT);
  pinMode(trigerD, OUTPUT);
  pinMode(echoD, INPUT);
  pinMode(trigerO, OUTPUT);
  pinMode(echoO, INPUT);

  pinMode(startSignal, INPUT); //FUNCIONARA EL INPUT?
  pinMode(stopSignal, INPUT);

}

//LECTURA

int HC_A() {
  digitalWrite(trigerA, LOW);
  delayMicroseconds(4);
  digitalWrite(trigerA, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigerA, LOW);

  duracionA = pulseIn(echoA, HIGH);
  duracionA = duracionA / 2;
  distanciaA = duracionA / 29; //1 CM DE RECORRIDO A LA VELOCIDAD DEL SONIDO TOMA 29.2 MICROSEGUNDOS

  porcentajeA = distanciaA / 2;



  return porcentajeA;

}


int HC_P() {
  digitalWrite(trigerP, LOW);
  delayMicroseconds(4);
  digitalWrite(trigerP, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigerP, LOW);

  duracionP = pulseIn(echoP, HIGH);
  duracionP = duracionP / 2;
  distanciaP = duracionP / 29; //1 CM DE RECORRIDO A LA VELOCIDAD DEL SONIDO TOMA 29.2 MICROSEGUNDOS

  porcentajeP = distanciaP / 2;
  return porcentajeP;

}

int HC_H() {
  digitalWrite(trigerH, LOW);
  delayMicroseconds(4);
  digitalWrite(trigerH, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigerH, LOW);

  duracionH = pulseIn(echoH, HIGH);
  duracionH = duracionH / 2;
  distanciaH = duracionH / 29; //1 CM DE RECORRIDO A LA VELOCIDAD DEL SONIDO TOMA 29.2 MICROSEGUNDOS

  porcentajeH = distanciaH / 2;
  return porcentajeH;
}

int HC_D() {
  digitalWrite(trigerD, LOW);
  delayMicroseconds(4);
  digitalWrite(trigerD, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigerD, LOW);

  duracionD = pulseIn(echoD, HIGH);
  duracionD = duracionD / 2;
  distanciaD = duracionD / 29; //1 CM DE RECORRIDO A LA VELOCIDAD DEL SONIDO TOMA 29.2 MICROSEGUNDOS
  porcentajeD = distanciaD / 2;


  return porcentajeD;
}

int HC_O() {
  digitalWrite(trigerO, LOW);
  delayMicroseconds(4);
  digitalWrite(trigerO, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigerO, LOW);

  duracionO = pulseIn(echoO, HIGH);
  duracionO = duracionO / 2;
  distanciaO = duracionO / 29; //1 CM DE RECORRIDO A LA VELOCIDAD DEL SONIDO TOMA 29.2 MICROSEGUNDOS
  porcentajeO = distanciaO / 2;

  return porcentajeO;
}

void setup() {
  inicializar();
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
}

void loop() {

  if (digitalRead(startSignal) ==  1) {

    do {

      A = HC_A();
      P = HC_P();
      H = HC_H();
      D = HC_D();

      
      if (A > 90) {
        Serial.print("=");
        Serial.println("FA");
      } else {
        Serial.print("=A");
        Serial.println(String(A));
      }
      
      if(P > 90){
        Serial.print("=");
        Serial.println("FP");
      }else{
        Serial.print("=P");
        Serial.println(String(P));
      }

      if(H > 90){
        Serial.print("=");
        Serial.println("FH");
      }else{
        Serial.print("=H");
        Serial.println(String(H));
      }

      if(D > 90){
        Serial.print("=");
        Serial.println("FD");
      }else{
        Serial.print("=D");
        Serial.println(String(D));
      }

      if(O > 90){
        Serial.print("=");
        Serial.println("FO");
      }else{
        Serial.print("=O");
        Serial.println(String(O));
      }

      digitalWrite(LED, HIGH);


    } while (digitalRead(stopSignal) == 0); //REVISAR SENAL INPUT_PULLUP
    digitalWrite(LED, LOW);
    ESP.restart();


  }


}
