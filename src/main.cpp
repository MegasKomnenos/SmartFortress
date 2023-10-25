#include <Servo.h>
#include <Arduino.h>
#include "DHT.h"

const int trigPin = 9;
const int echoPin = 10;
const int servoPin1 = 5;
const int servoPin2 = 6;
const int buzzerPin = 3;
const int ldrPin = A0;
const int ledPin = 11;
const int dhtPin = 2;

Servo servo1;
Servo servo2;

bool isNight = false;

DHT dht(dhtPin, DHT11);
float humidity, celcius, farenheit;
bool isFire = false;

int um[8] = {262, 294, 330, 340, 392, 440, 494, 523};


void fireSiren(){
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
  isNight = light < 300;
  Serial.println(light);
}

void doLight() {
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
  isFire = celcius > 30;
}

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  servo1.attach(servoPin1);
  servo2.attach(servoPin2);
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  // Check DHT and try again if error
  if(doDHT() == -1) {
    return;
  }
  
  // check if there's fire
  checkFire();

  // check if it's night
  checkNight();

  // turn on led if it's night
  doLight();

  // 초음파 센서로 거리 측정
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  int duration = pulseIn(echoPin, HIGH);
  float distance = duration * 0.034 / 2;

  // 포토 레지스터 값 읽기
  int ldrValue = analogRead(ldrPin);

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm, LDR Value: ");
  Serial.println(ldrValue);

  // 거리에 따라 서보 모터와 부저 제어
  if (distance < 10) {
    servo1.write(90);  // 서보 모터 1을 90도로 회전
    servo2.write(90);  // 서보 모터 2를 90도로 회전
    tone(buzzerPin, 1000);  // 1kHz 주파수로 부저 울림
  } else {
    servo1.write(0);  // 서보 모터 1을 0도로 회전
    servo2.write(0);  // 서보 모터 2를 0도로 회전
    noTone(buzzerPin);  // 부저 중지
  }

  delay(1000);  // 1초 대기 후 다음 측정 수행
}