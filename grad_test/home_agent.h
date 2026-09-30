#include "esp32-hal-ledc.h"
#ifndef HOME_AGENT_H
#define HOME_AGENT_H
#include <Arduino.h>

class ServoMotor; 
class sensors;
class ir_sens;
class connection_manager;

class home_agent{
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
    home_agent(ServoMotor *awning_servo,ServoMotor *curtain_servo, int acPin=-1, int R1_light_pin=-1,int R2_light_pin=-1, int R3_light_pin=-1, int Kit_light_pin=-1):awning_servo(awning_servo), curtain_servo(curtain_servo) {
      current_command_s ="";
      current_command = 0;
      ac_pin=acPin; room1l_pin=R1_light_pin ; room2l_pin=R2_light_pin ; room3l_pin=R3_light_pin;
      kit_l_pin=Kit_light_pin;

    }
    void homeAgent_setup(){
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
    void run(sensors *sens, ir_sens *dist, ir_sens *dist2, connection_manager *connect ){
      if (current_topic == "") return;
      if (current_topic == "home/agentControl/ac"){
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
            
      }else if(current_topic == "home/agentControl/awning"){
        Serial.print(current_command);
        if (current_command == 1){
          awning_servo->servo_open(175);
        }else{
          awning_servo->servo_close(0);
        }
      }else if(current_topic == "home/agentControl/curtain"){
        Serial.print(current_command);
        if (current_command == 1){
          curtain_servo->servo_open(170);
        }else{
          curtain_servo->servo_close(0);
        }
    
      }else if(current_topic == "home/agentControl/room1/light"){
        Serial.println(current_command);
        digitalWrite(room1l_pin, current_command);
      
      }else if(current_topic =="home/agentControl/room2/light"){
        Serial.print(current_command);
        digitalWrite(room2l_pin, current_command);
      
      }else if(current_topic =="home/agentControl/room3/light"){
        Serial.print(current_command);
        digitalWrite(room3l_pin, current_command);
     
      }else if(current_topic =="home/agentControl/kitchen/light"){
        Serial.print(current_command);
        digitalWrite(kit_l_pin, current_command);
      
      }else if(current_topic == "home/control/sensors/ac"){
        String st_speed = "The ac speed is " + String(speed);
        connect->publish_message("home/control/sensors/ac", st_speed.c_str());
      
      }else if(current_topic == "home/control/sensors/temperature_humidity"){
        float temp = sens->tempera();
        float humi = sens->humidity(); 
        String temperature_humidity = "The temperature is : " + String(temp) + "Celsius and the humidity is : " + String(humi)+ "Relative Humidity";
        connect->publish_message("home/control/sensors/temperature_humidity", temperature_humidity.c_str());
        Serial.println("done");
      }else if(current_topic == "home/control/sensors/raining"){
        bool raining = sens->is_raining(); 
        String straining = "The raining condition is : " + String(raining);
        connect->publish_message("home/control/sensors/raining", straining.c_str());
        Serial.println("done");
      }else if(current_topic == "home/control/sensors/lightlevel"){
        int light_level = sens->light_reads();
        String stlight_level = "The light level is : " + String(light_level) +" Lux";
        connect->publish_message("home/control/sensors/lightlevel", stlight_level.c_str());
        Serial.println("done");
      }else if(current_topic == "home/control/sensors/awning"){
        int awning_angle = awning_servo->getAngle();
        String stawning_angle = "The awning angle is about : " + String(awning_angle) +" degree";
        connect->publish_message("home/control/sensors/awning", stawning_angle.c_str());
        Serial.println("done");
      }else if(current_topic == "home/control/sensors/curtain"){
        int curtain_angle = curtain_servo->getAngle();
        String stcurtain_angle = "The curtain angle is about : " + String(curtain_angle) +" degree";
        connect->publish_message("home/control/sensors/curtain", stcurtain_angle.c_str());
        Serial.println("done");
      }else if(current_topic == "home/control/sensors/gases"){
        int gases = sens->gas_level();
        String stgases_level = "The gases level is : " + String(gases);
        connect->publish_message("home/control/sensors/gases", stgases_level.c_str());
        Serial.println("done");
      }

        
    

      current_topic = "";
      
    }
};
#endif