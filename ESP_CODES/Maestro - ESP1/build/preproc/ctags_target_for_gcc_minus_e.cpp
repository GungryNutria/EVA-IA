# 1 "d:\\Escritorio\\Maquina\\Maestro - ESP1\\Maestro-ESP1.ino"
//***********************************LIBRERIAS**************************
# 3 "d:\\Escritorio\\Maquina\\Maestro - ESP1\\Maestro-ESP1.ino" 2
# 4 "d:\\Escritorio\\Maquina\\Maestro - ESP1\\Maestro-ESP1.ino" 2
# 5 "d:\\Escritorio\\Maquina\\Maestro - ESP1\\Maestro-ESP1.ino" 2


//****************************DECLARACION DE PINES*********************************
#define startBtn 13
#define stopBtn 33
#define PN532_SCK 18
#define PN532_MOSI 23
#define PN532_SS 5
#define PN532_MISO 19

#define LED 2

#define startSignal 32
#define stopSignal 27

//***************************DECLARACION DE VARIABLES****************************
uint8_t DatoRecibido[4]; //Para almacenar los datos
uint32_t cardid;
uint32_t versiondata;

UNIT_PN532 nfc(5);// Línea enfocada para la comunicación por SPI

//**********************************NEXTION*************************************
//OBJETOS NEXTION
NexButton procesoBtn = NexButton(0,1,"b0"); //COLOCAR IDS +++++
NexButton saldoBtn = NexButton(0,2,"b1");
NexText labelTarjeta = NexText(0,3,"t0");
NexPage page0 = NexPage(0,0,"page0"); //PASAR PAGINA DE PROCESO++++++++++
//PROCESOS OBJ
NexText labelproceso = NexText(1,1,"t0");
NexText plasticos = NexText(1,2,"t1");
//NexText aluminio = NexText(1,2,"REVISAR");
//NexText metal = NexText(1,3,"REVISAR");
//NexText organica = NexText(1,4,"REVISAR");
NexPage page1 = NexPage(1,1,"page1"); //PASAR PAGINA DE SALDO++++++++++
//SALDOS OBJ
NexPage page2 = NexPage(2,0,"page2");

//REGISTRO DE EVENTOS
NexTouch *nex_listen_list[]{
  &procesoBtn,
  &saldoBtn,
  
# 47 "d:\\Escritorio\\Maquina\\Maestro - ESP1\\Maestro-ESP1.ino" 3 4
 __null
  
# 48 "d:\\Escritorio\\Maquina\\Maestro - ESP1\\Maestro-ESP1.ino"
 };

//********************************FUNCIONES*****************************************
void setupNextion(){ //INICIAR SETUP NEXTION

  nexInit();
  Serial.begin(115200); //QUITAR DESPUES++++
  procesoBtn.attachPush(proceso, &procesoBtn);
  saldoBtn.attachPush(saldo, &saldoBtn);
  Serial.print("NEXTION SETUP");
  page0.show();

  }

void setupTarjeta(){ //REVISAR+++++++++++++++
  nfc.begin(); //Comienza la comunicación del PN532
  versiondata = nfc.getFirmwareVersion();//Obtiene información de la placa

  if (! versiondata) { //Si no se encuentra comunicación con la placa --->
  while (1); // Detener
  Serial.print("NO SE ENCONTRO EL LECTOR"); //COLOCAR CODIGO DE ERROR A EDGARDO ++++++++++++++++++++
  }

  //Establezca el número máximo de reintentos para leer de una tarjeta. REVISAR++++++++++++++++
  //Esto evita que tengamos que esperar eternamente por una tarjeta,
  //que es el comportamiento predeterminado del PN532. 
  nfc.setPassiveActivationRetries(0xFF);
  nfc.SAMConfig();
  Serial.print("TARJETA SETUP");
}

String IDTarjeta(){ //FUNCIÓN QUE LEE EL ID DE LA TARJETA
String id_tarjeta;
String ID = "";
  if(ID == ""){
    uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 }; // Búfer para almacenar el UID devuelto.
    uint8_t LongitudUID; //Variable para almacenar la longitud del UID de la tarjeta.
    boolean LeeTarjeta; //Variable para almacenar la detección de una tarjeta

    LeeTarjeta = nfc.readPassiveTargetID((0x00), &uid[0], &LongitudUID); //LINEA IMPORTANTE
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
   String ID = "";

   do{ //

    labelTarjeta.setText("Coloca tu tarjeta");
    ID = IDTarjeta();

   }while(ID == "");

   do{

    labelTarjeta.setText("Encontre tu tarjeta, presiona el boton verde");



   }while(digitalRead(13) != 0);

  //INICIO DEL PROCESO ....................................
  page1.show(); //CAMBIAR PAGINA A PROCESO
  labelproceso.setText("Presiona el boton rojo para terminar");

  //SEÑAL DE INICIO ESCLAVOS Y RASP
  digitalWrite(32,0x1);
  delay(100);
  digitalWrite(32,0x0); //APAGAMOS EL PIN?++++

  //ESPERA DE BOTON stopBtn ..............................
  do{

    //RECIBIR DATO ULTRASONICOS CADA 10S ??
    if(Serial.available()){ //REVISAR +++++++++++++++++
      //IMPRIMIR EN PANTALLA EL VALOR DE ULTRASONICOS CON MILLIS
      plasticos.setText("VARIABLE");
      //metal.setText("REVISAR");
      //aluminio.setText("REVISAR");
      //organica.setText("REVISAR");


      }


  }while(digitalRead(33) != 0);

  //FINAL DEL PROCESO
  digitalWrite(27,0x1);
  delay(100);
  digitalWrite(27,0x0);

  //MOSTRAR DATOS EN PANTALLA?? ++++++++++++++
  //SE MUESTRA UNA PANTALLA 10 SEGUNDOS CON LOS PUNTOS GENERADOS... Y KILOGRAMOS??? +++++++++++++++++++++

  delay(3000);
  page0.show();

  }

void saldo(void*ptr){
    //MODIFICAR DESPUES+++++++++
    page2.show();
    do{



    }while(digitalRead(33) != 0);

    delay(3000);
    page0.show();

  }


void setup() {
  setupNextion();
  setupTarjeta();
  pinMode(13,0x05);
  pinMode(33,0x05);
  pinMode(32,0x02);
  pinMode(27,0x02);
  Serial.begin(115200);
  labelTarjeta.setText("LISTO");


}

void loop() {

  nexLoop(nex_listen_list);

}
