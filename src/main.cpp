#include <Servo.h>

const int trigPin = 9;
const int echoPin = 10;
const int servoPin1 = 5;
const int servoPin2 = 6;
const int buzzerPin = 3;
const int ldrPin = A0;

Servo servo1;
Servo servo2;

bool isNight = false;
int delayCheckNight = 0;

void checkNight() {
  if(delayCheckNight) {
    --delayCheckNight;
  }
  else {
    delayCheckNight = 1000;
    int light = analogRead(ldrPin);
    isNight = light < 200;
  }
}

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  servo1.attach(servoPin1);
  servo2.attach(servoPin2);
  Serial.begin(9600);
}

void loop() {
  // check if it's night
  checkNight();

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