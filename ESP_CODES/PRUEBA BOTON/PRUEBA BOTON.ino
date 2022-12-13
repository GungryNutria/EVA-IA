//LIBRERIAS
#include <Nextion.h>

//PINES
#define LED 2

#define startSignal 32
#define stopSignal 25

//+++++++++++++++++++++NEXTION+++++++++++++++++++++++++++++++
//OBJETOS NEXTION
NexButton procesoBtn = NexButton(0,1,"b0"); //COLOCAR IDS +++++
//NexButton saldoBtn = NexButton(0,2,"b1");
//NexText labelTarjeta = NexText(0,3,"t0");
NexPage page0 = NexPage(0,0,"page0"); //PASAR PAGINA DE PROCESO++++++++++
//PROCESOS OBJ
//NexText plasticos = NexText(1,1,"t0");
//NexText aluminio = NexText(1,2,"t1");
//NexText metal = NexText(1,3,"t2");
NexButton finalBtn = NexButton(1,4,"b0");
NexPage page1 = NexPage(1,0,"page1"); //PASAR PAGINA DE SALDO++++++++++
//SALDOS OBJ
//NexPage page2 = NexPage(2,0,"page2");
//NexText labelsaldo = NexText(2,1,"t5");

//REGISTRO DE EVENTOS
NexTouch *nex_listen_list[]{
  &procesoBtn,
  //&saldoBtn,
  &finalBtn,
  NULL
  };

//********************************FUNCIONES*****************************************
void setupNextion(){ //INICIAR SETUP NEXTION
  
  nexInit();
  Serial.begin(115200);   
  procesoBtn.attachPush(proceso, &procesoBtn);
  //saldoBtn.attachPush(saldo, &saldoBtn);
  finalBtn.attachPush(final, &finalBtn);
  page0.show();
  }

void proceso(void*ptr){
  digitalWrite(LED,HIGH);
  page1.show();


}

void final(void*ptr){

  digitalWrite(LED,LOW);
  page0.show();


}

void setup() {
  // put your setup code here, to run once:
  setupNextion();
  pinMode(LED,OUTPUT);


}

void loop() {
  // put your main code here, to run repeatedly:

  nexLoop(nex_listen_list);

}
