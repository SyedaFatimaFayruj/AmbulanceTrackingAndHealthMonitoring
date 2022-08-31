#include "MAX30100_PulseOximeter.h"
//#include <SoftwareSerial.h>
//#include <Adafruit_MLX90614.h>
//Adafruit_MLX90614 mlx = Adafruit_MLX90614();
#include "ThingSpeak.h"
#include <ESP8266WiFi.h>
PulseOximeter pox;

//static const int RXPin = 5, TXPin = 4;
static const uint32_t GPSBaud = 9600;
char bpm;
char ox;
String temp;
//float latitude , longitude;
//String  lat_str , lng_str;
// repace your wifi username and password
const char* ssid     = "BW";
const char* password = "12345678R";
unsigned long myChannelNumber =1845527;
const char * myWriteAPIKey = "UAT42P8PRBNNR1N9";
// The TinyGPS++ object
//TinyGPSPlus gps;
WiFiClient  client;
// The serial connection to the GPS device
//SoftwareSerial ss(RXPin, TXPin);
void onBeatDetected()
{
    Serial.println("Beat!");
}


void setup()
{
  Serial.begin(9600);
  //ss.begin(GPSBaud);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
   
  }
   Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Netmask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: ");
  Serial.println(WiFi.gatewayIP());
  ThingSpeak.begin(client);
  
    if (!pox.begin()) {
        Serial.println("FAILED");
        for(;;);
    } else {
        Serial.println("SUCCESS");
    }
     pox.setOnBeatDetectedCallback(onBeatDetected);
}
void loop()
{
//  while (ss.available() > 0){
//    if (gps.encode(ss.read()))
//    {
//      if (gps.location.isValid())
//      {
//        latitude = gps.location.lat();
//        lat_str = String(latitude , 6);
//        longitude = gps.location.lng();
//        lng_str = String(longitude , 6);
//        Serial.print("Latitude = ");
//        Serial.println(lat_str);
//        Serial.print("Longitude = ");
//        Serial.println(lng_str);
//        ThingSpeak.setField(1, lat_str);
//        ThingSpeak.setField(2, lng_str);
//        ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey); 
//      }

  pox.update();
        Serial.print("Heart rate:");
        Serial.print(pox.getHeartRate());
        Serial.print("bpm / SpO2:");
        Serial.print(pox.getSpO2());
        Serial.println("%");
        int bpm=pox.getHeartRate();
        int ox=pox.getSpO2();

  //bpm=String(heart_rate,6);
  Serial.print("Object = "); Serial.println(bpm);
  //ox=String(oxygen,6);
  Serial.print("Object = "); Serial.println(ox);
  

     
     
     ThingSpeak.setField(3,bpm);
     ThingSpeak.setField(4,ox);
     ThingSpeak.writeFields(myChannelNumber,myWriteAPIKey);
     
     delay(3000);
     Serial.println();  
    
  
}
