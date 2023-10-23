#include <Arduino.h>

bool isNight = false;
int delayCheckNight = 0;

void CheckNight() {
  if(delayCheckNight) {
    --delayCheckNight;
  }
  else {
    delayCheckNight = 5000;
    //isNight = SOME_FUNCTION_TO_CHECK_SENSOR();
  }
}

void setup() {
}

void loop() {

}