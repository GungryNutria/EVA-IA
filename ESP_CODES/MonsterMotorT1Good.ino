/// Motor 1
const byte M1CWPin = 5;  // INA: MotorShield 7
const byte M1CCWPin = 18;  // INB: MotorShield 8
const byte M1PWMPin = 19;  // PWM MotorShield 5

void setup() {
  Serial.begin(115200);
  pinMode(M1CWPin, OUTPUT);
  pinMode(M1CCWPin, OUTPUT);

  digitalWrite(M1CWPin, LOW);
  digitalWrite(M1CCWPin, LOW);

  pinMode(M1PWMPin, OUTPUT); //MOTOR SPEED
}

void loop() {
  // Motor 1 ClockWise
  digitalWrite(M1CWPin, HIGH);
  digitalWrite(M1CCWPin, LOW);

  // CONFIGURAS LA VEL. MAXIMA 0 -255
  analogWrite(M1PWMPin, 255);//0 - 255 

  //MOVIMIENTO POR 4 SEG.
  delay (4000);

//SE DETIENE CLCK PARA HACER ALTO TOTAL.
digitalWrite(M1CWPin, LOW);
digitalWrite(M1CCWPin, LOW);
//VELOCIDAD 0 PARA ALTO TOTAL 
analogWrite(M1PWMPin, 0);
 delay(2000);
}