#ifndef BUZZER_H
#define BUZZER_H
#include <Arduino.h>
class Buzzer {

  private:
    byte pin;

  public:
    Buzzer(byte pin);
    void init();
    void on();
    void off();
};
#endif
