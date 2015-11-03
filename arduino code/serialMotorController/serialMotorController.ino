#include <Wire.h>
#include <Adafruit_MotorShield.h>
#define FORWARD 1
#define BACKWARD 2
#define RELEASE 3

Adafruit_MotorShield MS;
Adafruit_DCMotor *leftm, *rightm;
byte LEFT_MOTOR_PORT = 1;
byte RIGHT_MOTOR_PORT = 2;
char incoming_msg[5] = "    ";
char msg_data[3];
char command;
int actual_speed;

void setup() {
  Serial.begin(9600);
  MS = Adafruit_MotorShield();
  MS.begin();
  leftm = MS.getMotor(LEFT_MOTOR_PORT);
  rightm = MS.getMotor(RIGHT_MOTOR_PORT);
  leftm -> setSpeed(0);
  rightm -> setSpeed(0);
  leftm -> run(BACKWARD);
  rightm -> run(FORWARD);
}

void loop() {
  if (Serial.available() >= 5) {
    Serial.readBytes(incoming_msg, 5);
    command = incoming_msg[4];
    msg_data[3] = incoming_msg[3];
    msg_data[2] = incoming_msg[2];
    msg_data[1] = incoming_msg[1];
    // last byte of message is string terminator
  }
  switch (command) {
    case 'L':
      leftm -> setSpeed(atoi(msg_data));
      break;
    case 'R':
      rightm -> setSpeed(atoi(msg_data));
      break;
    case 'S':
      drive(0, 0);
      break;
    default:
      // do nothing
      break;
  }
}

void drive(int l, int r) { // helper function to drive motors
  leftm -> setSpeed(l);
  rightm -> setSpeed(r);
}

