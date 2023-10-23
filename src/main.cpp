#include <Arduino.h>

bool isNight = false;
int delayCheckNight = 0;

bool buzzProximity = false;
bool buzzFire = false;
bool buzzRain = false;

void CheckNight() {
  if(delayCheckNight) {
    --delayCheckNight;
  }
  else {
    delayCheckNight = 5000;
    //isNight = SOME_FUNCTION_TO_CHECK_SENSOR();
  }
}

void CheckProximity() {
  if(isNight) {
    //buzzProximity = SOME_FUNCTION_TO_CHECK_SENSOR();
  }
}
void CheckFire() {
  //buzzFire = SOME_FUNCTION_TO_CHECK_SENSOR();
}
void CheckRain() {
  //buzzRain = SOME_FUNCTION_TO_CHECK_SENSOR();
}

void DoBridge() {
  if(isNight) {
    //SOME_FUNCTION_TO_CONTROL_MOTOR();
  }
}

void DoBuzz() {
  if(buzzRain) {
    //SOME_FUNCTION_TO_CONTROL_BUZZER();
  }
  else if(buzzFire) {
    //SOME_FUNCTION_TO_CONTROL_BUZZER();
  }
  else if(buzzProximity) {
    //SOME_FUNCTION_TO_CONTROL_BUZZER();
  }
}

void DoLight() {
  if(isNight) {
    //SOME_FUNCTION_TO_CONTROL_LED();
  }
}

void setup() {
}

void loop() {
  CheckNight();
  CheckProximity();
  CheckFire();
  CheckRain();

  DoBridge();
  DoBuzz();
  DoLight();
}