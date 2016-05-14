#include <Servo.h>

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


// MARK: - Variables
TapeActivity tapeSet = tUndefined;

void setup() {
  pinMode(leftTape, INPUT);
  pinMode(rightTape, INPUT);
  
  Serial.begin(9600);

}

void loop() {
  ReadTapeSensors();
  Serial.print("Tape reading: ");
  Serial.println(tapeSet);

}

void ReadTapeSensors(){
  
  // collect individual tape readings
  char leftReading = !digitalRead(leftTape);
  char rightReading = !digitalRead(rightTape);
  
  /* (Optional) Readings for far left and right readings 
  char farLeftReading = !digitalRead(farLeftTape);
  char farRightReading = !digitalRead(farRightTape);
  
  // summarize outside tape set
  if(middleFarLeft > 0 && middleFarRight > 0) outsideTapeSet = tLeftAndRight; // left and right 
  else if(middleFarLeft > 0) outsideTapeSet = tLeft;
  else if(middleFarRight > 0) outsideTapeSet = tRight;
  else outsideTapeSet = tNone;
  */
  
  // summarize center tape set
  /*
  if(centerReading > 0 && leftReading > 0 && rightReading > 0) tapeSet = tAll;
  else if(leftReading > 0 && centerReading > 0) tapeSet = tLeftAndCenter;
  else if(centerReading > 0 && rightReading > 0) tapeSet = tCenterAndRight;
  else */
  
  if(leftReading > 0 && rightReading > 0) tapeSet = tLeftAndRight;
  else if(leftReading > 0) tapeSet = tLeft;
  else if(rightReading > 0) tapeSet = tRight;
  // else if(centerReading > 0) tapeSet = tCenter;
  else tapeSet = tNone;
}

void CollectEnvInfo() {
  ReadTapeSensors();
}
