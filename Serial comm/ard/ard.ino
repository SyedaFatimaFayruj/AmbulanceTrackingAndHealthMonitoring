#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

//Arduino to NodeMCU Lib
#include <SoftwareSerial.h>
#include <ArduinoJson.h>



//Initialise Arduino to NodeMCU (5=Rx & 6=Tx)
SoftwareSerial nodemcu(5, 6);

//Initialisation of sensors
PulseOximeter pox;

float bpm;
float ox;

void setup() {
  Wire.begin();
  Serial.begin(115200);
 
  
  nodemcu.begin(115200);
  delay(1000);

  Serial.println("Program started");
  delay(1000);
  pox.begin();

}

void loop() {

  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& data = jsonBuffer.createObject();
    pox.update();
  //Obtain Temp and Hum data
   bpm=pox.getHeartRate();
   ox=pox.getSpO2();
   data["data"] = bpm;
   data["flag"] = 1;
   data.printTo(nodemcu);
   jsonBuffer.clear();

   delay(2000);

    Serial.print("BPM : ");Serial.println((int)bpm);
   
   
   data["data"] = ox;
   data["flag"] = 2;
   data.printTo(nodemcu);
   jsonBuffer.clear();

   delay(2000);

   
   }

   
