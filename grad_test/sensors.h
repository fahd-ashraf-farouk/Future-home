#include "esp32-hal-adc.h"
#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include "DHT.h"

class sensors{
  private:
  int rain_pin;
  int motion_pin;
  int light_pin;
  int gas_pin;
  int dht_pin;
  int dht_type;
  DHT dht;
  float temp;
  float humi;
  public:

  sensors(int rpin=-1, int mpin=-1, int lpin=-1, int gpin=-1, int dhtpin=-1, int dhttype =0)
  :dht(dhtpin, dhttype)
  {
    rain_pin = rpin; motion_pin = mpin; light_pin = lpin; gas_pin = gpin;
    dht_pin = dhtpin; dht_type = dhttype; 
  }

  void begin(){
    pinMode(rain_pin, INPUT); pinMode(motion_pin, INPUT); pinMode(light_pin, INPUT); pinMode(gas_pin, INPUT);
    dht.begin();
  }

  bool is_raining(){
    return digitalRead(rain_pin) == LOW;
  }

  bool is_moving(){
    return digitalRead(motion_pin) == HIGH;
  }

  int light_reads(){
    return analogRead(light_pin);
  }

  int gas_level(){
    return analogRead(gas_pin);
  }

  float humidity(){
    humi = dht.readHumidity();
    

    if (isnan(humi)){humi=-1;}
    
    return humi;
  }
  float tempera(){
    temp = dht.readTemperature();if(isnan(temp)){temp=-1;} return temp;
  }
};

#endif