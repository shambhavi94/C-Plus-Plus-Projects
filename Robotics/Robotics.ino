#include <Sparki.h> // include the sparki library

double overallX = 0;
double overallY = 0;
double overallTheta = 0;

double x  = 0;
double y = 0;
double theta = 0;

double lengthOfWheelBase = 7.5;
double radiusOfWheel = 2.5;

bool straight = false; 
bool turnLeft = false;
bool turnRight = false; 

void followLine()
{
 int threshold = 500;
 
  int lineLeft   = sparki.lineLeft();   // measure the left IR sensor
  int lineCenter = sparki.lineCenter(); // measure the center IR sensor
  int lineRight  = sparki.lineRight();  // measure the right IR sensor
 
  if ( lineLeft < threshold ) // if line is below left line sensor
  {  
    sparki.moveLeft(); // turn left
    turnLeft = true;
    turnRight = false;
    straight = false;
  }
 
  if ( lineRight < threshold ) // if line is below right line sensor
  {  
    sparki.moveRight(); // turn right
    turnRight = true;
    turnLeft = false;
    straight = false;
  }
 
  // if the center line sensor is the only one reading a line
  if ( (lineCenter < threshold) && (lineLeft > threshold) && (lineRight > threshold) )
  {
    sparki.moveForward(); // move forward
    straight = true;
    turnLeft = false;
    turnRight = false;
  }  
  //if we are at the start line
  if ( lineCenter < threshold && lineLeft < threshold && lineRight < threshold)
  {
    sparki.moveForward(1);
    overallX = 0;
    overallY = 0;
    overallTheta = 0;

    x = 0;
    y = 0;
    theta = 0;
  }
  return;
}

void calculateSpeed()
{
  int start = millis();
  sparki.moveForward(30);
  int end = millis();
  int timetoMove30 = end - start;
  double speed = 0.3 / (timetoMove30/ 1000 );
  //turns out to be 0.03 m/s
  // or 0.003 cm/ms 
  // 0.3 cm / 100 ms
}

void calcRotationalSpeed()
{
  int start = millis();
  sparki.moveRight(180);
  int end = millis();
  int timeToRotate180 = end - start;
  double rotationalSpeed = 180 / (timeToRotate180 / 1000);
  //turns out to be 45 degrees/second
  // or 4.5 degrees / ms
}
 
void setup() 
{
  sparki.clearLCD();
  sparki.updateLCD();
}
 
void loop() {
  int loopStartTime = millis();

  double deltaLeft;
  double deltaRight;
    
  followLine();

  //for each 100 ms (each time we run this loop), measure the distance each wheel has traveled since last loop
  // if robot is going straight, both wheels will just be 0.003 (cm/ms)
  // we know this from calculateSpeed();
  if (straight) 
  {
      deltaLeft = 0.3;
      deltaRight = 0.3;
  }
   // if robot is turning, each wheel depends upon the speed of turn (45 degrees/second) and direction it is turning
   // we know that it turns 4.5 degrees/100 ms based on calcRotationalSpeed()
   // length of arc = radius * angle 
  else if (turnRight) 
  {
        deltaLeft = lengthOfWheelBase *  4.5;
        deltaRight = 0;
  }
  else if (turnLeft)
  {
        deltaLeft = 0;
        deltaRight = lengthOfWheelBase * 4.5;
  }
  
  //then we use this change in distance, as well as current angle/orientation of robot to get new position 
  //if the robot is going straight, we do not need to worry about any fancy equations, it will just add the change
  // of distance and multiply it by whatever angle we are at
   if (straight)
   {
      overallX = x + deltaLeft * cos(theta);
      overallY = y + deltaRight * sin(theta);
      overallTheta = theta;                         //Since robot is straight, keep the same angle 
   }
   else
   {
      double R = lengthOfWheelBase * (deltaLeft + deltaRight) / (2 * (deltaRight - deltaLeft));
      double wd = (deltaRight - deltaLeft) / lengthOfWheelBase;
  
      overallX = x + R * sin(wd + theta) - R * sin(theta);
      overallY = y - R * cos(wd + theta) + R * cos(theta);
      overallTheta = theta + wd;                        //update angle, since robot is now turning
  }
  
  //update variables 
   x  = overallX; 
   y = overallY; 
   theta = overallTheta; 

  //print positions to screen
  sparki.clearLCD(); 
      
  sparki.print("X: "); // show left line sensor on screen
  sparki.println(overallX);
 
  sparki.print("Y: "); // show center line sensor on screen
  sparki.println(overallY);
 
  sparki.print("theta: "); // show right line sensor on screen
  sparki.println(overallTheta);

  if (straight){
    sparki.print("straight ");
  }
 
  sparki.updateLCD(); // display all of the information written to the screen
  
  int loopEndTime = millis();
  
  //if the end of the loop time has not yet reached 100 ms, wait the difference
  if ( loopEndTime - loopStartTime < 100 ) {
      delay(100 - (loopEndTime -loopStartTime)); 
  }
}

