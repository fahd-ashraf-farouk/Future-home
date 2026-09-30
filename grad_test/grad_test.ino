#include "connection.h"
#include "sensors.h"
#include "motors.h"
#include "distance.h"
#include <Arduino.h>
#include "voice_controllerr.h"
#include "automatic_controller.h"
#include "home_agent.h"


const int ac_pin = 21;
const int servo_pin = 22;
const int r1_light_pin = 5;     
const int r2_light_pin = 2;     
const int r3_light_pin = 23;
const int kit_light_pin = 25;
const int rain_pin = 32;
const int ldr_pin = 35;
const int gas_pin = 34;
const int motion_pin = 33;
const int dht_pin = 14;
const int ir_pin = 36;
const int ir2_pin = 39;
const int ultra_trig_pin = 18;   
const int ultra_echo_pin = 19;   
const int kit_w_servo_pin = 16;
const int gas_pump_servo_pin = 17;
const int door_servo_pin = 4;
const int curtain_servo_pin = 26;
long door_time = 0;
bool gas_message_sent = false;
int gases =0;
bool door_help = false;
unsigned long agent_timer = 0;


sensors sens(rain_pin, motion_pin, ldr_pin, gas_pin, dht_pin, DHT11);
distance ir_ultra_dist(ultra_trig_pin, ultra_echo_pin);
ir_sens ir_room1(ir_pin);
ir_sens ir_room2(ir2_pin);
ServoMotor awning_servo(servo_pin);
ServoMotor curtain_servo(curtain_servo_pin);
ServoMotor kit_servo(kit_w_servo_pin);
ServoMotor door_servo(door_servo_pin);
ServoMotor gas_pump_servo(gas_pump_servo_pin);
voice_control voi_cont(&awning_servo, &curtain_servo, ac_pin, r1_light_pin, r2_light_pin, r3_light_pin,kit_light_pin);
automatic auto_cont(&awning_servo,&curtain_servo, ac_pin, r1_light_pin, r2_light_pin, r3_light_pin, kit_light_pin);
home_agent agent_cont(&awning_servo, &curtain_servo, ac_pin, r1_light_pin, r2_light_pin, r3_light_pin,kit_light_pin);

const char *wifi_ssid = "PUT YOUR WIFI SSID";
const char *wifi_password = "PUT YOUR WIFI PASSWORD HERE";
const char *mqtt_server = "PUT THE BROKER IP HERE";
const uint16_t mqtt_port = 1883;
const char* clientid = "esp_id";


String last_topic = "";
String last_message = "";
bool new_message = false;
const char* topics[] = {
  "home/voice_controll",
  "home/home_agent",
  "home/control/awning",
  "home/control/ac",
  "home/control/room1/light",
  "home/control/room2/light",
  "home/control/room4/light",
  "home/recognition",
  "home/control/kitchen/light",
  "home/control/curtain",
  "home/control/sensors/ac",
  "home/control/sensors/temperature_humidity",
  "home/control/sensors/gases",
  "home/control/sensors/raining",
  "home/control/sensors/lightlevel",
  "home/control/sensors/awning",
  "home/control/sensors/curtain",
  "home/agentControl/room1/light",
  "home/agentControl/room3/light", 
  "home/agentControl/room2/light",
  "home/agentControl/ac",
  "home/agentControl/awning", 
  "home/agentControl/curtain", 
  "home/agentControl/kitchen/light"
  
};
 
const char *topic_dist = "home/sensors/ultra_dist";
const char *topic_gas = "home/sensors/gas_detiction";
int gas_level = 0;

enum cont_mode{
  VOICE_CONT,
  AUTO_CONT,
  AGENT_CONT
};
enum door_cont{
  OPEN_MODE,
  COLSE_MODE
};

door_cont door_state = COLSE_MODE;
cont_mode current_mode = AUTO_CONT;


void on_message_received(char *topic, byte *payload, unsigned int length){
  last_topic = String(topic);
  String message = "";
  String command_topics= "";
  String current_command_s= "";
  int current_command = 0;
  String agent_current_command_s= "";
  int agent_current_command = 0;
  unsigned int i;
  for (i=0; i < length; i++){
    message += (char)payload[i];
  }
  for(int i=0; i<12; i++){
      command_topics += last_topic[i];
  }
  if (last_topic == "home/recognition" ){
    door_time = millis();
    door_state = OPEN_MODE;
  }
  last_message = message;
  Serial.println(last_message);
  Serial.println(last_topic);


  if(last_topic == "home/home_agent"){
    current_mode = AGENT_CONT;
    agent_timer = millis();
  }

  if(last_topic == "home/voice_controll" ){
    if(message == "on"){
      current_mode =VOICE_CONT ;
    }else if(message == "off"){
      current_mode = AUTO_CONT;
    }

  }
  if ( command_topics == "home/control"){
    if(last_topic == "home/control/ac"){
      
      current_command_s = message;
      current_command = 0;
    }else{
      current_command = message.toInt();
      current_command_s = "";}
    }
    voi_cont.set_command(last_topic,current_command,current_command_s);

  if ( command_topics == "home/agentCn"){
    if(last_topic == "home/agentControl/ac"){
      
      agent_current_command_s = message;
      agent_current_command = 0;
    }else{
      agent_current_command = message.toInt();
      agent_current_command_s = "";}
    }
  agent_cont.set_command(last_topic,agent_current_command,agent_current_command_s);

    

}

connection_manager connect(
  wifi_ssid,
  wifi_password,
  clientid,
  mqtt_server,
  on_message_received
);

unsigned long last_pub = 0;
const long interval = 15000;
unsigned long gases_dec = 0;

void setup(){
  Serial.begin(115200);
  Serial.println("Starting System Setup...");
  ir_ultra_dist.begin();
  ir_room1.begin();
  ir_room2.begin();
  connect.wifi_connect();
  connect.mqtt_setup();
  voi_cont.voice_setup();
  auto_cont.automatic_setup();
  agent_cont.homeAgent_setup();
  kit_servo.begin();
  gas_pump_servo.begin();
  awning_servo.begin();
  curtain_servo.begin();
  door_servo.begin();
  for (int i=0; i<24; i++){
    Serial.println(topics[i]);
    connect.subscribe_topic(topics[i]); 
  }
  sens.begin();
}

void loop(){
  connect.loop_check();
  if (current_mode == VOICE_CONT){
    voi_cont.run();
  }else if(current_mode == AUTO_CONT){
    auto_cont.run(&sens, &ir_room1, &ir_room2);
  }else if(current_mode == AGENT_CONT){
    agent_cont.run(&sens, &ir_room1, &ir_room2, &connect);
    if(millis() - agent_timer > 30000 ){
      current_mode = AUTO_CONT;
    }    
  }

  if(door_state == OPEN_MODE){
    last_pub = millis();
    Serial.println("door is opening");
    door_servo.servo_open(90);
    door_state = COLSE_MODE;
    door_help = true;
  }
  if (millis() - door_time > 15000 && door_help==true){
      Serial.println("door is closing");
      door_servo.servo_close(0);
      door_help = false;
  }
  if (millis() - last_pub > interval){
    float dista = ir_ultra_dist.ultra_distance();
    String payloadd = String(dista);
    last_pub = millis();
    Serial.print("distance is ");
    Serial.println(payloadd);
    if (dista <= 20){
      if(connect.publish_message(topic_dist, payloadd.c_str())){
        Serial.print("distan is ");
        Serial.println(payloadd);
      }else{
        Serial.print("failed to send");
      }
    
    }
  }
  if(new_message){
    new_message = false ;
  }
  if (millis() - gases_dec > interval){
    gases_dec = millis();
    gases = sens.gas_level();
    Serial.println("gases = ");
    Serial.println(gases);
  }

  int kit_srvo_angle = kit_servo.getAngle();
  if (gases >=70 && !gas_message_sent ){
    String gas_message = "hey be careful there is gas leakage detected";
    if(connect.publish_message(topic_gas, gas_message.c_str())){
      gas_message_sent=true;
      Serial.println("done");}
    else{Serial.println("failed to send");}
  }
  if(gases>100){
      kit_servo.servo_open(100);
      gas_pump_servo.servo_close(0);
    
  }if(gases <= 30  && kit_srvo_angle > 0){
    gas_message_sent = false;
    kit_servo.servo_close(0);
    gas_pump_servo.servo_open(90);
  }
}