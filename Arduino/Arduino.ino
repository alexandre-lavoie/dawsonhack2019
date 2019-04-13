/*
 * Code created by Alexandre Lavoie of team int kiwi at Dawson Hackathon 2019.
 */

// Includes
#include <Servo.h>
#include <IRremote.h>

// Pins
#define S0 2
#define S1 3
#define S2 4
#define S3 5
#define LEFT_OUT 6
#define RIGHT_OUT 7
#define LEFT_MOTOR_PIN 9
#define RIGHT_MOTOR_PIN 10
#define IR_PIN 11

// IR Controller
#define BUTTON_OK 0xFF38C7
#define BUTTON_LEFT 0xFF10EF
#define BUTTON_RIGHT 0xFF5AA5
#define BUTTON_UP 0xFF18E7
#define BUTTON_DOWN 0xFF4AB5
#define BUTTON_0 0x000000 // CHANGE
#define BUTTON_1 0x000001 // CHANGE
IRrecv irrecv(IR_PIN);
decode_results results;

// Servo Calibration Constants
#define LEFT_MOTOR_ZERO 90
#define RIGHT_MOTOR_ZERO 90
#define LEFT_MOTOR_SPEED 30
#define RIGHT_MOTOR_SPEED -20
#define TURN_DELAY 250

// Servos
Servo left_motor;
Servo right_motor;

// States
bool auto_pilot = false;

void setup() {
  // Color Sensor
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(LEFT_OUT, INPUT);
  pinMode(RIGHT_OUT, INPUT);
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);

  // Remote Sensor
  irrecv.enableIRIn();

  // Servos
  left_motor.attach(LEFT_MOTOR_PIN);
  right_motor.attach(RIGHT_MOTOR_PIN);

  // Serial
  Serial.begin(9600);
}

void loop() {
  
  scan_controller();

  if(auto_pilot){
    line_follow();
  }else{
    line_avoid();  
  }
}

/*
 * Gets controller input and applies function.
 */

void scan_controller() {
  if(irrecv.decode(&results)){
    irrecv.resume();

    Serial.println(results.value, HEX);
    
    switch(results.value){
      case BUTTON_RIGHT:
        drive_and_stop(LEFT_MOTOR_SPEED, 0, TURN_DELAY);
        break;
      case BUTTON_LEFT:
        drive_and_stop(0, RIGHT_MOTOR_SPEED, TURN_DELAY);
        break;  
      case BUTTON_OK:
        drive(0, 0);
        break;
      case BUTTON_UP:
        drive(LEFT_MOTOR_SPEED, RIGHT_MOTOR_SPEED);
        break;
      case BUTTON_DOWN:
        drive(-LEFT_MOTOR_SPEED, -RIGHT_MOTOR_SPEED);
        break;
      case BUTTON_0:
        auto_pilot = true;
        break;
      case BUTTON_1:
        auto_pilot = false;
        break;
    }
  }  
}

/*
 * Line avoidance.
 */

void line_avoid(){
    if(is_green(LEFT_OUT))
      drive_and_stop(LEFT_MOTOR_SPEED, 0, TURN_DELAY);
    else if(is_blue(RIGHT_OUT))
      drive_and_stop(0, RIGHT_MOTOR_SPEED, TURN_DELAY);
}

/*
 * Uses the servos to drive the robot, given the shift.
 */

void drive(float left_motor_speed, float right_motor_speed){
  left_motor.write(LEFT_MOTOR_ZERO + left_motor_speed);
  right_motor.write(RIGHT_MOTOR_ZERO + right_motor_speed);
}

void drive_and_stop(float left_motor_speed, float right_motor_speed, int del){
  drive(left_motor_speed, right_motor_speed);
  delay(del);
  drive(0,0);  
}

/*
 * Line following.
 */

void line_follow(){
    if(is_blue(LEFT_OUT)){
      drive(0, RIGHT_MOTOR_SPEED);
    }else if(is_blue(RIGHT_OUT)){
      drive(LEFT_MOTOR_SPEED, 0); 
    }else{
      drive(LEFT_MOTOR_SPEED, RIGHT_MOTOR_SPEED);  
    }
}

/*
 * Get colors from sensor.
 */

bool is_blue(int pin){
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);

  int blueFrequency = pulseIn(pin, LOW);
  Serial.println(blueFrequency);

  delay(100);
  
  if(blueFrequency >= 470 && blueFrequency <= 700)
    return true;
  else
    return false;
}

bool is_green(int pin){
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);

  int greenFrequency = pulseIn(pin, LOW);
  Serial.println(greenFrequency);

  delay(100);
  
  if(greenFrequency >= 470 && greenFrequency <= 700)
    return true;
  else
    return false;
}
