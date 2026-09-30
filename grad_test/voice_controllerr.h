#include "esp32-hal-ledc.h"
#ifndef VOICE_CONTROLLERR_H
#define VOICE_CONTROLLERR_H
#include <Arduino.h>

class ServoMotor; 
class sensors;
class ir_sens;
class connection_manager;

class voice_control{
  private: 
    const int max_speed = 250;
    int ac_pin ;
    int speed = 0;
    int current_command;
    String current_topic;
    String current_command_s;
    int room1l_pin;
    int room2l_pin;
    int room3l_pin;
    int kit_l_pin;
    ServoMotor *awning_servo;
    ServoMotor *curtain_servo;
    const int pwmChannel = 0;
    const int pwmFreq = 5000;
    const int pwmResolution = 8;
  public:
    voice_control(ServoMotor *awning_servo,ServoMotor *curtain_servo, int acPin=-1, int R1_light_pin=-1,int R2_light_pin=-1, int R3_light_pin=-1, int Kit_light_pin=-1):awning_servo(awning_servo), curtain_servo(curtain_servo) {
      current_command_s ="";
      current_command = 0;
      ac_pin=acPin; room1l_pin=R1_light_pin ; room2l_pin=R2_light_pin ; room3l_pin=R3_light_pin;
      kit_l_pin=Kit_light_pin;

    }
    void voice_setup(){
      pinMode(room1l_pin, OUTPUT);
      pinMode(room2l_pin, OUTPUT);
      pinMode(room3l_pin, OUTPUT);
      pinMode(kit_l_pin, OUTPUT);
      ledcAttach(ac_pin, pwmFreq, pwmResolution);
      // pinMode(awning_pin, OUTPUT);
    }
    void set_command(String topic, int command, String command_s){
      current_command = command;
      current_command_s = command_s;
      current_topic = topic;

    }
    void ac(int sp){
      if (sp >= max_speed){sp = max_speed;}
      if (sp < 0 ) {sp = 0;}
      speed = sp;
      ledcWrite(ac_pin, speed);
    }
    void run(){
      if (current_topic == "") return;
      if (current_topic == "home/control/ac"){
        if (current_command_s == "raise"){
          speed += 10;
          ac(speed);          
        }else if(current_command_s == "lower"){
          speed -= 10;
          ac(speed); 
        }else if(current_command_s == "on"){
          ac(20);
        }else{
          ac(0);
        }
      }else if(current_topic == "home/control/awning"){
        Serial.print(current_command);
        if (current_command == 1){
          awning_servo->servo_open(175);
        }else{
          awning_servo->servo_close(0);
        }
      }else if(current_topic == "home/control/curtain"){
        Serial.print(current_command);
        if (current_command == 1){
          curtain_servo->servo_open(170);
        }else{
          curtain_servo->servo_close(0);
        }
    
      }else if(current_topic == "home/control/room1/light"){
        Serial.println(current_command);
        digitalWrite(room1l_pin, current_command);
      
      }else if(current_topic =="home/control/room2/light"){
        Serial.print(current_command);
        digitalWrite(room2l_pin, current_command);
      
      }else if(current_topic =="home/control/room3/light"){
        Serial.print(current_command);
        digitalWrite(room3l_pin, current_command);
      
      }else if(current_topic =="home/control/kitchen/light"){
        Serial.print(current_command);
        digitalWrite(kit_l_pin, current_command);
      
      }
      current_topic = "";
      
    }

};
#endif