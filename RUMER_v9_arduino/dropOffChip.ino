#include "Arduino.h"

void dropOffChip(){
        //move to the end to drop off the chip
      stepperX.moveTo(DROPOFFDISTANCE);
      while (stepperX.distanceToGo() != 0)
      { stepperX.setSpeed(250);stepperX.runSpeed();}

      delay(2000);

      stepperX.moveTo(DROPOFFDISTANCE/2);
      while (stepperX.distanceToGo() != 0)
      { stepperX.setSpeed(-250);stepperX.runSpeed();}
}

