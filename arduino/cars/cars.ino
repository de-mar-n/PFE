#include <Servo.h>

Servo mot;
int pulse = 1500;

void setup()
{
  //mot.attach(2);//, 1000, 2000);
  pinMode(6, OUTPUT);
  delay(15);
  //digitalWrite(6, HIGH);
  analogWrite(6, 255);
  delay(2000);
  analogWrite(6, 0);
}

void setAngle(int n)
{
  mot.write(n);
}

void testAngle() {
  for (int i = 0; i < 90; i+=10) {
    setAngle(i);
    delay(1000);
  }
}

void loop()
{
  //delay(3000);
  //testAngle();
  //digitalWrite(6, HIGH);
  //delay(1500);
  //for (int i = 0; i <= 255; i++)
  //{
  //  delay(pulse);
  //  digitalWrite(6, LOW);
  //  delay(pulse);
  //}
}
