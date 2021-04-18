#include "Buzzer.h"
#include "Arduino.h"
Buzzer::Buzzer(byte pinIn) : pin(pinIn) {
  init();
}
void Buzzer::init() {
  pinMode(pin, OUTPUT);
  off();
}
void Buzzer::on() {
  tone(pin, 1000);
}
void Buzzer::off() {
  noTone(pin);
}
