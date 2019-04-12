/*
 * Code created by Alexandre Lavoie of team int kiwi at Dawson Hackathon 2019.
 */

// Includes
#include <Servo.h>

 // Ultrasonic Sensor Pins
const int TRIGGER_PIN = 2;
const int ECHO_PIN = 3;

// Servo Pins
const int LEFT_MOTOR_PIN = 1;
const int RIGHT_MOTOR_PIN = 4;

// Servo Calibration Constants
const int LEFT_MOTOR_ZERO = 90;
const int RIGHT_MOTOR_ZERO = 90;
const int LEFT_MOTOR_SPEED = 20;
const int RIGHT_MOTOR_SPEED = 20;

// Servos
Servo left_motor;
Servo right_motor;

void setup() {
  // Ultrasonic Sensor
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // On-board LED
  pinMode(LED_BUILTIN, OUTPUT);

  // Servos
  left_motor.attach(LEFT_MOTOR_PIN);
  right_motor.attach(RIGHT_MOTOR_PIN);

  // Serial
  Serial.begin(9600);
}

/*
 * Gets distance in front of the ultrasonic sensor.
 */

float get_distance(){
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(4);

  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(12);
  digitalWrite(TRIGGER_PIN, LOW);

  long time_taken =  pulseIn(ECHO_PIN, HIGH);

  return (340.0 * (1.0 / 1000000.0) * (100.0) * (time_taken / 2.0));
}

/*
 * Uses the servos to drive the robot, given the shift.
 */

void drive(float left_motor_speed, float right_motor_speed){
  left_motor.write(LEFT_MOTOR_ZERO + left_motor_speed);
  right_motor.write(RIGHT_MOTOR_ZERO + right_motor_speed);
}

/*
 * Read the serial, returns the key and caries function.
 */

void read_serial(){
   if(Serial.available()>0){
     int byte_in = Serial.read();
     Serial.write(byte_in);
     
     switch(byte_in){
      case 'p':
       Serial.write(get_distance());
       break;
      case 'b':
        digitalWrite(LED_BUILTIN, HIGH);
        delay(1000);
        digitalWrite(LED_BUILTIN, LOW);
        break;
      case 'w':
        drive(LEFT_MOTOR_SPEED, RIGHT_MOTOR_SPEED);
        break;
      case 'a':
        drive(0, RIGHT_MOTOR_SPEED);
        break;
      case 'd':
        drive(LEFT_MOTOR_SPEED, 0);
        break;
      case 's':
        drive(-LEFT_MOTOR_SPEED, -RIGHT_MOTOR_SPEED);
        break;
     }
  }
}

void loop() {
  read_serial();
  
  delay(100);
}
