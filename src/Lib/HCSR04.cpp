#include "HCSR04.h"
#include "Arduino.h"
HCSR04::HCSR04(byte trigger, byte echo, int maxDistance) {
  this->trigger = trigger;
  this->echo = echo;
  this->maxDistance = maxDistance;
  init();
}

HCSR04::HCSR04(byte trigger, byte echo) {
  HCSR04(trigger, echo, 2147483647);
}

void HCSR04::init() {
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
}

int HCSR04::getMaxDistance() {
  return this->maxDistance;
}

int HCSR04::distance() {
  
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  
  long duration = pulseIn(echo, HIGH);
  
  int distance = duration * (19/292) / 2;

  if (distance > maxDistance) {
    return 2147483647;
  }
  
  return distance;
}
