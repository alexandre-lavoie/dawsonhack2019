/*
 * Code created by Alexandre Lavoie of team int kiwi at Dawson Hackathon 2019.
 */

// Includes
#include <Servo.h>
#include <IRremote.h>

// Pins
#define LEFT_MOTOR_PIN 9
#define RIGHT_MOTOR_PIN 10
#define IR_PIN 5
#define COLOR_SENSOR_PIN 6

// IR Controller
#define BUTTONOK 0xFF38C7
#define BUTTONLEFT 0xFF10EF
#define BUTTONRIGHT 0xFF5AA5
#define BUTTONUP 0xFF18E7
#define BUTTONDOWN 0xFF4AB5
IRrecv irrecv(IR_PIN);
decode_results results;

// Servo Calibration Constants
#define LEFT_MOTOR_ZERO 90
#define RIGHT_MOTOR_ZERO 90
#define LEFT_MOTOR_SPEED 30
#define RIGHT_MOTOR_SPEED -20

// Servos
Servo left_motor;
Servo right_motor;

void setup() {
  // Color Sensor
  pinMode(COLOR_SENSOR_PIN, INPUT);

  // Remote Sensor
  irrecv.enableIRIn();

  // On-board LED
  pinMode(LED_BUILTIN, OUTPUT);

  // Servos
  left_motor.attach(LEFT_MOTOR_PIN);
  right_motor.attach(RIGHT_MOTOR_PIN);

  // Serial
  Serial.begin(9600);
}

void loop() {
  //read_serial();
  scan_controller();
  
  //delay(100);
}

void scan_controller() {
  if(irrecv.decode(&results)){
    irrecv.resume();

    Serial.println(results.value, HEX);
    
    switch(results.value){
      case BUTTONRIGHT:
        drive(LEFT_MOTOR_SPEED, 0);
        delay(250);
        drive(0,0);
        break;
      case BUTTONLEFT:
        drive(0, RIGHT_MOTOR_SPEED);
        delay(250);
        drive(0,0);
        break;  
      case BUTTONOK:
        drive(0, 0);
        break;
      case BUTTONUP:
        drive(LEFT_MOTOR_SPEED, RIGHT_MOTOR_SPEED);
        break;
      case BUTTONDOWN:
        drive(-LEFT_MOTOR_SPEED, -RIGHT_MOTOR_SPEED);
        break;
    }
    
  }  
}

/*
 * Line following.
 */

void line_follow(){
    if(digitalRead(COLOR_SENSOR_PIN)){}
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
