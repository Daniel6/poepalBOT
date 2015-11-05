#include <Wire.h>
#include <Adafruit_MotorShield.h>
#define FORWARD 1
#define BACKWARD 2
#define RELEASE 3

Adafruit_MotorShield MS;
Adafruit_DCMotor *leftm, *rightm;
byte LEFT_MOTOR_PORT = 1;
byte RIGHT_MOTOR_PORT = 2;
int LEFT_SENSOR_PIN = A5, RIGHT_SENSOR_PIN = A3;
int LEFT_SENSOR_THRESH = 800;
int RIGHT_SENSOR_THRESH = 800;
int left_sensor, right_sensor;
char incoming_msg[4] = "   "; //three chars and a string temrinator
char command_arg[3];
char command;
int actual_speed;
boolean argParse = false;
boolean foundLine = false;

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
  if (Serial.available()) {
    if (!argParse) {
      command = Serial.read();
      resetDriveParams();
      //flag argParse if command needs additional argument
      argParse = (command == 'L' || command == 'R');
    } else {
      //we are looking for arguments to command
      //currently only looking for 3 byte arguments
      if (Serial.available() >= 4) { //4 bytes indicates 3 bytes and one string terminator
        Serial.readBytes(incoming_msg, 4);
        //store 3 bytes of argument
        command_arg[3] = incoming_msg[3];
        command_arg[2] = incoming_msg[2];
        command_arg[1] = incoming_msg[1];
        argParse = false; //deflag argParse so we can read the next msg
      }
    }
  }
  switch (command) { //Continually operate on last received command
    case 'L': //Control Left Motor
      if (!argParse) {
        //only set motor speed when we get the desired speed
        leftm -> setSpeed(atoi(command_arg));
      }
      break;
      
    case 'R': //Control Right Motor
      if (!argParse) {
        //only set motor speed when we get the desired speed
        rightm -> setSpeed(atoi(command_arg));
      }
      break;
      
    case 'S': //Stop
      drive(0, 0);
      break;
      
    case 'F': //Follow Line
      followLine();
      break;

    case '<': //Turn left until you find a line
      findLineLeft();
      break;

    case '>': //Turn right until you find a line
      findLineRight();
      break;
      
    default:
      //change nothing
      break;
  }
}

void findLineLeft() {
  left_sensor = analogRead(LEFT_SENSOR_PIN);

  if (!foundLine) {
    if (left_sensor < LEFT_SENSOR_THRESH) {
      drive(-20, 20);
    } else {
      foundLine = true;
    }
  } else {
    if (left_sensor > LEFT_SENSOR_THRESH) {
      drive(-20, 20);
    } else {
      drive(0, 0);
    }
  }
}

void findLineRight() {
  right_sensor = analogRead(RIGHT_SENSOR_PIN);

  if (!foundLine) {
    if (right_sensor < RIGHT_SENSOR_PIN) {
      drive(-20, 20);
    } else {
      foundLine = true;
    }
  } else {
    if (right_sensor > RIGHT_SENSOR_PIN) {
      drive(-20, 20);
    } else {
      drive(0, 0);
    }
  }
}

void followLine() {
  left_sensor = analogRead(LEFT_SENSOR_PIN);
  right_sensor = analogRead(RIGHT_SENSOR_PIN);

  if (left_sensor > LEFT_SENSOR_THRESH) {
    //Left sensor is over line
    //Swerve left until left sensor is not over line
    drive(40, 50); //Slow down left wheel to swerve left
  } else if (right_sensor > RIGHT_SENSOR_THRESH) {
    //Right sensor is over line
    //Swerve right until right sensor is not over line
    drive(50, 40); //Slow down right wheel to swerve right
  } else {
    //No sensors are over line
    //Drive straight
    drive(50, 50);
  }
}

void drive(int l, int r) { // helper function to drive motors
  leftm -> setSpeed(l);
  rightm -> setSpeed(r);
}

void resetDriveParams() {
  foundLine = false;
}
