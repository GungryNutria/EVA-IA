#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>

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

int maquinaEstado = 0;

String porcentaje;
String token;
String creditos;
const char* ssid = "ITQ-Eventos";
const char* password = "evento.2021";
String response;
String GLOBAL_ID = "";   
DynamicJsonDocument doc(2048);
int a=0;
int p=0;                                                                                                                   
int h=0;                                                                                                                   
int d=0;                                                                                                                   
int o=0;                                                                                                                   
UNIT_PN532 nfc(PN532_SS);// Línea enfocada para la comunicación por SPI
String codigo;

//**********************************NEXTION*************************************
//OBJETOS NEXTION
NexButton procesoBtn = NexButton(0,2,"b0"); //COLOCAR IDS +++++
NexButton saldoBtn = NexButton(0,3,"b1");
NexButton finalBtn = NexButton(1,0,"b2");
NexText labelTarjeta = NexText(0,1,"t0");
NexPage page0 = NexPage(0,0,"page0"); //PASAR PAGINA DE PROCESO++++++++++
//PROCESOS OBJ
NexText labelproceso = NexText(1,6,"t5");
NexText plasticos = NexText(1,1,"t0");
NexText aluminio = NexText(1,2,"t1");
NexText metal = NexText(1,3,"t2");
NexPage page1 = NexPage(1,1,"page1"); //PASAR PAGINA DE SALDO++++++++++
//SALDOS OBJ
NexPage page2 = NexPage(2,0,"page2");
NexText labelsaldo = NexText(2,1,"t5");

//REGISTRO DE EVENTOS
NexTouch *nex_listen_list[]{
  &procesoBtn,
  &saldoBtn,
  &finalBtn,
  NULL
  };

//********************************FUNCIONES*****************************************
void setupNextion(){ //INICIAR SETUP NEXTION
  
  nexInit();
  Serial.begin(115200); 
  procesoBtn.attachPush(proceso, &procesoBtn);
  saldoBtn.attachPush(saldo, &saldoBtn);
  finalBtn.attachPush(final, &finalBtn);
  page0.show();
  }

void setupConnection(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,password);
  // put your setup code here, to run once:
}

void generateToken(){
  HTTPClient http;
  String request = "https://reciclate.queretaro.tecnm.mx/Reciclador-API/auth";
  http.begin(request);
  http.addHeader("Content-Type", "application/json");
  http.POST("{\"email\":\"gGisDLl6\",\"password\":\"Z6aCgi8yGr80dbXQ\"}");
  response = http.getString();
  DeserializationError error = deserializeJson(doc, response);
  if(error){
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.f_str());
  }
  token = doc["token"].as<String>();
  http.end();

}

void getCredits(String token, String tarjeta){
  HTTPClient http;
  String request = "https://reciclate.queretaro.tecnm.mx/Reciclador-API/reciclador/Consulta-Saldo";
  http.begin(request);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Authorization", token);
  http.POST("{\"tarjeta_uuid\":\""+tarjeta+"\"}");
  response = http.getString();
  DeserializationError error = deserializeJson(doc, response);
  if(error){
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.f_str());
    labelsaldo.setText("ERROR");
  }
  if(doc["codigo"].as<String>() == "200"){
    creditos = doc["mensaje"].as<String>();
  }else if(doc["codigo"].as<String>() == "403"){
    creditos = doc["mensaje"].as<String>();
  }else{
    creditos = "Error de Conexion";
  }
  http.end();
}

void ConfirmTarjeta(String token, String tarjeta){
  HTTPClient http;
  String request = "https://reciclate.queretaro.tecnm.mx/Reciclador-API/reciclador/Consulta-Saldo";
  http.begin(request);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Authorization", token);
  http.POST("{\"tarjeta_uuid\":\""+tarjeta+"\"}");
  response = http.getString();
  DeserializationError error = deserializeJson(doc, response);
  if(error){
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.f_str());
    labelsaldo.setText("ERROR");
  }
  codigo = doc["codigo"].as<String>();
  http.end();
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
  maquinaEstado = 1;
  //AL PRESIONAR EL BOTON VERDE, SE EVALUA LA PRESENCIA DE LA TARJETA Y SI ES VERDADERA, SE CONTINUA. DE LO CONTRARIO SE MENCIONA EN LA PANTALLA
  digitalWrite(LED,HIGH);
         
   
   do{ //
    
    labelTarjeta.setText("Coloca tu tarjeta"); 
    GLOBAL_ID = IDTarjeta();
   }while(GLOBAL_ID == "");
  
  //INICIO DEL PROCESO ....................................
  page1.show(); //CAMBIAR PAGINA A PROCESO
  labelproceso.setText("Presiona el boton rojo para terminar");

  //SEÑAL DE INICIO ESCLAVOS Y RASP
  digitalWrite(startSignal,HIGH);
  delay(2000);
  digitalWrite(startSignal,LOW); //APAGAMOS EL PIN?++++
  //MOSTRAR DATOS EN PANTALLA?? ++++++++++++++
  //SE MUESTRA UNA PANTALLA 10 SEGUNDOS CON LOS PUNTOS GENERADOS... Y KILOGRAMOS??? +++++++++++++++++++++

  
  }

void saldo(void*ptr){
    //MODIFICAR DESPUES+++++++++
  digitalWrite(LED,HIGH);
  String ID = ""; 
   
  do{ //
    
    labelTarjeta.setText("Coloca tu tarjeta"); 
    ID = IDTarjeta();
    
  }while(ID == "");
  
  page2.show();
  //mandar ID a DB
    generateToken();
    labelsaldo.setText(ID);
    getCredits(token, ID);
    labelsaldo.setText(creditos);  
    delay(3000);
    ESP.restart();

  }

void final(void*ptr){
  maquinaEstado=0;
  //FINAL DEL PROCESO
  digitalWrite(stopSignal,HIGH);
  delay(3000);
  digitalWrite(stopSignal,LOW);
  digitalWrite(LED,LOW);
  //mandar ID a DB
  Serial.print("=");
  Serial.println(GLOBAL_ID);
  delay(1000);
  ESP.restart();
}

void setup() {
  setupConnection();
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
  if(maquinaEstado == 1){
    if(Serial.available()){
      material = Serial.read();
      switch(material){
          case 'A':
          a++;
          aluminio.setText(String(a));
          break;
          case 'P':
          p++;
          plasticos.setText(String(p));
          break;
          case 'H':
          h++;
          metal.setText(String(h));
          break;
          case 'F':
          break;
        }
      material = 'F';
        
    }
  }
  nexLoop(nex_listen_list);
  
}



    /*RECIBIR DATO ULTRASONICOS CADA 10S ??
    if(Serial.available()){
      HCmaterial = String(Serial.readString());
    
      material = HCmaterial.charAt(0);
      
      for(int i = 1; i < HCmaterial.length(); i++){
        
        porcentaje = porcentaje + HCmaterial.charAt(i);
      }
 
      HCmaterial = "";
      
    switch(material){
        case 'A':
        aluminio.setText(porcentaje);
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
        case 'D':
        desconocido.setText(porcentaje);
        porcentaje = "";
        break;
        case 'a':
        aluminio.setText ("LLENO");
        porcentaje = "";
        break;
        case 'p':
        plasticos.setText ("LLENO");
        porcentaje = "";
        break;
        case 'h':
        metal.setText ("LLENO");
        porcentaje = "";
        break;
        case 'o':
        organica.setText (porcentaje);
        porcentaje = "";
        break;
        */
