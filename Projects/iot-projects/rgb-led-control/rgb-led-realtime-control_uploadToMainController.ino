int redPin = 11;
int greenPin = 10;
int bluePin = 9;

// data definition
int incomingByte = 0;   // for incoming serial data

void setup() {
  delay(500);
  Serial1.begin(9600);

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT); 
}

void loop() {

  // send data only when you receive data:
  if (Serial1.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial1.read();

    // say what you got:
    Serial.print("I received: ");
    Serial.println(incomingByte, DEC);
  }
  switch (incomingByte) {
    case 50:
      setColor(255, 255, 0);
      break;
    case 52:
      setColor(0, 255, 255);
      break;
    case 54:
      setColor(0, 0,0);
      break;
  }
  Serial.println(incomingByte, DEC);
  
}

void setColor(int red, int green, int blue)
{
  #ifdef COMMON_ANODE
    red = 255 - red;
    green = 255 - green;
    blue = 255 - blue;
  #endif
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);  
}
