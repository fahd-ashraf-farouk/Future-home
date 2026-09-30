#ifndef DISTANCE_H
#define DISTANCE_H

#include <Arduino.h>

class distance{
private:
  int trig_pin;
  int echo_pin;
  long duration;
  float dist;

public:
  distance(int trig=-1, int echo=-1) : trig_pin(trig), echo_pin(echo) {}
  void begin() {
    pinMode(trig_pin, OUTPUT);
    pinMode(echo_pin, INPUT);
  }

  float ultra_distance() {
      digitalWrite(trig_pin, LOW);
      delayMicroseconds(2);
      digitalWrite(trig_pin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trig_pin, LOW);
      duration = pulseIn(echo_pin, HIGH);
      dist = (duration*0.0343)/2; 

      return dist;
  }
};
class ir_sens{
  private:
  int ir_pin;

  public:
  ir_sens(int ir=-1) : ir_pin(ir) {}
  void begin(){pinMode(ir_pin, INPUT);}
  bool ir_detector() {
      return digitalRead(ir_pin) == LOW; 
  }
};
#endif