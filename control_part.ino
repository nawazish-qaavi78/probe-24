#include <Servo.h>
Servo servo;
int msp = 255;
int trig = 2;
int echo = 4;
int speed = 3;
int dir1 = 7;  //left motor (YELLOW WIRES) //assuming clockwise
int dir2 = 8;
int d1 = 12;  //right motor (RED WIRES)
int d2 = 13;
int leftir = 9;
int rightir = 10;
int rightdis = 0, leftdis = 0, middis = 0;
void forward() {
  analogWrite(speed, msp);
  digitalWrite(dir1, HIGH);
  digitalWrite(d1, HIGH);
  digitalWrite(dir2, LOW);
  digitalWrite(d2, LOW);
  Serial.println("FORWARD");
}
void backward() {
  analogWrite(speed, msp);
  digitalWrite(dir2, HIGH);
  digitalWrite(d2, HIGH);
  digitalWrite(dir1, LOW);
  digitalWrite(d1, LOW);
  Serial.println("BACKWARD");
}
void rightturn() {
  analogWrite(speed, msp);
  digitalWrite(dir1, LOW);
  digitalWrite(d1, HIGH);
  digitalWrite(dir2, HIGH);
  digitalWrite(d2, LOW);
  Serial.println("RIGHT");
}
void leftturn() {
  analogWrite(speed, msp);
  digitalWrite(dir1, HIGH);
  digitalWrite(d1, LOW);
  digitalWrite(dir2, LOW);
  digitalWrite(d2, HIGH);
  Serial.println("LEFT");
}
void stop() {
  digitalWrite(speed, LOW);
  Serial.println("STOP");
}
int distance() {
  digitalWrite(trig, LOW);
  delayMicroseconds(10);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  int tt = pulseIn(echo, HIGH);
  int d = (0.0343 * tt) / 2;  // speed = 0.0343 cm/microsecond
  Serial.print("distance =");
  Serial.print(d);
  Serial.println(" cm");
  return d;
}
void setup() {
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(dir1, OUTPUT);
  pinMode(dir2, OUTPUT);
  pinMode(d1, OUTPUT);
  pinMode(d2, OUTPUT);
  pinMode(speed, OUTPUT);  //l293d driver had only one common enable pin
  pinMode(leftir, OUTPUT);
  pinMode(rightir, OUTPUT);
  Serial.begin(9600);
  servo.attach(5);
}
void loop() {
  //OBSTACLE AVOIDANCE CODE !
  servo.write(90);
  delay(500);
  middis = distance();
  if (middis <= 15) {
    stop();
    delay(500);
    servo.write(10);
    delay(500);
    leftdis = distance();
    delay(250);
    servo.write(180);
    delay(500);
    rightdis = distance();
    delay(250);
    servo.write(90);
    delay(1000);
    if (rightdis > leftdis) {
      rightturn();
      delay(250);
    } else if (leftdis > rightdis) {
      leftturn();
      delay(250);
    } else if ((rightdis <= 15) || (leftdis <= 15)) {
      backward();
    }
  } else {
    forward();
  }
  //Hand-follow mode
  int leftSensor = digitalRead(leftir);  // Read sensor values directly
  int rightSensor = digitalRead(rightir);
  if (leftSensor == LOW && rightSensor == HIGH) {
    leftturn();
  } else if (leftSensor == HIGH && rightSensor == LOW) {
    rightturn();
  } else if (leftSensor == LOW && rightSensor == LOW) {
    int action = distance();
    if (action >= 15) {
      forward();
    } else if (action <= 10) {
      backward();
    } else {
      stop();
    }
  } else {
    stop();
  }
}
