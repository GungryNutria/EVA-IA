#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

volatile int encoder01=0;
volatile int encoder02=0;
volatile int encoder03=0;

int startSignal = 18;

boolean estadoError = false;

const char* ssid = "ITQ-Eventos";
const char* password = "evento.2021";

String token;
DynamicJsonDocument doc(2048);
String response;

void rutina01() {
  encoder01++;
}
void rutina02(){
  encoder02++;
}
void rutina03(){
  encoder03++;
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

void setError(String token){
  HTTPClient http; 
  String request = "https://reciclate.queretaro.tecnm.mx/Reciclador-API/reciclador/update-capacity";
  http.begin(request);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Authorization", token);
  http.POST("[{\"id_reciclador\":\"13517193\",\"tipo_error\":\"1\"}]");
  response = http.getString();
  DeserializationError error = deserializeJson(doc, response);
  if(error){
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.f_str());
  }
  http.end();
}

void setup() {
  setupConnection();
  pinMode(25, INPUT);//encoder 1
  pinMode(33, INPUT);//encoder 2
  pinMode(32, INPUT);//encoder 3
  
  pinMode(18, INPUT); //Señal de start
  
  attachInterrupt(25, rutina01, FALLING);//encoder 1
  attachInterrupt(33, rutina02, FALLING);//encoder 2
  attachInterrupt(33, rutina03, FALLING);//encoder 3
 
  pinMode(35, OUTPUT);
   
  digitalWrite(35, estadoError);
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  
}

void loop() {
  
  // put your main code here, to run repeatedly:
  
  if(digitalRead(startSignal)== 1){ //Cuando es encendida la máquina (las bandas están en movimiento) se evalua el movimiento de las bandas
    
    if(encoder01<1 || encoder02<1 || encoder03<1 ){ //Si alguna (o todas) de las bandas no están en movimiento, envia la señal de error
    generateToken();
    setError(token);
    estadoError = true;
    digitalWrite(35, estadoError); //Señal de error enviada
  
   }
  }
  Serial.println( "RPM01: "); //impresion de las rpm de los motores (los motores están en movimiento, de no ser así se ejecutaría la condicion de error)
  
  encoder01=0;     //se regresa a cero el contador de la interrupcion para los encoder
  encoder02=0;
  encoder03=0;
  delay(1000);
  

  
}
