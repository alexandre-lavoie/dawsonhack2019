/*
 * Code created by Alexandre Lavoie of team int kiwi during Dawson Hackathon 2019.
 */

int kiwi = 255; // N.P - <3

// Includes
#include <Servo.h>
#include <IRremote.h>

// Color Sensors -> S0-S3 are pins on the color sensor. I realized during the competition I could put them in parallel.
#define S0 2
#define S1 3
#define S2 4
#define S3 5
#define LEFT_OUT 7 // Left Color Sensor Out
#define RIGHT_OUT 6 // Right Color Sensor Out

// Servos
#define LEFT_MOTOR_PIN 9
#define RIGHT_MOTOR_PIN 10
Servo left_motor;
Servo right_motor;
// Servo Calibration
#define LEFT_MOTOR_ZERO 90
#define RIGHT_MOTOR_ZERO 90
#define LEFT_MOTOR_SPEED 28
#define RIGHT_MOTOR_SPEED -20
#define TURN_DELAY 250

// 7 Segment Display
#define L1 A0
#define L2 A1
#define L3 A2
#define L4 A3
#define L5 A4
#define L6 A5
#define L7 12
// 7 Segment Display Timing and States for Drawings
int state = -1;
int timer = 0;

// IR Receiver
#define IR_PIN 11
#define BUTTON_OK 0xFF38C7
#define BUTTON_LEFT 0xFF10EF
#define BUTTON_RIGHT 0xFF5AA5
#define BUTTON_UP 0xFF18E7
#define BUTTON_DOWN 0xFF4AB5
#define BUTTON_STAR 0xFF6897
#define BUTTON_POUND 0xFFB04F
#define BUTTON_1 0xFFA25D
#define BUTTON_2 0xFF02FD
#define BUTTON_3 0xFFE21D
#define HOLD 0xFFFFFF // Not used, but I realized the IR Controller sent this value when any button held (Better controls for turning?)
IRrecv irrecv(IR_PIN);
decode_results results;

// States
bool auto_pilot = false; // Swtich between line following and manual control.
bool drive_forward = false; // On-Off Drive Forward (To prevent drive forward coming off while turning)

void setup() {
  // Color Sensor
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(LEFT_OUT, INPUT);
  pinMode(RIGHT_OUT, INPUT);
  // S0-S3 Set in Setup because only looking at Clear Filter at 20%
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);
  digitalWrite(S2, HIGH);
  digitalWrite(S3, LOW);

  // LED Segment
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(L3, OUTPUT);
  pinMode(L4, OUTPUT);
  pinMode(L5, OUTPUT);
  pinMode(L6, OUTPUT);
  pinMode(L7, OUTPUT);
  digitalWrite(L1, HIGH);
  
  // IR Sensor
  irrecv.enableIRIn();

  // Servos
  left_motor.attach(LEFT_MOTOR_PIN);
  right_motor.attach(RIGHT_MOTOR_PIN);

  // Serial
  Serial.begin(9600);
}

/*
 * Turns on/off LEDs on the 7 Segment Display.
 */

void led_display_draw(bool l1=false, bool l2=false, bool l3=false, bool l4=false, bool l5=false, bool l6=false, bool l7=false){
  digitalWrite(L1, (l1) ? HIGH : LOW);
  digitalWrite(L2, (l2) ? HIGH : LOW);
  digitalWrite(L3, (l3) ? HIGH : LOW);
  digitalWrite(L4, (l4) ? HIGH : LOW);
  digitalWrite(L5, (l5) ? HIGH : LOW);
  digitalWrite(L6, (l6) ? HIGH : LOW);
  digitalWrite(L7, (l7) ? HIGH : LOW);
}

/*
 * Updates the 7 Segment Display with hardcoded LED drawings.
 */

void led_display_update(){
  if(timer >= 10000) {state ++; timer=0;}
  if(state>8) state = 0;

  // For Drawing Purpose - 1 : LEFT-TOP, 2: RIGHT-BOTTOM, 3: TOP-TOP, 4: LEFT-BOTTOM, 5: BOTTOM-BOTTOM, 6: TOP-RIGHT, 7: MIDDLE-MIDDLE

  switch(state){
    case 0:
      led_display_draw(true, true, true, true, false, true, true); // A
      break;
    case 1:
      led_display_draw(true, true, false, true, false, false, false); // L
      break;
    case 2:
      led_display_draw(true, false, true, true, true, false, true); // E
      break;
    case 3:
      led_display_draw(true, true, false, true, false, true, true); // X
      break;
    case 4:
      led_display_draw(); // EMPTY
      break;
    case 5:
      led_display_draw(true, true, false, true, true, true, false); // Smiley -> :)
      break;
    case 6:
      led_display_draw(); // Empty
      break;
    case 7:
      led_display_draw(true, true, false, true, true, true, false); // Smiley -> :)
      break;
    case 8:
      led_display_draw(); // Empty
      break;
    default:
      led_display_draw(true, true, true, true, true, true, true); // If there is a state error, display full segment.
      break;
  }
}

/*
 * Get controller inputs and performs functions using switch statment.
 */

void scan_controller() {
  if(irrecv.decode(&results)){
    irrecv.resume();

    Serial.print("Key Value: ");
    Serial.println(results.value, HEX);
    
    switch(results.value){
      case BUTTON_RIGHT:
        drive_and_stop(LEFT_MOTOR_SPEED, -RIGHT_MOTOR_SPEED, TURN_DELAY);
        break;
      case BUTTON_LEFT:
        drive_and_stop(-LEFT_MOTOR_SPEED, RIGHT_MOTOR_SPEED, TURN_DELAY);
        break;  
      case BUTTON_OK:
        drive_forward = false;
        drive(0, 0);
        break;
      case BUTTON_UP:
        drive_forward = true;
        break;
      case BUTTON_DOWN:
        drive_forward = false;
        drive(-LEFT_MOTOR_SPEED, -RIGHT_MOTOR_SPEED);
        break;
      case BUTTON_STAR:
        auto_pilot = true;
        drive_forward = false;
        drive(0,0);
        break;
      case BUTTON_POUND:
        auto_pilot = false;
        drive_forward = false;
        drive(0,0);
        break;
      case BUTTON_1:
        drive_and_stop(-LEFT_MOTOR_SPEED, RIGHT_MOTOR_SPEED, TURN_DELAY * 2);
        break;
      case BUTTON_2:
        break;
      case BUTTON_3:
        drive_and_stop(LEFT_MOTOR_SPEED, -RIGHT_MOTOR_SPEED, TURN_DELAY * 2);
        break;
    }
  }  
}

/*
 * Continuous Driving for Servos unless if Overwritten -> Forward/Backward Driving
 */

void drive(float left_motor_speed, float right_motor_speed){
  left_motor.write(LEFT_MOTOR_ZERO + left_motor_speed);
  right_motor.write(RIGHT_MOTOR_ZERO + right_motor_speed);
}

/*
 * Timed Driving for Servos -> Turning
 */

void drive_and_stop(float left_motor_speed, float right_motor_speed, int del){
  drive(left_motor_speed, right_motor_speed);
  delay(del);
  drive(0,0);  
}

/*
 * Line Following Algorithm.
 */

void line_follow(){
  int frequency = pulseIn(LEFT_OUT, LOW);
  int frequency2 = pulseIn(RIGHT_OUT, LOW);
  
  Serial.print("Frequency 1: ");
  Serial.println(frequency);
  Serial.print("Frequency 2 ");
  Serial.println(frequency2);

  // Checks if one color sensor sees brighter/darker values. If one sees a dark values, means they're closer to the line, thus turn the other side. (Maybe could look at the variation of both?)
  if(frequency > frequency2){
    drive(LEFT_MOTOR_SPEED / 2, RIGHT_MOTOR_SPEED);
  }else if(frequency < frequency2){
    drive(LEFT_MOTOR_SPEED, RIGHT_MOTOR_SPEED / 2); 
  }else{
    drive(LEFT_MOTOR_SPEED, RIGHT_MOTOR_SPEED);
  }

  delay(10);
}

void loop() {
  
  scan_controller();

  led_display_update();
  
  if(auto_pilot == true){
    timer+=1000; // Timer for display (considering the 10 second delay in the line_folow function).
    line_follow();
  }else{
    timer++; // Timer for display with no function running.
    if(drive_forward){
      drive(LEFT_MOTOR_SPEED, RIGHT_MOTOR_SPEED);
    }
  }
}
