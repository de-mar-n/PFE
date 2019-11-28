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
#define NB_SONAR 3
// PWM Output 0-25000US,Every 50US represent 1cm

int URECHO1 = 3; //Ultrason du gauche (quand on regarde la voiture en face)   
int URECHO2 = 11; // Ultrason de milieu 
int URECHO3 = 12; // Ultrason arriere

int URECHO_array[] = {URECHO1, URECHO2, URECHO3};

int URTRIG = 5;         // PWM trigger pin


// variable to store the value coming from the sensor
int sensorValue = 0;
//int sensorVal2 = 0;
//int sensorVal3 = 0;


//int sensorPin = A0;     // select the input pin for the potentiometer

unsigned int DistanceMeasured= 0;

int PWM_Mode(int ECHO)                              // a low pull on pin COMP/TRIG  triggering a sensor reading
{
  Serial.print("Distance Measured=");
  digitalWrite(URTRIG, LOW);
  digitalWrite(URTRIG, HIGH);               // reading Pin PWM will output pulses

  unsigned long LowLevelTime = pulseIn(ECHO, LOW) ;
  if(LowLevelTime>=45000)                 // the reading is invalid.
  {
    Serial.print("Invalid");
    return ERROR_VALUE;
  }
  else{
    DistanceMeasured = LowLevelTime /50;   // every 50us low level stands for 1cm
    Serial.print("Capteur ");
    Serial.println(ECHO);
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
  pinMode(URTRIG,OUTPUT);                    // A low pull on pin COMP/TRIG
  digitalWrite(URTRIG,HIGH);                 // Set to HIGH
  //pinMode(URECHO1, INPUT);                    // Sending Enable PWM mode command
  delay(500);
  Serial.println("Init the sensor");

  // ------- FOR MOVE -------
  // Attach the motor on pin 9
  mot.attach(9,1000,2000); // (pin, min pulse width, max pulse width in microseconds) 
  dir.attach(6, 1000, 2000);
  delay(1500);
}

// Return an array with the values of ultrasons 
int* get_ultrason_values()
{
  int* sonar_values = malloc(NB_SONAR * sizeof(int));
  for (unsigned int i; i < NB_SONAR; ++i)
  {
    sonar_values[i] = PWM_Mode(URECHO_array[i]);
  }
  return sonar_values;
}

void car_control(int* sonar_values)
{
  // Go forward => no obstacle in front
  if (/*sonar_values[0] > LIMIT_DISTANCE &&*/ sonar_values[1] > LIMIT_DISTANCE) // Sonar from the left and the middle OK
  {
    setSpeed(107);
    //delay(800);
  }
  // Turn right => obstacle on the left
  else if (sonar_values[0] < LIMIT_DISTANCE /*&& sonar_values[1] > LIMIT_DISTANCE*/) // Sonar from the left NOT OK but sonar from the middle OK
  {
    setAngle(40);
    delay(900);
    //setSpeed(107);
    //delay(800);
  }
  // Go backward => Obstacle on the left and middle
  else if (/*sonar_values[0] < LIMIT_DISTANCE && sonar_values[1] < LIMIT_DISTANCE &&*/ sonar_values[2] > LIMIT_DISTANCE)
  {
    setSpeed(75); // Go backward
    //delay(800);
  }
  // Turn left => Obstacle on the right 
  else // (sonar_values[0] > LIMIT_DISTANCE /*&& sonar_values[1] > LIMIT_DISTANCE*/) //FIXME : no sonar on the left
  {
    setAngle(100);
    delay(900);
    //setSpeed(107);
    //delay(800);
    
  }
}

void loop()
{
  car_control(get_ultrason_values());
}
