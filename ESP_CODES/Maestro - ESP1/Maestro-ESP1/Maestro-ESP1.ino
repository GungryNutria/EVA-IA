//***********************************LIBRERIAS**************************
#include <Nextion.h>
#include <SPI.h>
#include <UNIT_PN532.h> //Librkjjuhjería Modificada


//****************************DECLARACION DE PINES*********************************
#define startBtn 13
#define stopBtn 33

#define PN532_SCK  18
#define PN532_MOSI 23
#define PN532_SS   5
#define PN532_MISO 19

#define LED 2

#define startSignal 32
#define stopSignal 25

//***************************DECLARACION DE VARIABLES****************************
uint8_t DatoRecibido[4]; //Para almacenar los datos
uint32_t cardid;
uint32_t versiondata;
char material;
String porcentaje;
UNIT_PN532 nfc(PN532_SS);// Línea enfocada para la comunicación por SPI

//**********************************NEXTION*************************************
//OBJETOS NEXTION
NexButton procesoBtn = NexButton(0,2,"b0"); //COLOCAR IDS +++++
NexButton saldoBtn = NexButton(0,3,"b1");
NexText labelTarjeta = NexText(0,1,"t0");
NexPage page0 = NexPage(0,0,"page0"); //PASAR PAGINA DE PROCESO++++++++++
//PROCESOS OBJ
NexText labelproceso = NexText(1,6,"t5");
NexText plasticos = NexText(1,1,"t0");
NexText aluminio = NexText(1,2,"t1");
NexText metal = NexText(1,3,"t2");
NexText desconocido = NexText(1,4,"t3");
NexText organica = NexText(1,5,"t4");
NexPage page1 = NexPage(1,1,"page1"); //PASAR PAGINA DE SALDO++++++++++
//SALDOS OBJ
NexPage page2 = NexPage(2,0,"page2");
NexText labelsaldo = NexText(2,1,"t5");

//REGISTRO DE EVENTOS
NexTouch *nex_listen_list[]{
  &procesoBtn,
  &saldoBtn,
  NULL
  };

//********************************FUNCIONES*****************************************
void setupNextion(){ //INICIAR SETUP NEXTION
  
  nexInit();
  Serial.begin(115200); 
  procesoBtn.attachPush(proceso, &procesoBtn);
  saldoBtn.attachPush(saldo, &saldoBtn);
  page0.show();  
  
  }

void setupTarjeta(){ //REVISAR+++++++++++++++
  nfc.begin(); //Comienza la comunicación del PN532
  versiondata = nfc.getFirmwareVersion();//Obtiene información de la placa
  
  if (! versiondata) { //Si no se encuentra comunicación con la placa --->
  while (1); // Detener
  //Serial.print("NO SE ENCONTRO EL LECTOR"); //COLOCAR CODIGO DE ERROR A EDGARDO ++++++++++++++++++++
  }
  
  //Establezca el número máximo de reintentos para leer de una tarjeta. REVISAR++++++++++++++++
  //Esto evita que tengamos que esperar eternamente por una tarjeta,
  //que es el comportamiento predeterminado del PN532. 
  nfc.setPassiveActivationRetries(0xFF);
  nfc.SAMConfig();
  //Serial.print("TARJETA SETUP");
}

String IDTarjeta(){ //FUNCIÓN QUE LEE EL ID DE LA TARJETA
String id_tarjeta;
String ID = "";
  if(ID == ""){
    uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Búfer para almacenar el UID devuelto.
    uint8_t LongitudUID; //Variable para almacenar la longitud del UID de la tarjeta.
    boolean LeeTarjeta; //Variable para almacenar la detección de una tarjeta
    
    LeeTarjeta = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &LongitudUID); //LINEA IMPORTANTE
    cardid = uid[0];
    if(LeeTarjeta){
      cardid <<= 8;
      cardid |= uid[1];
      cardid <<= 8;
      cardid |= uid[2];
      cardid <<= 8;
      cardid |= uid[3];
      ID = String(cardid); //SE REGRESA ESTE VALOR AL CONDICIONAL DEL CICLO?? ++++++
    }
    return ID;
  }else{
    return id_tarjeta = IDTarjeta();
  }
  }


void proceso(void*ptr){
  //AL PRESIONAR EL BOTON VERDE, SE EVALUA LA PRESENCIA DE LA TARJETA Y SI ES VERDADERA, SE CONTINUA. DE LO CONTRARIO SE MENCIONA EN LA PANTALLA
   digitalWrite(LED,HIGH);
   String ID = "";                  
   
   
   do{ //
    
    labelTarjeta.setText("Coloca tu tarjeta"); 
    ID = IDTarjeta();
    
   }while(ID == "");
   
   do{
    
    labelTarjeta.setText("Encontre tu tarjeta, presiona el boton verde");
    
    

   }while(digitalRead(startBtn) != 0);
   
  //INICIO DEL PROCESO ....................................
  page1.show(); //CAMBIAR PAGINA A PROCESO
  labelproceso.setText("Presiona el boton rojo para terminar");

  //SEÑAL DE INICIO ESCLAVOS Y RASP
  digitalWrite(startSignal,HIGH);
  delay(2000);
  digitalWrite(startSignal,LOW); //APAGAMOS EL PIN?++++

  //ESPERA DE BOTON stopBtn ..............................
  do{
    
    //RECIBIR DATO ULTRASONICOS CADA 10S ??
    if(Serial.available()){ 
      String HCplastico = "";
      HCplastico = String(Serial.readString());
      material = HCplastico.charAt(0);
      for(int i = 1; i < HCplastico.length(); i++){
        porcentaje = porcentaje + HCplastico.charAt(i);
      }
      
      switch(material){
        case 'A':
        aluminio.setText (porcentaje);
        porcentaje = "";
        break;
        case 'P':
        plasticos.setText(porcentaje);
        porcentaje = "";
        break;
        case 'H':
        metal.setText(porcentaje);
        porcentaje = "";
        break;
        case 'O':
        organica.setText(porcentaje);
        porcentaje = "";
        break;
        case'D':
        desconocido.setText(porcentaje);
        porcentaje = "";
        break;
                                                                                                                                                                                                                                                                                                                                                               
      }
 
      

      
      }

    
  }while(digitalRead(stopBtn) != 0);

  //FINAL DEL PROCESO
  digitalWrite(stopSignal,HIGH);
  delay(3000);
  digitalWrite(stopSignal,LOW);
  digitalWrite(LED,LOW);

  //mandar ID a DB
  Serial.print("=");
  Serial.println(ID);
  
  ESP.restart();
  //MOSTRAR DATOS EN PANTALLA?? ++++++++++++++
  //SE MUESTRA UNA PANTALLA 10 SEGUNDOS CON LOS PUNTOS GENERADOS... Y KILOGRAMOS??? +++++++++++++++++++++
  

  }

void saldo(void*ptr){
    //MODIFICAR DESPUES+++++++++
    page2.show();
    do{

        

    }while(digitalRead(stopBtn) != 0);
    
    delay(3000);
    ESP.restart();

  }


void setup() {
  setupNextion();
  setupTarjeta();
  pinMode(startBtn,INPUT_PULLUP);
  pinMode(stopBtn,INPUT_PULLUP);
  pinMode(startSignal,OUTPUT);
  pinMode(stopSignal,OUTPUT);
  pinMode(LED,OUTPUT);
  Serial.begin(115200); 
}

void loop() {

  nexLoop(nex_listen_list);
  
}
