// CocoRobo September 2017
int STBY = A5; //standby

//Motor A
int PWMA = 5; // Speed control
int AIN2 = 10; // Direction
int AIN1 = 9; // Direction

//Motor B
int PWMB = 6; // Speed control
int BIN1 = 4; // Direction
int BIN2 = 8; // Direction

int incomingByte = 0;
int currentSpeed = 80;

void setup() {
  pinMode(STBY, OUTPUT);

  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);

  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);

  Serial1.begin(115200);
}

void loop() {
  incomingByte = 0;

  if (Serial1.available() > 0) {
    incomingByte = Serial1.read();

    //Serial.print("I received: ");
    //Serial.println(incomingByte, DEC);
  }

  if (incomingByte == 50)
  { // Go Forward
    Serial.println("Going Forward Now.");
    Serial.println("-");
    motor(0, 0, 1, 1);
  } else if (incomingByte == 51)
  { // Go Backward
    Serial.println("Going Backward Now.");
    Serial.println("-");
    motor(0, 0, 0, 0);
  } else if (incomingByte == 52)
  { // Turn Left
    Serial.println("Turning Left Now.");
    Serial.println("-");
    motor(60, 0, 1, 1);
  }  else if (incomingByte == 53)
  { // Turn Right
    Serial.println("Turning Right Now");
    Serial.println("-");
    motor(0, 60, 1, 1);
  }  else if (incomingByte == 54)
  { // Stop
    Serial.println("Stop Now.");
    Serial.println("-");
    stop();
    delay(20);
  }

  //delay(50);
}

void motor(int valAddLeft, int valAddRight, int dirLeft, int dirRight) {
  move(0, currentSpeed - valAddLeft, dirLeft);
  move(1, currentSpeed - valAddRight, dirRight);
  delay(50);
}

void move(int motor, int speed, int direction) {
  //Move specific motor at speed and direction
  //motor: 0 for B 1 for A
  //speed: 0 is off, and 255 is full speed
  //direction: 0 clockwise, 1 counter-clockwise

  digitalWrite(STBY, HIGH); //disable standby

  boolean inPin1 = LOW;
  boolean inPin2 = HIGH;

  if (direction == 1) {
    inPin1 = HIGH;
    inPin2 = LOW;
  }

  if (motor == 1) {
    digitalWrite(AIN1, inPin1);
    digitalWrite(AIN2, inPin2);
    analogWrite(PWMA, speed);
  } else {
    digitalWrite(BIN1, inPin1);
    digitalWrite(BIN2, inPin2);
    analogWrite(PWMB, speed);
  }
}

void stop() {
  //enable standby
  digitalWrite(STBY, LOW);
}
