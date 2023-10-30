#include <Servo.h>
#include <Arduino.h>
#include "DHT.h"

const int buzzerPin = 3;
const int ldrPin = A0;
const int ledPin = 4;
const int dhtPin = 2;

Servo servo1;
Servo servo2;

bool isNight = false;
bool didNightBeep = false;

DHT dht(dhtPin, DHT11);
float humidity, celcius, farenheit;
bool isFire = false;
bool isRain = false;

int um[8] = {262, 294, 330, 340, 392, 440, 494, 523};


void fireSiren(){
  if (!isFire)return;
  for (int hz = 300; hz <= 750; hz++)
  {
    tone(buzzerPin, hz);

    delay(5);
  }
  for (int hz = 750; hz >= 300; hz--)
  {
    tone(buzzerPin, hz);

    delay(5);
  }
}

void checkRain() {
  isRain = humidity > 80;
}

void rainSiren(){
  if (!isRain)return;
  for (int hz = 300; hz <= 750; hz++)
  {
    tone(buzzerPin, hz);

    delay(5);
  }
  for (int hz = 750; hz >= 300; hz--)
  {
    tone(buzzerPin, hz);

    delay(5);
  }
}

void nightSiren(){
  if (!isNight)return;
  if(didNightBeep) return;
  didNightBeep = true;
  int eliseMelody[] = {659, 587, 659, 587, 659, 494, 523, 587, 392, 523, 349};
  int noteDuration = 300; // 음표의 지속 시간 (밀리초)

  for (int i = 0; i < 11; i++) {
    tone(buzzerPin, eliseMelody[i], noteDuration); // 부저를 해당 음계로 설정하여 재생
    delay(noteDuration + 50);
  }
}

void stopBuzzer(){
  noTone(buzzerPin);
}


void checkNight() {
  int light = analogRead(ldrPin); // +, -, out
  if(isNight && light < 300) {
    return;
  }
  isNight = light < 300;
  didNightBeep = !isNight;
  Serial.println(light);
  Serial.println(isNight);
}

void doLight() {
  Serial.println(isNight);
  if(isNight) {
    digitalWrite(ledPin, HIGH);
  }
  else {
    digitalWrite(ledPin, LOW);
  }
}

int doDHT() {
  humidity = dht.readHumidity();
  celcius = dht.readTemperature();
  farenheit = dht.readTemperature(true);

  if (isnan(humidity) || isnan(celcius) || isnan(farenheit)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return -1;
  }

  Serial.println(humidity);
  Serial.println(celcius);
  Serial.println(farenheit);

  return 0;
}

void checkFire() {
  isFire = celcius > 25;
}

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  // Check DHT and try again if error
  if(doDHT() == -1) {
    return;
  }

  doLight();
  
  // check if there's fire
  checkFire();
  fireSiren();

  // check if it's night
  checkNight();
  nightSiren();

  // check if it's rain
  checkRain();
  rainSiren();

  delay(1000);  // 1초 대기 후 다음 측정 수행
}