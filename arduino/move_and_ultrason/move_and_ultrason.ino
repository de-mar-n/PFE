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

#define LIMIT_DISTANCE 15
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

#define ERROR_VALUE -1
#define NB_SONAR 4
// PWM Output 0-25000US,Every 50US represent 1cm

const int URECHO1 = 13; //Ultrason du droite (quand on regarde la voiture en face)
const int URTRIG1 = 10;
const int URECHO2 = 11; // Ultrason de milieu
const int URTRIG2 = 7;
const int URECHO3 = 3; // gauche
const int URTRIG3 = 9;
const int URECHO4 = 12; // Ultrason arriere
const int URTRIG4 = 8;

const int URECHO_array[] = {URECHO1, URECHO2, URECHO3, URECHO4};

int URTRIG = 5;         // PWM trigger pin


const int URTRIG_array[] = {URTRIG1, URTRIG2, URTRIG3, URTRIG4};


// variable to store the value coming from the sensor
int sensorValue = 0;

int sonar_values[NB_SONAR] = {0,0,0,0};



unsigned int DistanceMeasured= 0;

void printCapteur(int n)
{
  switch (n)
  {
    case URECHO1:
      Serial.print("Droite");
      break;
    case URECHO2:
      Serial.print("Avant");
      break;
    case URECHO3:
      Serial.print("Gauche");
      break;
    case URECHO4:
      Serial.print("Arrière");
      break;
    default:
      break;
  }
}

int PWM_Mode(int ECHO)                              // a low pull on pin COMP/TRIG  triggering a sensor reading
{
  //Serial.print("Distance Measured=");
  digitalWrite(URTRIG, LOW);
  digitalWrite(URTRIG, HIGH);               // reading Pin PWM will output pulses

  unsigned long LowLevelTime = pulseIn(ECHO, LOW) ;
  if(LowLevelTime>=45000)                 // the reading is invalid.
  {
    printCapteur(ECHO);
    Serial.println (" Invalid");
    return ERROR_VALUE;
  }
  else{
    DistanceMeasured = LowLevelTime /50;   // every 50us low level stands for 1cm
    Serial.print("Capteur ");
    //Serial.println(ECHO);
    printCapteur(ECHO);
    Serial.print(" ");
    Serial.print(DistanceMeasured);
    Serial.println("cm");
    return DistanceMeasured;
  }
}


// -------------- SETUP AND LOOP ------------
void setup()
{
  // ------- FOR ULTRASON -------
  //Serial initialization
  Serial.begin(9600);                        // Sets the baud rate to 9600
  for (int i = 0; i < NB_SONAR; i++)
  {
    pinMode(URTRIG_array[i],OUTPUT);                    // A low pull on pin COMP/TRIG
    digitalWrite(URTRIG_array[i],HIGH);                 // Set to HIGH
  }
  //pinMode(URECHO1, INPUT);                    // Sending Enable PWM mode command
  delay(500);
  Serial.println("Init the sensor");

  // ------- FOR MOVE -------
  // Attach the motor on pin 9
  mot.attach(5,1000,2000); // (pin, min pulse width, max pulse width in microseconds)
  dir.attach(6, 1000, 2000);
  delay(1500);
  }

// Return an array with the values of ultrasons
void get_ultrason_values()
{

              // reading Pin PWM will output pulses

  unsigned long LowLevelTimes[NB_SONAR];

  for (unsigned int i = 0; i < NB_SONAR; ++i)
  {
    digitalWrite(URTRIG_array[i], LOW);
    digitalWrite(URTRIG_array[i], HIGH); 
    LowLevelTimes[i] = pulseIn(URECHO_array[i], LOW) ;
  }
  for (unsigned int i = 0; i < NB_SONAR; ++i)
  {
    unsigned long LowLevelTime = LowLevelTimes[i];
    int ECHO = URECHO_array[i];
    if(LowLevelTime>=45000)                 // the reading is invalid.
    {
      /*printCapteur(ECHO);
      Serial.println (" Invalid");*/
      sonar_values[i] =  ERROR_VALUE;
    }
    else{
      DistanceMeasured = LowLevelTime /50;   // every 50us low level stands for 1cm
      /*Serial.print("Capteur ");
      //Serial.println(ECHO);
      printCapteur(ECHO);
      Serial.print(" ");
      Serial.print(DistanceMeasured);
      Serial.println("cm");*/
      sonar_values[i] =  DistanceMeasured;
    }
  }
 }

void car_control(int* sonar_values)
{
  // Go forward => no obstacle in front
  if (sonar_values[0] > LIMIT_DISTANCE && sonar_values[1] > LIMIT_DISTANCE && sonar_values[2] > LIMIT_DISTANCE)
  {
    setAngle(90);
    setSpeed(108);
    //Serial.println("forward");
  }
  // Turn left => obstacle on the left
  else if (sonar_values[0] < LIMIT_DISTANCE && sonar_values[2] > LIMIT_DISTANCE && sonar_values[1] > LIMIT_DISTANCE) // Sonar from the left NOT OK but sonar from the right OK
  {
    setAngle(150);
    setSpeed(107);
    //Serial.println("left");

  }
  // Turn right => Obstacle on the right
  else if (sonar_values[0] > LIMIT_DISTANCE && sonar_values[2] < LIMIT_DISTANCE && sonar_values[1] > LIMIT_DISTANCE)
  {
    setAngle(40);
    setSpeed(107);
    //Serial.println("right");

  }
  // Go backward => Obstacle on the left and middle
  else if (sonar_values[3] > LIMIT_DISTANCE)
  {
    setAngle(90);
    setSpeed(55); // Go backward
    //Serial.println("back");
    }
  else
  {
    setAngle(90);
    setSpeed(90);
    //Serial.println("NOTHING");
  }
  //delay(50);
}

void loop()
{
  /*
  for (int i = 90; i > 50; i--)
  {
    setSpeed(i);
    Serial.println(i);
    delay(1000);
  }*/
  
  
  static int i = 0;
  int limit = 10000;
  if (i < limit)
  {
    i++;
    get_ultrason_values();
    car_control(sonar_values);
    //delay(50);
  }
  else
  {
    setSpeed(90);
    setAngle(90);
    delay(100);
  }
}
