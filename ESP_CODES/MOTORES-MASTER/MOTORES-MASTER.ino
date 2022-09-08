#define banda1 22
#define banda2 23
#define banda3 1

#define stopSignal 32
#define startSignal 33

void setup() {
  // put your setup code here, to run once:
  pinMode(banda1, OUTPUT);
  pinMode(banda2, OUTPUT);
  pinMode(banda3, OUTPUT);

  pinMode(startSignal, INPUT);
  pinMode(stopSignal, INPUT);
}

void loop() {
  if (digitalRead(startSignal) == 1) {
    do {
      digitalWrite(banda1, HIGH);
      digitalWrite(banda2, HIGH);
      digitalWrite(banda3, HIGH);
    } while (digitalRead(stopSignal) != 1);
  }

}
