#include "Arduino.h"

//this is the code responsible for moving the heat gun 
//directly on top of desired chip by moving x axis forward
//a fixed amount of distance

void heatChip(){
  //stepperX.moveTo(TravelX + 100); //move the heatgun to position over the chip
  //delay(1000); //30 seconds for chip to heat up
  //stepperX.moveTo(TravelX-100); //move the gripper back over the chip

      stepperX.move(22*MM2STEP);
      while (stepperX.distanceToGo() != 0)
      { stepperX.setSpeed(250);stepperX.runSpeed();}

      delay(100000);

      stepperX.moveTo(TravelX);
      while (stepperX.distanceToGo() != 0)
      { stepperX.setSpeed(-250);stepperX.runSpeed();}

  
}

