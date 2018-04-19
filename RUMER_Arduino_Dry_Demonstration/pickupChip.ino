
/*
 * Function responsible for end effector module's motion to pick up the chip
 */
#include "Arduino.h"

void pickupChip(){
  digitalWrite(VACUUM_PUMP, HIGH); //turn on suction!
  suctionServo.write(SERVO_DOWN_DISTANCE); //bring the suction gripper down to reach the chip
  delay(1500);                      //keep the suction on for 1.5s to ensure chip sticks without cooling solder
  suctionServo.write(SERVO_HOME); //bring the suction gripper back up to nominal position
  
}
