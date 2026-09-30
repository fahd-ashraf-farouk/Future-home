#include "esp32-hal-ledc.h"
#include "hal/ledc_ll.h"
#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include <ESP32Servo.h>
#include <Arduino.h>

class ServoMotor {
private:
  Servo myServo;
  int pin;
  unsigned long last_move=0;
  int i;
  int reads=0;
public:
  ServoMotor(int servoPin) : pin(servoPin) {}

  void begin() {
      myServo.attach(pin);
  }

  void servo_open(int angle) {
      if (angle >= 0 && angle <= 180) {
        i = myServo.read();
        for(i; i<=angle; i+=5){
          myServo.write(i);
        }   
      }
   }
  void servo_close(int angle){
    if (angle >= 0 && angle <= 180){
      i = myServo.read();
      for(i; i>=angle; i-=5){
          myServo.write(i);
        }
      }
  }
  int getAngle() {
    reads = myServo.read();
    if (reads<0 || reads > 180){reads = 0;}
    return reads;
  }
};

class DCMotor {
private:
  int speedPin;
  const int pwmFreq = 5000;
  const int pwmResolution = 8;

public:
  DCMotor(int speedP) : speedPin(speedP) {}

  void begin() {
    ledcAttach(speedPin, pwmFreq, pwmResolution);
  }

  void setSpeed(int speed) {
      int actualSpeed = abs(speed);
      if (actualSpeed > 255) actualSpeed = 255; 
      ledcWrite(speedPin, actualSpeed);
  }
};

#endif 