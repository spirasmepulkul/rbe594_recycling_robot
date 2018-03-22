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

    //when user enters in travel distance in mm

    //--------------------X and Y Axis--------------------------------
    if ((TravelX < 0 || TravelX > X_AXIS_MAX) || (TravelY < 0 || TravelY > Y_AXIS_MAX)) { // Make sure the position entered is not beyond the HOME or MAX position
      Serial.println("");
      Serial.println("Please enteur a value between 0 and " + String(X_AXIS_MAX / 10) + "...");
      Serial.println("");
    }

    else {
      //Serial.println("Moving stepper to (x,y) position in mm: " + firstNum + "," + secondNum);
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
    if (therm.read()) //on success, read() will return 1, on fail 0.
    {
      temperature = (therm.object());
      //use the object() and ambient() functions to grab the object and ambient temp
      //they are floats, calculated out ot the unit you set with setUnit()
      //Serial.println("Object in celsius: " + String(therm.object(), 2));
      //Serial.println("Ambient in celsius: " + String(therm.ambient(),2));
    }

    /*----------------------------this is where it does the heat chip movements------------------*/

    //stepperX.moveTo(TravelX + heatChipMovement); //move the heatgun to position over the chip
    //delay(2000); // 2 seconds for chip to heat up
    //stepperX.moveTo(TravelY); //move the gripper back over the chip

    /*** --------------------- End Thermometer/Begin Stepper Complete Movement ----------------------***/

    // If move is completed display message on Serial Monitor
    if ((move_finished == 0) && (stepperX.distanceToGo() == 0) && (stepperY.distanceToGo() == 0)) {
      //Serial.println("COMPLETED!");
       //Serial.println("Object temperature in celsius: " + String(temperature));
      //stepperX.moveTo(TravelX + heatChipMovement); //move the heatgun to position over the chip
      //delay(2000); // 2 seconds for chip to heat up
      //stepperX.moveTo(TravelX); //move the gripper back over the chip
      // Serial.println("");
      //Serial.println("Enter travel distance in mm or 0,0 to return to HOME): ");
      move_finished = 1; // Reset move variable
            
    /***---------------------Position the heatgun TCP over the Chip-------------***/ 
      stepperX.moveTo(TravelX+heatChipDistance);
      while (stepperX.distanceToGo() != 0)
      {stepperX.run();}

      delay(3000); //simulate the time it takes to heat up the chip
            
      stepperX.moveTo(TravelX);
      while (stepperX.distanceToGo() != 0)
      {stepperX.run();}
      

      pickupChip(); //pick up the chip

      
      //move to the end to drop off the chip
      stepperX.moveTo(dropOffChip);
      while (stepperX.distanceToGo() != 0)
      {stepperX.run();}
     }
    //moving the motor back and forth for the heatgun

  }



}
