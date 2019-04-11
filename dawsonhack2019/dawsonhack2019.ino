/*
 * Code created by Alexandre Lavoie of team int kiwi at Dawson Hackathon 2019.
 */

// Includes
#include <Servo.h>

 // Ultrasonic Sensor Pins
const int TRIGGER_PIN = 2;
const int ECHO_PIN = 3;

// Servo Pins
const int LEFT_MOTOR_PIN;
const int RIGHT_MOTOR_PIN;

// Servos
Servo left_motor;
Servo right_motor;

void setup() {
  // Ultrasonic Sensor
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Servos
  left_motor.attach(LEFT_MOTOR_PIN);
  right_motor.attach(RIGHT_MOTOR_PIN);

  // Serial
  Serial.begin(9600);
}

float get_distance(){
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(4);

  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(12);
  digitalWrite(TRIGGER_PIN, LOW);

  long time_taken =  pulseIn(ECHO_PIN, HIGH);

  return (340.0 * (1.0 / 1000000.0) * (100.0) * (time_taken / 2.0));
}

void drive(float left_motor_speed, float right_motor_speed){
  left_motor.write(90 + left_motor_speed);
  right_motor.write(90 + right_motor_speed);
}

void loop() {
  drive(5,5);
  Serial.write(get_distance());
  delay(1000);
}
