#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

//Include Lib for Arduino to Nodemcu
#include <SoftwareSerial.h>
#include <ArduinoJson.h>

//D6 = Rx & D5 = Tx
SoftwareSerial nodemcu(D6, D5);



// WiFi parameters
#define WLAN_SSID       "BW"
#define WLAN_PASS       "12345678R"
// Adafruit IO
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME  "Nafis_Washir"
#define AIO_KEY       "aio_iaao51uEFCb5yNpzMRRYskYrJVDO"
WiFiClient client;
// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Publish BPM1 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/BPM1");
Adafruit_MQTT_Publish Oxygen_level1 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Oxygen_level1");

void setup() {
  
  Serial.begin(9600);
  Serial.println(F("Adafruit IO Example"));
  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  delay(10);
  Serial.print(F("Connecting to "));
  Serial.println(WLAN_SSID);
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println();
  Serial.println(F("WiFi connected"));
  Serial.println(F("IP address: "));
  Serial.println(WiFi.localIP());

  // connect to adafruit io
  connect();
  // Initialize Serial port
  //Serial.begin(9600);
  nodemcu.begin(9600);
  while (!Serial) continue;
  
}
void connect() {
  Serial.print(F("Connecting to Adafruit IO... "));
  int8_t ret;
  while ((ret = mqtt.connect()) != 0) {
    switch (ret) {
      case 1: Serial.println(F("Wrong protocol")); break;
      case 2: Serial.println(F("ID rejected")); break;
      case 3: Serial.println(F("Server unavail")); break;
      case 4: Serial.println(F("Bad user/pass")); break;
      case 5: Serial.println(F("Not authed")); break;
      case 6: Serial.println(F("Failed to subscribe")); break;
      default: Serial.println(F("Connection failed")); break;
    }

    if(ret >= 0)
      mqtt.disconnect();

    Serial.println(F("Retrying connection..."));
    delay(10000);
  }
  Serial.println(F("Adafruit IO Connected!"));
}


int flg=1;
void loop() {
  if(! mqtt.ping(3)) {
    // reconnect to adafruit io
    if(! mqtt.connected())
      connect();
  }
  
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& data = jsonBuffer.parseObject(nodemcu);

  if (data == JsonObject::invalid()) {
    Serial.println("Invalid Json Object");
    jsonBuffer.clear();
    return;
  }
  

  //Serial.println("JSON Object Recieved");
  flg= data["flag"];

  if (flg==1){
    float bpm = data["data"];
    Serial.print("BPM :  ");
    Serial.println(bpm);
    

    if (! BPM1.publish(bpm)) {                     //Publish to Adafruit
      Serial.println(F("Failed"));
    }
  }
  if (flg==2){
    float ox = data["data"];
    Serial.print("Oxygen Level:  ");
    Serial.println(ox);
    

    if (! Oxygen_level1.publish(ox)) {                     //Publish to Adafruit
      Serial.println(F("Failed"));
    } 
  }
  
  
delay(4000);
}
