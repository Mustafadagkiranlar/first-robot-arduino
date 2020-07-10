#include <NewPing.h> //For ultrasonic sensor
#include <SparkFun_TB6612.h>

#define AIN1        6
#define AIN2        5
#define PWMA        10
#define BIN1        7
#define BIN2        8
#define PWMB        9
#define STBY        4

#define BI_LED_RED  A3
#define BI_LED_GRN  11
#define DIR_LED_LFT 12
#define DIR_LED_RHT 13

#define US_ECHO A4
#define US_TRIG A5

#define SENSOR_LEFT     2
#define SENSOR_RIGHT     3

#define AUX_OUTPUT A2
#define LDR A1
#define BATT_VOLTAGE A0
#define maxDistance 200
NewPing sonar(US_TRIG, US_ECHO, maxDistance);

const int offsetA = 1;
const int offsetB = 1;
const int nSpeed = 250;
int carSpeed = 0;
bool togi = false;
int sensorLeftState = 0;
int sensorRightState = 0;
int sonarState = 0;

unsigned long ti, tf, tt;

Motor leftmotor = Motor(AIN2, AIN1, PWMA, offsetA, STBY);
Motor rightmotor = Motor(BIN1, BIN2, PWMB, offsetB, STBY);

void toggleDir()
{
  if (togi)
  {
    digitalWrite(DIR_LED_LFT, HIGH);
    digitalWrite(DIR_LED_RHT, LOW);
  }
  else
  {
    digitalWrite(DIR_LED_LFT, LOW);
    digitalWrite(DIR_LED_RHT, HIGH);
  }
  togi = !togi;
}

void setup()
{
  Serial.begin(9600);
  pinMode(BI_LED_RED, OUTPUT);
  pinMode(BI_LED_GRN, OUTPUT);
  pinMode(DIR_LED_LFT, OUTPUT);
  pinMode(DIR_LED_RHT, OUTPUT);
  digitalWrite(DIR_LED_RHT, LOW);
  digitalWrite(DIR_LED_RHT, LOW);
  leftmotor.brake();
  rightmotor.brake();
}


void loop()
{
  sensorLeftState = digitalRead(SENSOR_LEFT);
  sensorRightState = digitalRead(SENSOR_RIGHT);
  sonarState = sonar.ping_cm();

  if (sonarState >= 1 && sonarState <= 15) {
    brake(leftmotor, rightmotor );
    digitalWrite(DIR_LED_LFT, HIGH);
    digitalWrite(DIR_LED_RHT, HIGH);
    digitalWrite(BI_LED_GRN, LOW);
    digitalWrite(BI_LED_RED, LOW);
  } else if (sonarState >= 25 || sonarState == 0) {
    forward(leftmotor, rightmotor, nSpeed);
    if (sensorRightState == 1 && sensorLeftState == 0) {
      left(leftmotor, rightmotor, 255);
    } else if (sensorLeftState == 1 && sensorRightState == 0) {
      right(leftmotor, rightmotor, 255);
    } else if (sensorLeftState == 1 && sensorRightState == 1) {
      brake(leftmotor, rightmotor);
    }
    digitalWrite(BI_LED_GRN, HIGH);
    digitalWrite(BI_LED_RED, LOW);
    digitalWrite(DIR_LED_LFT, LOW);
    digitalWrite(DIR_LED_RHT, LOW);
  } else if (sonarState > 15 && sonarState < 25) {
    if (sensorRightState == 1 && sensorLeftState == 0) {
      left(leftmotor, rightmotor, 255);
    } else if (sensorLeftState == 1 && sensorRightState == 0) {
      right(leftmotor, rightmotor, 255);
    } else if (sensorLeftState == 1 && sensorRightState == 1) {
      brake(leftmotor, rightmotor);
    }
    carSpeed = map(sonarState, 5, 25, 40, 250);
    forward(leftmotor, rightmotor, carSpeed);
    digitalWrite(BI_LED_GRN, LOW);
    digitalWrite(BI_LED_RED, HIGH);
    digitalWrite(DIR_LED_LFT, LOW);
    digitalWrite(DIR_LED_RHT, LOW);
  }
  //Serial.print("Distance: "); Serial.print(sonarState); Serial.println("cm");
  //Serial.println(carSpeed);
  delay(10);
}
