
/*This is the code responsible for moving the heat gun 
 * directly on top of desired chip by moving x axis forward
 * a fixed amount of distance
 */
#include "Arduino.h"

void heatChip(){
      stepperX.move(22*MM2STEP);
      while (stepperX.distanceToGo() != 0)
      { stepperX.setSpeed(250);stepperX.runSpeed();}

      delay(2000); //increases this time to heat the chip for longer

      stepperX.moveTo(TravelX);
      while (stepperX.distanceToGo() != 0)
      { stepperX.setSpeed(-250);stepperX.runSpeed();
}

