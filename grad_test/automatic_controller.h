#include "esp32-hal-gpio.h"
#ifndef AUTOMATIC_CONTROLLERR_H
#define AUTOMATIC_CONTROLLERR_H
#include <Arduino.h>

class ServoMotor; 
class sensors;
class ir_sens;

class automatic{
  private:
  int speed;
  int ac_speed;
  float temp;
  float humi;
  ServoMotor *awning_servo;
  ServoMotor *curtain_servo;
  int light_level;
  bool motion;
  bool raining;
  bool ir_room1;
  bool ir_room2;
  int awning_angle = 0;
  int awaing_pin;
  int ac_pin;
  int r1_light_pin;
  int r2_light_pin;
  int r3_light_pin;
  int kit_light_pin;
  int delay_s = 250;
  int curtain_angel = 0;
  unsigned long timer = 0;
  int max_speed = 250;
  const int pwmChannel = 0;
  const int pwmFreq = 5000;
  const int pwmResolution = 8;


  public:
  automatic(ServoMotor *awning_servo, ServoMotor *curtain_servo, int acPin=-1, int R1_light_pin=-1,int R2_light_pin=-1, int R3_light_pin=-1, int Kit_light_pin=-1):awning_servo(awning_servo), curtain_servo(curtain_servo){
    ac_pin=acPin; r1_light_pin=R1_light_pin ; r2_light_pin=R2_light_pin ; r3_light_pin=R3_light_pin;
    kit_light_pin=Kit_light_pin;
  }
  void automatic_setup(){
    pinMode(r1_light_pin, OUTPUT);
    pinMode(r2_light_pin, OUTPUT);
    pinMode(kit_light_pin, OUTPUT);
    pinMode(r3_light_pin, OUTPUT);
    ledcAttach(ac_pin, pwmFreq, pwmResolution);
  }
  
  void ac(int sp){
      if (sp >= max_speed){sp = max_speed;}
      if (sp < 0 ) {sp = 0;}
      speed = sp;
      Serial.print("ac speed is : ");
      Serial.println(speed);
      ledcWrite(ac_pin, speed);
    }

  void run(sensors *sens, ir_sens *dist, ir_sens *dist2){
    if (millis() - timer > 10000){
    light_level = sens->light_reads(); motion = sens->is_moving(); raining=sens->is_raining(); 
    awning_angle = awning_servo->getAngle(); ir_room1=dist->ir_detector(); ir_room2=dist2->ir_detector();
    humi = sens->humidity(); temp = sens->tempera();


    Serial.println(raining); Serial.println(light_level); Serial.println(awning_angle);Serial.println(motion);timer=millis();Serial.println(humi);Serial.println(temp);Serial.println(light_level);Serial.println("hello"); Serial.println(curtain_angel);}

    if (temp >= 27){
      ac_speed = (temp-27) * (max_speed/50) + 27 ;
      ac(ac_speed);
    }

    if(light_level > 2800 ){curtain_angel=curtain_angel-(light_level-2800)/10 * ((4080-2700)/180);curtain_servo->servo_close(curtain_angel);}

    if(light_level < 2700){curtain_angel=(2700-light_level)/10 * ((2700-1744)/180); curtain_servo->servo_open(curtain_angel);}

    if (raining && awning_angle < 170){awning_servo->servo_open(175);awning_angle = awning_servo->getAngle();}

    if (!raining && awning_angle > 0){awning_servo->servo_close(0);}
    
    if (motion && light_level > 2850){digitalWrite(kit_light_pin, HIGH);}
    
    if (!motion) {digitalWrite(kit_light_pin, LOW);}
    
    if (ir_room1 && light_level > 2850) {digitalWrite(r1_light_pin, HIGH);}
    
    if (!ir_room1) {digitalWrite(r1_light_pin, LOW);}
    
    if (ir_room2 && light_level > 2850) {digitalWrite(r2_light_pin, HIGH);}
    
    if (!ir_room2) {digitalWrite(r2_light_pin, LOW);}
  }
};

#endif