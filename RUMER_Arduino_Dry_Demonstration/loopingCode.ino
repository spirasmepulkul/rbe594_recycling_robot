#include "Arduino.h"

void loopingCode() {


  /*** Stepper Motor Movement Code (Post-Homing)***/
  while (Serial.available() > 0) { //check if values are available in the serial buffer
    move_finished = 0;

    String serialInput = Serial.readString(); //read in the string from serial (x,y) and call it serialInput
    int commaIndex = serialInput.indexOf(','); //the index of the comma, should be after first number
    String firstNum = serialInput.substring(0, commaIndex);
    String secondNum = serialInput.substring(commaIndex + 1);

    TravelX = firstNum.toInt() * MM2STEP; //cast the string that is the number into an int to use
    TravelY = secondNum.toInt() * MM2STEP; //and multiply it by MM2STEP to get the number of steps

    //--------------------X and Y Axis--------------------------------
    if ((TravelX < 0 || TravelX > X_AXIS_MAX) || (TravelY < 0 || TravelY > Y_AXIS_MAX)) { // Make sure the position entered is not beyond the HOME or MAX position
      Serial.println("");
      Serial.println("Please enter a value between 0 and " + String(X_AXIS_MAX / 10) + "...");
      Serial.println("");
    }

    else {
      //move in x direction
      stepperX.moveTo(TravelX); //set the new moveto position of stepper
      //move in y direction
      stepperY.moveTo(TravelY);
      delay(1000);
    }
  }

  if ((TravelX >= 0 && TravelX <= X_AXIS_MAX) && (TravelY >= 0 && TravelY <= Y_AXIS_MAX))  {
    //check if the stepper has reached desired position
    if ((stepperX.distanceToGo() != 0)) {
      stepperX.run();  //Move Stepper into position
    }
    if ((stepperY.distanceToGo() != 0)) {
      stepperY.run(); //move y axis stepper into position
    }

    /*** -----------------------Thermometer takes in object temp reading here -----------------***/
    //Call therm.read() to read object and ambient temperatures from the sensor
    //if (therm.read()) //on success, read() will return 1, on fail 0.
    //{
    //  temperature = (therm.object());
    // }

    /*** --------------------- End Thermometer/Begin Stepper Complete Movement ----------------------***/

    // If move is completed display message on Serial Monitor
    if ((move_finished == 0) && (stepperX.distanceToGo() == 0) && (stepperY.distanceToGo() == 0)) {

      move_finished = 1; // Reset move variable

      /***---------------------Position the heatgun TCP over the Chip-------------***/

      delay(500);

      heatChip();

      pickupChip(); //pick up the chip

      dropOffChip();
    }
  }
}
