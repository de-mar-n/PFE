#include <Servo.h>

Servo mot;     // create servo object to control the motor
//int potValue;  // value from the analog pin
Servo dir;  // create servo for direction (trun left/right)

int HighSpeed = 150;
int SlowSpeed = 110;
int TurnSpeed = 110;
int SlowTurnSpeed = 105;
int BigAngle = 50;
int SmallAngle = 20;
int BackHighSpeed = 40;
int BackSlowSpeed = 70;


void setSpeed(int n) //Keep beetween 0 and 180
// n > 90 go ahead, n < 90 go backward, n = 90 neutral
{
    mot.write(n);
}

void setAngle(int n) //Keep beetween 0 (right) and 180 (left)
{
  dir.write(n);
}


void setup() {
  // Attach the motor on pin 9
  mot.attach(9,1000,2000); // (pin, min pulse width, max pulse width in microseconds) 
  dir.attach(6, 1000, 2000);
  delay(1500);
}
void loop() {
  //potValue = analogRead(A0);   // reads the value of the potentiometer (value between 0 and 1023)
  //potValue = map(potValue, 0, 1023, 0, 180);   // scale it to use it with the servo library (value between 0 and 180)
  //potValue = map(potValue, 0, 1023, 0, 180);   // scale it to use it with the servo library (value between 0 and 180)
  //mot.write(potValue);    // Send the signal to the ESC
  //Serial.print(potValue);
  setSpeed(70);
  delay(2000);
  setAngle(110);
  delay(2000);
  setSpeed(90);
  delay(2000);
  setAngle(40);
  delay(2000);
 
 
}
