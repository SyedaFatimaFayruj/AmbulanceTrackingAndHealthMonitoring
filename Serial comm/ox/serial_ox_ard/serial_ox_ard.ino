//arduino Uncomment


#include<SoftwareSerial.h>
#include<ArduinoJson.h>
//#include <Wire.h>
#include "MAX30100_PulseOximeter.h"


SoftwareSerial uno(5,6);

PulseOximeter pox;



void onBeatDetected()
{
    Serial.println("Beat!");
}


void setup(){
    Serial.begin(115200);
    uno.begin(9600);
        if (!pox.begin()) {
        Serial.println("FAILED");
        
    } else {
        Serial.println("SUCCESS");
    }
    pox.setOnBeatDetectedCallback(onBeatDetected);
}


void loop(){
    pox.update();
        Serial.print("Heart rate:");
        Serial.print(pox.getHeartRate());
        Serial.print("bpm / SpO2:");
        Serial.print(pox.getSpO2());
        Serial.println("%");

        int bpm=pox.getHeartRate();
        int oxygen_saturation=pox.getSpO2();
        
  Serial.println(bpm);


    StaticJsonBuffer<1000>jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    root["data1"]=bpm;
    root["data2"]=oxygen_saturation;

if(uno.available()>0)
{
root.printTo(uno);

}

//Serial.println(root["data1"]);

delay(1000);
}
