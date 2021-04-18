#ifndef HCSR04_H
#define HCSR04_H
#include <Arduino.h>
class HCSR04 {

  private:
    byte trigger;
    byte echo;
    int maxDistance;

  public:
    HCSR04(byte trigger, byte echo, int maxDistance);
    HCSR04(byte trigger, byte echo);
    void init();
    int distance();
    int getMaxDistance();
};
#endif
