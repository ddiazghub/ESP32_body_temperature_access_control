#include "Buzzer.h"
Buzzer::Buzzer(byte pinIn, byte channelIn) {
  pin = pinIn;
  channel = channelIn;
  frequency = 1000;
  init();
}
void Buzzer::init() {
  ledcSetup(channel, frequency, 8);
  off();
}
void Buzzer::on(int frequency) {
  setFrequency(frequency);
  on();
}
void Buzzer::on() {
  ledcAttachPin(pin, channel);
  ledcWriteTone(channel, frequency);
}
void Buzzer::off() {
  ledcDetachPin(pin);
  ledcWrite(channel, 0);
}
void Buzzer::setFrequency(int frequency) {
  this->frequency = frequency;
}
void Buzzer::siren() {
  for (int i = 0; i < 20; i++) {
    on(1200);
    delay(250);
    on(800);
    delay(250);
  }
  off();
}
void Buzzer::error() {
  on(200);
  delay(3000);
  off();
}
void Buzzer::success() {
  for (byte i = 0; i < 3; i++) {
    on(2700);
    delay(100);
    off();
    delay(25);
  }
}