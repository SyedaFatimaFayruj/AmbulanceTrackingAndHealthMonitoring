//arduino Uncomment


#include<SoftwareSerial.h>
#include<ArduinoJson.h>
SoftwareSerial uno(5,6);

void setup(){
    Serial.begin(9600);
    uno.begin(9600);
}


void loop(){
StaticJsonBuffer<1000>jsonBuffer;
JsonObject& root = jsonBuffer.createObject();
root["data1"]=100;
root["data2"]=200;

if(uno.available()>0)
{
root.printTo(uno);

}

Serial.println("Yes!");

delay(1000);
}
