#include "Led.h"
Led::Led(byte pinIn) : pin(pinIn) {
  init();
}
void Led::init() {
  pinMode(pin, OUTPUT);
  off();
}
void Led::on() {
  digitalWrite(pin, HIGH);
}
void Led::off() {
  digitalWrite(pin, LOW);
}
