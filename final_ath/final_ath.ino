
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include "MAX30100_PulseOximeter.h"
/********************************
 * Constants and objects
 *******************************/
///senors
#define SENSOR A0 // Set the A0 as SENSOR
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
PulseOximeter pox;


//


char const * SSID_NAME = "Fluh"; // Put here your SSID name
char const * SSID_PASS = "bleh1233"; // Put here your password
char const * TOKEN = "BBFF-2xa6B8EJWqBjnRlny7YwUokebPu7TM"; // Put here your TOKEN

char const * HTTPSERVER = "things.ubidots.com";

char const * DEVICE_LABEL = "eee416_project"; // Put here your device label

/* Put here your variable's labels*/
char const * VARIABLE_LABEL_1 = "ECG_var";
char const * VARIABLE_LABEL_2 = "Temp_var";
char const * VARIABLE_LABEL_3 = "Oxy_var";
/*char const * VARIABLE_LABEL_4 = "var-test-4";
char const * VARIABLE_LABEL_5 = "var-test-5";
char const * VARIABLE_LABEL_6 = "var-test-6";
**/




const int HTTPPORT = 80;

char const * USER_AGENT = "ESP8266";
char const * VERSION = "1.0";
char payload[700];
char topic[1000];

WiFiClient clientUbi;

// Space to store values to send
char str_val_ECG[30];
char str_val_Temp[30];
char str_val_OX[30];
/*char str_val_4[30];
char str_val_5[30];
char str_val_6[30];
*/
/********************************
 * Auxiliar Functions
 *******************************/

void SendToUbidots(char* payload) {

  int i = strlen(payload); 
  /* Builds the request POST - Please reference this link to know all the request's structures https://ubidots.com/docs/api/ */
  sprintf(topic, "POST /api/v1.6/devices/%s/?force=true HTTP/1.1\r\n", DEVICE_LABEL);
  sprintf(topic, "%sHost: things.ubidots.com\r\n", topic);
  sprintf(topic, "%sUser-Agent: %s/%s\r\n", topic, USER_AGENT, VERSION);
  sprintf(topic, "%sX-Auth-Token: %s\r\n", topic, TOKEN);
  sprintf(topic, "%sConnection: close\r\n", topic);
  sprintf(topic, "%sContent-Type: application/json\r\n", topic);
  sprintf(topic, "%sContent-Length: %d\r\n\r\n", topic, i);
  sprintf(topic, "%s%s\r\n", topic, payload);

  /* Connecting the client */
  clientUbi.connect(HTTPSERVER, HTTPPORT); 

  if (clientUbi.connected()) {
    /* Sends the request to the client */
    clientUbi.print(topic);
    Serial.println("Connected to Ubidots - POST");
  } else {
    Serial.println("Connection Failed ubidots - Try Again"); 
  }  
    
  /* Reads the response from the server */
  while (clientUbi.available()) {
    char c = clientUbi.read();
    //Serial.print(c); // Uncomment this line to visualize the response on the Serial Monitor
  }

  /* Disconnecting the client */
  clientUbi.stop();
}

/********************************
 * Main Functions
 *******************************/

void setup() {
  Serial.begin(9600);
  
  /* Connects to AP */
  WiFi.begin(SSID_NAME, SSID_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    while(!Serial)
    {;
      }
    }

  WiFi.setAutoReconnect(true);
  Serial.println(F("WiFi connected"));
  Serial.println(F("IP address: "));
  Serial.println(WiFi.localIP());
  // Assign the pin as INPUT 
  pinMode(14, INPUT); // Setup for leads off detection LO +
  pinMode(12, INPUT); // Setup for leads off detection LO -
   mlx.begin();  
   pox.begin();

}

void loop() {

  //if(Serial.available())
  //{ 
    String str_val_Temp= Serial.readString();
    Serial.println(str_val_Temp);
   // delay(2000);
 // }
    pox.update();
  /* Adapt your value array following the structure specified at https://ubidots.com/docs/api/index.html#send-values-to-one-variable */
  
  /*---- Simulates the values of the sensors -----*/
  float sensor_value_ECG =analogRead(SENSOR) ;
  float sensor_value_OX =pox.getSpO2();
  /*float sensor_value_3 = random(0, 1000)*1.0;
  float sensor_value_4 = random(0, 1000)*1.0;
  float sensor_value_5 = random(0, 1000)*1.0;
  float sensor_value_6 = random(0, 1000)*1.0;
*/
  
  /*---- Transforms the values of the sensors to char type -----*/

  /* 4 is mininum width, 2 is precision; float value is copied onto str_val*/
  dtostrf(sensor_value_ECG, 4, 2, str_val_ECG);
  dtostrf(sensor_value_OX, 4, 2, str_val_OX);
 
 //dtostrf(sensor_value_Temp, 4, 2, str_val_Temp);
  /*dtostrf(sensor_value_4, 4, 2, str_val_4);
  dtostrf(sensor_value_5, 4, 2, str_val_5);
  dtostrf(sensor_value_6, 4, 2, str_val_6);
  */
  /* Builds the payload with structure: {"temperature":25.00,"humidity":50.00} for first 4 variables*/

  // Important: Avoid to send a very long char as it is very memory space costly, send small char arrays
  sprintf(payload, "{\"");
  sprintf(payload, "%s%s\":%s", payload, VARIABLE_LABEL_1, str_val_ECG);
  sprintf(payload, "%s,\"%s\":%s", payload, VARIABLE_LABEL_2, str_val_Temp);
  sprintf(payload, "%s,\"%s\":%s", payload, VARIABLE_LABEL_3, str_val_OX);
  //sprintf(payload, "%s,\"%s\":%s", payload, VARIABLE_LABEL_4, str_val_4);*/
  sprintf(payload, "%s}", payload);
  Serial.println(payload);

  /* Calls the Ubidots Function POST */
  SendToUbidots(payload);

  /* Builds the payload with structure: {"temperature":25.00,"humidity":50.00} for the rest of variables*/
 /*
  sprintf(payload, "%s", ""); //Cleans the content of the payload
  sprintf(payload, "{\"");
  sprintf(payload, "%s%s\":%s", payload, VARIABLE_LABEL_5, str_val_5);
  sprintf(payload, "%s,\"%s\":%s", payload, VARIABLE_LABEL_6, str_val_6);
  sprintf(payload, "%s}", payload);
*/
  /* Calls the Ubidots Function POST */
 /*
  SendToUbidots(payload);
  */
  delay(4000);
  
}
