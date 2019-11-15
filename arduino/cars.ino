#include "commands_cars.h"

//Arduino PWM Speed Control
int E1 = 5;
int M1 = 4;
int E2 = 6;
int M2 = 7;
int HighSpeed = 200;
int SlowSpeed = 200;
int TurnSpeed = 150;
int SlowTurnSpeed = 150;
int BigAngle = 150;
int SmallAngle = 150;
int BackHighSpeed = 150;
int BackSlowSpeed = 100;

void setup()
{
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  Serial.begin(115200);
}

void setSpeed(int n) //Keep beetween -255 and 255
{
  if (n < 0)
  {
    digitalWrite(M2, HIGH);
    n = - n;
  }
  else
    digitalWrite(M2, LOW);
  analogWrite(E2, n);
}

void setAngle(int n) //Keep beetween -255 (left) and 255 (right)
{
  if (n < 0)
  {
    digitalWrite(M1, LOW);
    n = - n;
  }
  else
    digitalWrite(M1, HIGH);
  analogWrite(E1, n);
}

void loop()
{
  if (Serial.available() > 0)
  {
    char input = Serial.read();

    if (input == DO_NOTHING)
    {
      setSpeed(0);
      setAngle(0);
    }
    else if (input == FORWARD)
    {
      setSpeed(HighSpeed);
      setAngle(0);
    }
    else if (input == LEFT)
    {
      setSpeed(TurnSpeed);
      setAngle(-BigAngle);
    }
    else if (input == RIGHT)
    {
      setSpeed(TurnSpeed);
      setAngle(BigAngle);
    }
    else if (input == LEFT_SLOW)
    {
      setSpeed(SlowTurnSpeed);
      setAngle(-SmallAngle);
    }
    else if (input == RIGHT_SLOW)
    {
      setSpeed(SlowTurnSpeed);
      setAngle(SmallAngle);
    }
    else if (input == FORWARD_SLOW)
    {
      setSpeed(SlowSpeed);
      setAngle(0);
    }
    else if (input == BACK)
    {
      setSpeed(-BackHighSpeed);
      setAngle(0);
    }
    else if (input == BACK_SLOW)
    {
      setSpeed(-BackSlowSpeed);
      setAngle(0);
    }
    else if (input == DUMP_VARS)
    {
      char buf [512];
      sprintf(buf, "HighSpeed: %.4d\r\n"
                   "SlowSpeed: %.4d\r\n"
                   "TurnSpeed: %.4d\r\n"
                   "SlowTurnSpeed: %.4d\r\n"
                   "BigAngle: %.4d\r\n"
                   "SmallAngle: %.4d\r\n"
                   "BackHighSpeed: %.4d\r\n"
                   "BackSlowSpeed: %.4d\r",
                   HighSpeed, SlowSpeed, TurnSpeed, SlowTurnSpeed,
                   BigAngle, SmallAngle, BackHighSpeed, BackSlowSpeed);
      Serial.println(buf);
    }

    switch (input & 0x1f) {
    case HIGH_SPEED:
      update_var_from_command(&HighSpeed, input);
      break;
    case SLOW_SPEED:
      update_var_from_command(&SlowSpeed, input);
      break;
    case TURN_SPEED:
      update_var_from_command(&TurnSpeed, input);
      break;
    case SLOW_TURN_SPEED:
      update_var_from_command(&SlowTurnSpeed, input);
      break;
    case BIG_ANGLE:
      update_var_from_command(&BigAngle, input);
      break;
    case SMALL_ANGLE:
      update_var_from_command(&SmallAngle, input);
      break;
    case BACK_HIGH_SPEED:
      update_var_from_command(&BackHighSpeed, input);
      break;
    case BACK_SLOW_SPEED:
      update_var_from_command(&BackSlowSpeed, input);
      break;
    default:
      break;
    }
  }
}
