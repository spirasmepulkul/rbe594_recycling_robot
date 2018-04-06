
/*
   This code combines homing of the steppers with the temperature sensor reading
   Also, the difference between this code and homingAccelStepper and homingAccelStepper_Temp
   is that this code tries to combine the rbe594 code as well, to command it to a set location
   based on mm instead of just how many steps to go to. 
   NOTE: The difference between this code and V3 is that this one has servos (picking mechanism)
   and vacuum pump (suction) included. 
   We also have the option of taking out the serial input to try playing around with more 
   hard coded applications. We will then look into how to take in serial input from python code. 
   By Saraj (Jetro) Pirasmepulkul 2/19/18
*/
#include <Servo.h>
#include <Wire.h>
#include <SparkFunMLX90614.h>
#include "AccelStepper.h"

/*---------------------Thermometer Stuff------------------------*/

IRTherm therm; // Create an IRTherm object
const byte LED_PIN = 8;
float temperature;

/* ---------------------Stepper Motor Stuff ---------------------*/
//AccelStepper stepperX(1,2,3); //pin2 = step, pin3 = dir
AccelStepper stepperX(1, 8, 9); //pin 8 = step, pin 9 = dir
AccelStepper stepperY(1,10,11);

#define home_switch_x 2 //pin 2 is connected to home switch 
#define home_switch_y 3 //y axis home switch connected to pin 3

//stepper travel variables
long TravelX; //used to store the x value entered in the serial monitor
long TravelY; //used to store the y value entered in the serial monitor

int move_finished = 1; //to check if move is completed
long initial_homing = -1; //used to home stepper at startup

const int X_AXIS_MAX = 2000; //the total number of steps that the stepper can go (X)
const int Y_AXIS_MAX = 2000; //the total number of steps that the stepper can go (Y)

const float MM2STEP = 6.25; //need 300 steps to move 48 mm

const int heatChipDistance = 40*MM2STEP;//distance (in mm) between suction to heatgun TCP
const int dropOffChip = 245*MM2STEP; //distance (in mm) to move X axis to drop off the chip

/*---------------------End Effector Module Stuff-------------------*/

const int SERVO_PIN = 6;
const int VACUUM_PUMP =7; //initialize the pins
const int SERVO_HOME = 150; //position of servo home
const int SERVO_DOWN_DISTANCE = 30; //how far down the servo goes

Servo suctionServo; //initialize servo object for z axis picking up motion

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  //end effector module setup 
  suctionServo.attach(SERVO_PIN); //attach servo object
  pinMode(VACUUM_PUMP, OUTPUT); //initialize digital pin VACUUM_PUMP as output
  suctionServo.write(SERVO_HOME); //home the suction servo to nominal position (retracted)
  digitalWrite(VACUUM_PUMP, LOW); //turn vacuum off

  //stepper motor setup
  pinMode(home_switch_x, INPUT_PULLUP);
  pinMode(home_switch_y, INPUT_PULLUP);
  delay(5); //wait for driver to wake up

  //set maxspeed and acceleration of each stepper at startup for homing
  stepperX.setMaxSpeed(100.0);
  stepperX.setAcceleration(100.0);

  stepperY.setMaxSpeed(100.0);
  stepperY.setAcceleration(100.0);

  //start homing procedure of stepper motor at startup
  Serial.println("Stepper is homing....");

  //--------Homing X Axis Stepper------------------

  //make the stepper move CCW until the switch is activated
  while (digitalRead(home_switch_x)) {
    stepperX.moveTo(initial_homing); //set the position to move to
    initial_homing--;//decrease by 1 for next move if needed
    stepperX.run();
    delay(5);
  }
  stepperX.setCurrentPosition(0); //set the current position as zero for now
  stepperX.setMaxSpeed(100.0);
  stepperX.setAcceleration(100.0);
  initial_homing = 1;

  while (!digitalRead(home_switch_x)) { //make the stepper move CW until the switch is deactivated
    stepperX.moveTo(initial_homing);
    stepperX.run();
    initial_homing++;
    delay(5);
  }

  //--------Homing Y Axis Stepper------------------

  //make the stepper move CCW until the switch is activated
  while (digitalRead(home_switch_y)) {
    stepperY.moveTo(initial_homing); //set the position to move to
    initial_homing--;//decrease by 1 for next move if needed
    stepperY.run();
    delay(5);
  }
  stepperY.setCurrentPosition(0); //set the current position as zero for now
  stepperY.setMaxSpeed(100.0);
  stepperY.setAcceleration(100.0);
  initial_homing = 1;

  while (!digitalRead(home_switch_y)) { //make the stepper move CW until the switch is deactivated
    stepperY.moveTo(initial_homing);
    stepperY.run();
    initial_homing++;
    delay(5);
  }

/*** -----------Homing Complete --------------***/

  stepperX.setCurrentPosition(0);
  stepperY.setCurrentPosition(0);
  //Serial.println("Homing Completed");
  delay(1000);
  Serial.write("home"); //for python interface
  //Serial.println("");
  stepperX.setMaxSpeed(300.0); //set Max speed of stepper (for faster regular movements)
  stepperX.setAcceleration(500.0); //set Acceleration of stepper
  stepperY.setMaxSpeed(300.0); //set Max speed of stepper (for faster regular movements)
  stepperY.setAcceleration(500.0); //set Acceleration of stepper

  //print out instructions on the serial monitor at start
  //Serial.println("Enter travel distance (pos for cw/ neg foru ccw/ 0 for back to home): ");


  /*** Thermometer stuff ***/
  therm.begin();
  therm.setUnit(TEMP_C);
  pinMode(LED_PIN, OUTPUT); //led pin as output
  setLED(LOW); //LED OFF

  /*** Suction Gripper ***/


}

void loop() {
  // put your main code here, to run repeatedly:
  loopingCode();
  
  
  
  //pinMode(4,OUTPUT);
  //digitalWrite(4,HIGH);
}



// https://www.youtube.com/watch?v=YsLykxnHApg
