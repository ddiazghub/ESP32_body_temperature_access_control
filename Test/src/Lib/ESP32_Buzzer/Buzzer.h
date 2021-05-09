#ifndef BUZZER_H
#define BUZZER_H
#include <Arduino.h>
#include "Pitches.h"
class Buzzer {

  private:
    byte pin;
    byte channel;
    int frequency;

  public:
    Buzzer(byte pinIn, byte channelIn);
    void init();
    void setFrequency(int frequency);
    void on();
    void on(int frequency);
    void off();
    void siren();
    void error();
    void success();
};
#endif
