// # Editor     : ZRH from DFRobot
// # Date       : 29.08.2014

// # Product name: URM V4.0 ultrasonic sensor
// # Product SKU : SEN0001
// # Version     : 1.0

// # Description:
// # The Sketch for scanning 180 degree area 3-500cm detecting range
// # The sketch for using the URM37 PWM trigger pin mode from DFRobot
// #   and writes the values to the serialport
// # Connection:
// #       Vcc (Arduino)    -> Pin 1 VCC (URM V4.0)
// #       GND (Arduino)    -> Pin 2 GND (URM V4.0)
// #       Pin 3 (Arduino)  -> Pin 4 ECHO (URM V4.0)
// #       Pin 5 (Arduino)  -> Pin 6 COMP/TRIG (URM V4.0)
// #       Pin A0 (Arduino) -> Pin 7 DAC (URM V4.0)
// # Working Mode: PWM trigger pin  mode.

// ---------------- FOR MOVING THE CAR -------------
#include <Servo.h>

#define LIMIT_DISTANCE 10
Servo mot;     // create servo object to control the motor
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


// ------------- FOR THE ULTRASON ------------
#define  Measure  1     //Mode select
int URECHO = 3;         // PWM Output 0-25000US,Every 50US represent 1cm
int URTRIG = 5;         // PWM trigger pin
int sensorPin = A0;     // select the input pin for the potentiometer
int sensorValue = 0;    // variable to store the value coming from the sensor

unsigned int DistanceMeasured= 0;

void PWM_Mode()                              // a low pull on pin COMP/TRIG  triggering a sensor reading
{
  Serial.print("Distance Measured=");
  digitalWrite(URTRIG, LOW);
  digitalWrite(URTRIG, HIGH);               // reading Pin PWM will output pulses
  if( Measure)
  {
    unsigned long LowLevelTime = pulseIn(URECHO, LOW) ;
    if(LowLevelTime>=45000)                 // the reading is invalid.
    {
      Serial.print("Invalid");
    }
    else{
    DistanceMeasured = LowLevelTime /50;   // every 50us low level stands for 1cm
    Serial.print(DistanceMeasured);
    Serial.println("cm");
  }

  }
  else {
    sensorValue = analogRead(sensorPin);
    if(sensorValue<=10)                   // the reading is invalid.
    {
      Serial.print("Invalid");
    }
    else {
    sensorValue = sensorValue*0.718;
    Serial.print(sensorValue);
    Serial.println("cm");
    }
  }
}


// -------------- SETUP AND LOOP ------------
void setup()
{
  // ------- FOR ULTRASON -------
  //Serial initialization
  Serial.begin(9600);                        // Sets the baud rate to 9600
  pinMode(URTRIG,OUTPUT);                    // A low pull on pin COMP/TRIG
  digitalWrite(URTRIG,HIGH);                 // Set to HIGH
  pinMode(URECHO, INPUT);                    // Sending Enable PWM mode command
  delay(500);
  Serial.println("Init the sensor");

  // ------- FOR MOVE -------
  // Attach the motor on pin 9
  mot.attach(9,1000,2000); // (pin, min pulse width, max pulse width in microseconds) 
  dir.attach(6, 1000, 2000);
  delay(1500);


 }

void loop()
{
  // ------ For ultrason ------
  PWM_Mode();
  delay(100);

  // ------ For moving the car ------
  if (DistanceMeasured > LIMIT_DISTANCE || sensorValue > LIMIT_DISTANCE)
  {
    setSpeed(107);
    delay(800);
  }
  else
  {
    setSpeed(75); // Go backward
    delay(800);
    setAngle(40);
    delay(2000);
  }
}
