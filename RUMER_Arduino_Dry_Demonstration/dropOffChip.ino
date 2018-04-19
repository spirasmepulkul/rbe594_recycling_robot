/*
 * move to the end to drop off the chip by turning off the pump
 * and coming back to where that chip was. 
*/

#include "Arduino.h"

void dropOffChip(){

      stepperX.moveTo(DROPOFFDISTANCE);
      while (stepperX.distanceToGo() != 0)
      { stepperX.setSpeed(250);stepperX.runSpeed();}
      digitalWrite(VACUUM_PUMP, LOW);

      delay(1000);

      stepperX.moveTo(TravelX);
      while (stepperX.distanceToGo() != 0)
      { stepperX.setSpeed(-250);stepperX.runSpeed();}
}

