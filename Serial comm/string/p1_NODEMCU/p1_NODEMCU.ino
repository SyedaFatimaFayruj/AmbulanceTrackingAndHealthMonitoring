#include<SoftwareSerial.h>
#include<ArduinoJson.h>

SoftwareSerial nodemcu(D6,D5);
void setup(){

Serial.begin(9600);
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
Serial.println("");
int data1=root["data1"];
Serial.print(data1);
Serial.print("2nd Data :");
int data2=root["data2"];
Serial.print(data2);
Serial.println("");
Serial.println("--------");




}
