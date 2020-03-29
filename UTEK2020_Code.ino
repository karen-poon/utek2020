/* Pseudo Code
   First the robot must be autonomous (e.g. line sensor and detect the path)
   The robot must be able to detect the different "houses"
   Recognize how much timbits to provide
   Q: How are we dropping off the timbits?
   When does the robot know when to continue on path
*/
#include <Servo.h>

int line_sensor1 = 2; //left sensor
int colour1 = 0;
int line_sensor2 = 4; //right sensor
int colour2 = 0;

int echoPin = 12;
int trigPin = 13;

float t = 0;
float distance = 0;

#define enL 3
#define enR 5
int in1 = 7;  //MotorLeft
int in2 = 6;  //MotorLeft
int in3 = 8;  //MotorRight
int in4 = 11;  //MotorRight

Servo servo1;  // first servo
Servo servo2;  // second servo
int servoPinFirst = 9;
int servoPinSecond = 10;

int ledPin = A0; //analog pin

/*global variables*/
int servo1_from = 0, servo1_to = 60;
int servo2_from = 0, servo2_to = 45;
int timbits_to_unload[5] = {1,1,1,0,2}; 
int fcu = 1; //keep track of current house
int current_timbits_unloaded = 0;
int current_house = 0;

void moveFwd() {
  analogWrite(enL, 255);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(enR, 255);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void turnLeft() {
  analogWrite(enL, 0);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  analogWrite(enR, 255);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void turnRight() {
  analogWrite(enL, 255);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(enR, 0);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void moveStop() {
  analogWrite(enL, 0);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(enR, 0);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void activate_servo_1(int from, int to) {
  for (int angle = from; angle <= to; angle++) { 
    servo1.write(angle);
    delay(15);
  }
}

void activate_servo_2(int from, int to) {
  for (int angle = from; angle <= to; angle++) { 
    servo2.write(angle);
    delay(15);
  }
}

//use this for activating the servo
void activate_servo() {
  if (current_timbits_unloaded <= 2) {
    Serial.println("Activate Servo 1");
    activate_servo_1(servo1_from, servo1_to);
    servo1_from = servo1_from + 60;
    servo1_to = servo1_to + 60;
    current_timbits_unloaded++;
  } 
  else if (current_timbits_unloaded > 2 && current_timbits_unloaded <= 5) {
    Serial.println("Activate Servo 2");
    activate_servo_2(servo2_from, servo2_to);
    servo2_from = servo2_from + 45;
    servo2_to = servo2_to + 45;
    current_timbits_unloaded++;
  }
}

/*********************************************************************/

void setup() {
  // put your setup code here, to run once:
  pinMode(line_sensor1, INPUT);

  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);

  pinMode(enL, OUTPUT);
  pinMode(enR, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  servo1.attach(servoPinFirst);  // attaches the first servo
  servo1.write(0);
  servo2.attach(servoPinSecond); //attaches the second servo
  servo2.write(0);

  analogWrite(ledPin, 0);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  //line sensor1
  colour1 = digitalRead(line_sensor1);
  if (colour1 == 1) {
    Serial.println("This is black");
  }
  else {
    Serial.println("This is white");
  }

  //line sensor2
  colour2 = digitalRead(line_sensor2);
  if (colour2 == 1) {
    Serial.println("This is black");
  }
  else {
    Serial.println("This is white");
  }

  //ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  t = pulseIn(echoPin, HIGH);
  distance = (0.034 * t) / 2; //distance in cm
  Serial.println(distance);
  delay(1000);

  //when it detects the house
  if (distance <= 20) {
    current_house++;
    moveStop();
    delay(3000);

    for (int i = 0; i < timbits_to_unload[current_house]; i++) {
      activate_servo();
    }
  }
  else {
    if (colour1 != 1 && colour2 != 1) {
      moveFwd();
      delay(100);
      Serial.println("This moves forward");
      moveStop();
      delay(100);
    }
    else if (colour1 == 1 && colour2 != 1) {
      turnLeft();
      delay(100);
      Serial.println("This moves left");
      moveStop();
      delay(100);
    }
    else if (colour1 != 1 && colour2 == 1) {
      turnRight();
      delay(100);
      Serial.println("This moves right");
      moveStop();
      delay(100);
    }
    else {
      moveFwd();
    }
  }

  if (current_timbits_unloaded == 5) {
    analogWrite(ledPin, 255);
  }
}
