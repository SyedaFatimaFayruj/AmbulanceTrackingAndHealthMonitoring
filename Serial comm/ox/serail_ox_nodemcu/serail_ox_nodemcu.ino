#include <ESP8266WiFi.h>
/*#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
*/
#include<SoftwareSerial.h>
#include<ArduinoJson.h>

SoftwareSerial nodemcu(D6,D5);
void setup(){

Serial.begin(115200);
nodemcu.begin(9600);
while(!Serial)
continue;
}

void loop(){

StaticJsonBuffer<1000>jsonBuffer;
JsonObject& root=jsonBuffer.parseObject(nodemcu);
if(root ==JsonObject::invalid())
    return;

Serial.println("Json received.");
root.prettyPrintTo(Serial);
Serial.print("1st Data:");
Serial.print("");
int data1=root["data1"];
Serial.println(data1);
Serial.print("2nd Data :");
int data2=root["data2"];
Serial.println(data2);
Serial.println("");
Serial.println("--------");




}
