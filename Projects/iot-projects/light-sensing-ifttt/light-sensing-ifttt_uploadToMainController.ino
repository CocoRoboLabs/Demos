
#define PIN_LIGHT A1


void setup() {

  Serial1.begin(9600);

  pinMode(PIN_LIGHT, INPUT);
}

void loop() {

  int lightValue = analogRead(PIN_LIGHT);
  int lightState;

  if (lightValue > 50) {
    lightState = 0;
  } else if (lightValue < 50) {
    lightState = 1;
  }

  unsigned char storeBytes[1] = { lightState };
  Serial1.write(storeBytes, 1);
  
  delay(100);
}
