#include "Arduino.h"

//function responsible for end effector module's motion to pick up the chip
void pickupChip(){
  digitalWrite(VACUUM_PUMP, HIGH); //turn on suction!
  suctionServo.write(SERVO_DOWN_DISTANCE); //bring the suction gripper down to reach the chip
  delay(1500);                      //keep the suction on for 5s to ensure chip sticks
  suctionServo.write(SERVO_HOME); //bring the suction gripper back up to nominal position
  delay(1000);           //delay 5 more seconds to allow the chip to be transported
  
}

