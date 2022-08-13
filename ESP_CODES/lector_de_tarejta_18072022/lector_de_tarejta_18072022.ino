#include "Nextion.h"
#include <SPI.h>
#include <SD.h>
#include "NexHardware.h"
#include <Wire.h>
#include <UNIT_PN532.h> //Librería Modificada
//Conexiones SPI del ESP32
#define PN532_SCK  (18)
#define PN532_MOSI (23)
#define PN532_SS   (5)
#define PN532_MISO (19)
//#define inicio 4 // nombre de mui variable
#define rx (0)
#define tx (1)

uint8_t DatoRecibido[4]; //Para almacenar los datos
UNIT_PN532 nfc(PN532_SS);// Línea enfocada para la comunicación por SPI
//DECLARAR EL OBJETO DEL TIPO DE COMPONENTE
NexText Tarjeta = NexText(0,8,"target");
NexText plastico = NexText (0,1,"cont1");
NexText hojalata = NexText (0,2,"cont2");
NexText aluminio = NexText (0,3,"cont3");
NexText organico = NexText (0,4,"cont4");
NexText desconocido = NexText (1,5,"cont5");//id de pagina,id de componente y nom obj
  //REGISTRAR LOS EVENTOS EN UNA LISTA
NexTouch *nex_listen_list[]=
{
&Tarjeta,
&plastico,
&hojalata,
&aluminio,
&organico,
&desconocido,
NULL
};

char material;
int p = 0;
int a = 0;
int h = 0;

uint32_t cardid;
uint32_t versiondata;

void readCard(){
  
      uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Búfer para almacenar el UID devuelto.
      uint8_t LongitudUID; //Variable para almacenar la longitud del UID de la tarjeta.
      boolean LeeTarjeta; //Variable para almacenar la detección de una tarjeta
      LeeTarjeta = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &LongitudUID);
      cardid = uid[0];
      if (LeeTarjeta) {
          cardid <<= 8;
          cardid |= uid[1];
          cardid <<= 8;
          cardid |= uid[2];
          cardid <<= 8;
          cardid |= uid[3];
          //int card=cardid;
          Tarjeta.setText(String(cardid));
          Serial.print('=');
          Serial.println("START");  
      }else{
          Serial.print('=');
          Serial.println("STOP");  
      }
}

void addMaterial(char c){
  switch(c){
          case 'P':
            p = p + 1;
            plastico.setText(String(p));
            
            delay(2000);
          break;
          case 'H':
            h = h + 1;
            hojalata.setText(String(h));
            
            delay(2000);
          break;
          case 'A':
            a = a + 1;
            aluminio.setText(String(a));
            
            delay(2000);
            
          break;
          case 'F':
            
          break;
  }
}


void setupCard(){
  nfc.begin(); //Comienza la comunicación del PN532
  versiondata = nfc.getFirmwareVersion();//Obtiene información de la placa
  
  if (! versiondata) { //Si no se encuentra comunicación con la placa --->
  while (1); // Detener
  }
  
  //Establezca el número máximo de reintentos para leer de una tarjeta.
  //Esto evita que tengamos que esperar eternamente por una tarjeta,
  //que es el comportamiento predeterminado del PN532. 
  nfc.setPassiveActivationRetries(0xFF);
  nfc.SAMConfig();
}

void setup() {
  nexInit();
  Serial.begin(115200);
  setupCard();
}

void loop() {
       readCard();
       if(Serial.available()){material = Serial.read();}
//       showLed(material);
       addMaterial(material);
  
nexLoop(nex_listen_list);
//plastico.setText("98");
//hojalata.setText("987");
//aluminio.setText("9876");
//organico.setText("98765");
//desconocido.setText("987654");
}



    
