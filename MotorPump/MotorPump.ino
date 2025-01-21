#include <AccelStepper.h>
#include <ArduinoLog.h>

// Pins for stepper motor 1
const int stepPin1 = 8;
const int dirPin1 = 9;
const int enablePin1 = 10;
// Pins for stepper motor 2
const int stepPin2 = 11;
const int dirPin2 = 12;
const int enablePin2 = 13;
// Limit switch pins
const int limitSwitchPin1 = 2; // Limit switch for stepper 1
const int limitSwitchPin2 = 3; // Limit switch for stepper 2
// Create AccelStepper objects
AccelStepper stepper1(1, stepPin1, dirPin1);
AccelStepper stepper2(1, stepPin2, dirPin2);

void setup() {
  Serial.begin(9600);
  // Initialize logger with Serial output
  Log.begin(LOG_LEVEL_VERBOSE, &Serial);
  Log.notice("System Initialized\n");
  
  // LOG_LEVEL_SILENT
  // LOG_LEVEL_FATAL
  // LOG_LEVEL_ERROR
  // LOG_LEVEL_WARNING
  // LOG_LEVEL_NOTICE
  // LOG_LEVEL_TRACE
  // LOG_LEVEL_VERBOSE

  // Initialize stepper motors
  pinMode(enablePin1, OUTPUT);
  digitalWrite(enablePin1, LOW); // Enable stepper 1
  pinMode(enablePin2, OUTPUT);
  digitalWrite(enablePin2, LOW); // Enable stepper 2
  // Initialize limit switch pins
  pinMode(limitSwitchPin1, INPUT_PULLUP); // Use INPUT_PULLUP to detect LOW when pressed
  pinMode(limitSwitchPin2, INPUT_PULLUP);
  // Set max speed and acceleration
  stepper1.setMaxSpeed(6000);
  stepper1.setAcceleration(1000);
  stepper2.setMaxSpeed(6000);
  stepper2.setAcceleration(1000);
  // Move motors to find zero position
  findZeroPosition(stepper1, limitSwitchPin1);
  findZeroPosition(stepper2, limitSwitchPin2);
  Log.notice("Set Zero Complete!\n");
}

void loop() {
  // stepper1.moveTo(steps);
  // while (stepper.distanceToGo() != 0) {
  //   stepper1.run();
  // }
}

void findZeroPosition(AccelStepper& stepper, int limitSwitchPin) {
  // Move the motor backward to find the limit switch
  stepper.setSpeed(-100); // Set a slow speed for homing
  while (digitalRead(limitSwitchPin) == HIGH) { // Wait until the switch is pressed
    stepper.runSpeed();
  }
  // Stop the motor and set the current position as zero
  stepper.stop();
  stepper.setCurrentPosition(0);
  Log.notice("Homing complete for stepper motor.");
}

int cmtostep(float cm, int speed, int acceleration, AccelStepper& stepper){
 // Configuration for stepper1
  const float stepsPerRevolution1 = 205.0;
  const float distancePerRevolution1 = 1.0;
  // Configuration for stepper2
  const float stepsPerRevolution2 = 515.8;
  const float distancePerRevolution2 = 1.0;
  long stepsToMove = 0; // Initialize the variable to hold the calculated steps
  
  // Check which stepper is being used
  if (&stepper == &stepper1) { // Compare addresses
    stepsToMove = cm * (stepsPerRevolution1 / distancePerRevolution1);
  } else if (&stepper == &stepper2) {
    stepsToMove = cm * (stepsPerRevolution2 / distancePerRevolution2);
  }
  // Set speed and acceleration for the stepper
  stepper.setMaxSpeed(speed);
  stepper.setAcceleration(acceleration);
  Log.notice("Step to Move: %d\n", stepsToMove);
  return stepsToMove;
}

void sentSerialMessage(String msg){
  // Send a message to the laptop when the stepper reaches the target
  Serial.println(msg);
  Serial.flush(); // Ensure the data is fully sent
  Log.notice("Already Sent the MSG : %s\n", msg);
}