#include <Timers.h>
#include <Servo.h>

// Available timer(s)
enum Timer {
  LineCorrectionTimer // amount of time to spend turning in correction while following a line
};

// Indicates on-status/activity of  tape sensors in a single row
enum TapeActivity {
  tLeftAndCenter = 0,
  tCenterAndRight = 1,
  tLeftAndRight = 2,
  tCenter = 3,
  tLeft = 4,
  tRight = 5,
  tAll = 6,
  tNone = 7,
  tUndefined = 8
};

// MARK: - Pins
const int leftTape = 6;
const int rightTape = 7;
const int leftServoPin = 5;
const int rightServoPin = 3;
const int TURN_TIME = 50;


// MARK: - Variables
TapeActivity tapeSet = tUndefined;
Servo leftWheel;
Servo rightWheel;

void setup() {
  pinMode(leftTape, INPUT);
  pinMode(rightTape, INPUT);
  leftWheel.attach(leftServoPin); //analog pin 0
//  leftWheel.write(94); //Write the neutral position to that servo
  rightWheel.attach(rightServoPin); //analog pin 1
//  rightWheel.write(94);
  
  stop();
  Serial.begin(9600);

}

void loop() {
  
  // Read line sensor values, characterize the tapeSet
  ReadTapeSensors();
  Serial.print("Tape reading: ");
  Serial.println(tapeSet);
  
  // Line follow
  // forward();

}

// MARK: - Movement methods
void forward() {
  leftWheel.write(180);
  rightWheel.write(0);
}

void reverse() {
  leftWheel.write(0);
  rightWheel.write(180);
}

void right() {
  leftWheel.write(180);
  rightWheel.write(180);
}

void left() {
  leftWheel.write(0);
  rightWheel.write(0);
}

void stop() {
  leftWheel.write(95);
  rightWheel.write(95);
}

void ReadTapeSensors(){
  
  // collect individual tape readings
  char leftReading = !digitalRead(leftTape);
  char rightReading = !digitalRead(rightTape);
  
  // characterize tapeSet
  if(leftReading > 0 && rightReading > 0) tapeSet = tLeftAndRight;
  else if(leftReading > 0) tapeSet = tLeft;
  else if(rightReading > 0) tapeSet = tRight;
  else tapeSet = tNone;
}

// Line follow using two sensors. Initial state expected to be centered on line
void TwoSensorLineFollow(){
  static int lineFollowState = 0; // 0 - movingForward, 1 - sTurning
  
  if (lineFollowState == 0){ // in, or leaving MovingForward
    if(tapeSet == tLeft || tapeSet == tRight){ // line found
      lineFollowState = 1; // leave this state, and start turning
    } else { // otherwise, no line found
      forward(); // keep moving forward
    }
    
    // in or leaving, sTurning
  } else if (IsTimerExpired(LineCorrectionTimer)){ // timer expired, or not init'd
    if(tapeSet == tLeft){
      lineFollowState = 1;
      
      left(); // move left off of left tape sensor
      
      if(IsTimerExpired(LineCorrectionTimer)){ // start timer
        StartTimer(LineCorrectionTimer, TURN_TIME);
      }
    } else if(tapeSet == tRight){
      lineFollowState = 1;
      
      right(); // move slightly right
      
      if(IsTimerExpired(LineCorrectionTimer)){ // start timer
        StartTimer(LineCorrectionTimer, TURN_TIME);
      }
    } else {
      lineFollowState = 0;
    }
  }
}

// Start specified timer
void StartTimer(int timer, unsigned long time){
  TMRArd_InitTimer(timer, time);
}

// Return whether specified timer has expired or not
unsigned char IsTimerExpired(int timer){
  return (unsigned char)(TMRArd_IsTimerExpired(timer));
}

void CollectEnvInfo() {
  ReadTapeSensors();
}
