#include <AccelStepper.h>

// Define a stepper and the pins it will use
AccelStepper stepper(1,8,9);

int pos = 200;

void setup()
{  
  stepper.setMaxSpeed(300);
  stepper.setAcceleration(1000);
}

void loop()
{
 stepper.moveTo(500);  // set target forward 500 steps
  while (stepper.distanceToGo() != 0) 
  {  stepper.run(); }

  stepper.moveTo(0);  // set target back at start position
  while (stepper.distanceToGo() != 0) 
  {  stepper.run(); }

  stepper.moveTo(300); // set target forward 300 steps
  while (stepper.distanceToGo() != 0) 
  {  stepper.run(); }
}

