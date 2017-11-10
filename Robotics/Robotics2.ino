#include <Sparki.h> // include the sparki library

int state = 1;

void gripObject()
{
  sparki.clearLCD();
  sparki.println("State: Grabbing Object"); // print current state
  sparki.updateLCD();
  sparki.moveStop();
  sparki.gripperClose();
  delay(3000);
  sparki.gripperStop();
  sparki.moveLeft(180); // turn around
  state = 3; // Find the line
}

void releaseObject()
{
  sparki.clearLCD();
  sparki.println("State: Releasing Object"); // print current state
  sparki.updateLCD();
  sparki.moveStop();
  sparki.gripperOpen();
  delay(3000);
  sparki.gripperStop();
  sparki.RGB(RGB_GREEN);
  sparki.moveBackward(15);
  sparki.moveLeft(90);
  sparki.moveForward(10);
  state = 1; // Go back to finding object
}

void moveLeft() // helpful to pivot about the left wheel CREDIT: http://arcbotics.com/lessons/object-retrieval/
{
  sparki.motorRotate(MOTOR_LEFT, DIR_CCW, 0);
  sparki.motorRotate(MOTOR_RIGHT, DIR_CW, 100);
}

void moveRight() // helpful to pivot about the right wheel CREDIT: http://arcbotics.com/lessons/object-retrieval/
{
  sparki.motorRotate(MOTOR_LEFT, DIR_CCW, 100);
  sparki.motorRotate(MOTOR_RIGHT, DIR_CW, 0);
}

void findObject()
{
  sparki.clearLCD();
  sparki.println("State: Finding Object"); // print current state
  sparki.updateLCD();
  int cm = sparki.ping(); // determine distance from sparki to what is in front of sparki
  if (cm < 20) // if distance between sparki and current obstruction is less than 20 centimeters..
  {
    sparki.RGB(RGB_RED);
    sparki.beep();
    sparki.moveForward(cm); // move to it
    sparki.RGB(RGB_GREEN);
    state = 2; // Grab the object
  }
  sparki.moveRight(25);
}

void findLine()
{
  sparki.RGB(RGB_WHITE); // turn light white to indicate we are finding a line
  sparki.clearLCD();
  sparki.println("State: Finding Line"); // print current state
  sparki.updateLCD();
  if (sparki.lineCenter() < 500) // if the center line sensor indicates surface is black
  {
    sparki.moveLeft(25); // turn a little bit to avoid starting parallel to the track
    state = 4; // Find the start
    loop();
  }
  sparki.moveForward();
  delay(100);
}

void goToStart()
{
  sparki.RGB(RGB_BLUE); // turn light blue to indicate we are searching for the start
  sparki.clearLCD();
  sparki.print("Left edge:  ");
  sparki.println(sparki.edgeLeft());
  sparki.print("Left line:  ");
  sparki.println(sparki.lineLeft());
  sparki.print("Center line:  ");
  sparki.println(sparki.lineCenter());
  sparki.print("Right line:  ");
  sparki.println(sparki.lineRight());
  sparki.print("Right edge:  ");
  sparki.println(sparki.edgeRight());
  sparki.println("State: Finding Home"); // print current state (all above prints dispaly current sensor info)
  sparki.updateLCD();
  if (sparki.lineLeft() < 500) // if surface below left sensor is black
  {
    moveLeft(); // turn about left wheel
  }
  else if (sparki.lineRight() < 500) // if surface below right sensor is black
  {
    moveRight(); // turn about right wheel
  }
  else if (sparki.lineLeft() > 700 && sparki.lineRight() > 700 && sparki.lineCenter() < 500) // if lift/right sensors are reading white and center is reading black
  {
    sparki.moveForward(); // move forward
  }
  else if (sparki.lineLeft() > 700 && sparki.lineRight() > 700 && sparki.lineCenter() > 700) // if left, right, and center sensors are reading white, find the line again
  {
    state = 3; // search for the line again
  }
  if (sparki.lineCenter() < 100 && sparki.lineLeft() < 100 && sparki.lineRight() < 100 && (sparki.edgeLeft() > 500 || sparki.edgeRight() > 500)) // if left, right, and center sensors are reading black, as well as one of the edge sensors
  {
    state = 5; // let go of the object
  }
}

void setup()
{
  sparki.servo(SERVO_CENTER); // rotate the servo to is 0 degree postion (forward)
  sparki.RGB(RGB_OFF); // turn off LCD to start
  sparki.gripperOpen();
  delay(5000);
  sparki.gripperStop();
}

void loop()
{
  switch (state)
  {
    case 1:
      findObject();
      break;
    case 2:
      gripObject();
      break;
    case 3:
      findLine();
      break;
    case 4:
      goToStart();
      break;
    case 5:
      releaseObject();
      break;
  }
}

