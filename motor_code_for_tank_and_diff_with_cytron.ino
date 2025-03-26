//tank drive and differential steering w/ Cytron library
//By Carlos Carroz
//Date: 3/20/2025
//Version: 1
#include <CytronMotorDriver.h>

CytronMD front_left(PWM_DIR, 3, 2);    //format is (specific h-drive connections), PWM pin, DIR pin
CytronMD back_left(PWM_DIR, 5, 4);
CytronMD front_right(PWM_DIR, 6, 7);
CytronMD back_right(PWM_DIR, 9, 8);

bool tank_drive_mode = true;
int left_x;
int left_y;
int right_x;
int right_y;

void setup() {
  Serial.begin(115200); // Start serial communication (we need high data rate)
}

void tank_drive() {
  int left_pwm = map(left_y, min, max, -255, 255);     // Map joystick values (-min to max) to the PWM range (-255 to 255)
  int right_pwm = map(right_y, min, max, -255, 255);

  front_left.setSpeed(left_pwm);    // Set motors speeds
  back_left.setSpeed(left_pwm);
  front_right.setSpeed(right_pwm);
  back_right.setSpeed(right_pwm);
}

void differential_steering() {
  // Speed variables for both sides
  int left_speed = right_y;   // Speed for left motors (same for both)
  int right_speed = right_y;  // Speed for right motors (same for both)

  // Adjust speeds based on right_x (turning)
  if (right_x > 0) {
    right_speed *= 0.6;        // Move right motors slower when joystick is moved right
  } else if (right_x < 0) {
    left_speed *= 0.6;         // Move left motors slower when joystick is moved left
  }

  front_left.setSpeed(left_speed);
  back_left.setSpeed(left_speed);
  front_right.setSpeed(right_speed);
  back_right.setSpeed(right_speed);
}

void loop(){
  if (Serial.available() > 0) {       // Check if data is available (has to be send as four values separated by comma)
    left_x = Serial.parseInt();
    left_y = Serial.parseInt();       // Controls front_left and back_left
    right_x = Serial.parseInt();
    right_y = Serial.parseInt();      // Controls front_right and back_right
    button_a = Serial.parseInt();     // checks state of the button

    if (button_a == 1){
      tank_drive_mode = !tank_drive_mode;
    }
    if (tank_drive_mode){
      tank_drive();
    }else{
      differential_steering();
    }
  }
}