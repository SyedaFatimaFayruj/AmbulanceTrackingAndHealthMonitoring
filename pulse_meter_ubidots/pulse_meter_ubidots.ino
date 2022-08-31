
/********************************
 * Libraries included
 *******************************/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

//#include <TinyGPS++.h>
#include <SoftwareSerial.h>


/********************************
 * Constants and objects
 *******************************/
///senors
/*#define SENSOR A0 // Set the A0 as SENSOR
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
SoftwareSerial serial_connection(0,2); //tx,rx 
*/
/********************************
 * Constants and objects
 *******************************/
//static const int RXPin = 5, TXPin = 4;
static const uint32_t GPSBaud = 9600;
#define REPORTING_PERIOD_MS     1000

//TinyGPSPlus gps;// GPS object to process the NMEA data
PulseOximeter pox;


//
//pulse_oxymeter variable tsLastReport
//uint32_t tsLastReport = 0;

char const * SSID_NAME = "BW"; // Put here your SSID name
char const * SSID_PASS = "12345678R"; // Put here your password
char const * TOKEN = "BBFF-f6mfHdQ7RnaZbX9JVgTBic13CND3yC"; // Put here your TOKEN

char const * HTTPSERVER = "things.ubidots.com";

char const * DEVICE_LABEL = "pulse"; // Put here your device label

/* Put here your variable's labels*/
//char const * VARIABLE_LABEL_1 = "Latitude";
//char const * VARIABLE_LABEL_2 = "Longitude";
char const * VARIABLE_LABEL_3 = "heartrate";
char const * VARIABLE_LABEL_4 = "spo2";
/*char const * VARIABLE_LABEL_5 = "var-test-5";
char const * VARIABLE_LABEL_6 = "var-test-6";
**/




const int HTTPPORT = 80;

char const * USER_AGENT = "ESP8266";
char const * VERSION = "1.0";
char payload[700];
char topic[1000];

WiFiClient clientUbi;
//SoftwareSerial ss(RXPin, TXPin);



// Space to store values to send
//char str_val_ECG[30];
//char str_val_Temp[30];
//char str_val_GPS_lat[30];
//char str_val_GPS_lng[30];
char str_val_BPM[30];
char str_val_oxymeter[30];

//char location[30];
//char lat[30];
//char lng[30];
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
// Callback (registered below) fired when a pulse is detected
void onBeatDetected()
{
    Serial.println("Beat!");
}


/********************************
 * Main Functions
 *******************************/

void setup() {
  Serial.begin(9600);
//  ss.begin(GPSBaud);
  
  /* Connects to AP */
  WiFi.begin(SSID_NAME, SSID_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    }

  WiFi.setAutoReconnect(true);
  Serial.println(F("WiFi connected"));
  Serial.println(F("IP address: "));
  Serial.println(WiFi.localIP());
  // Assign the pin as INPUT 
  //pinMode(14, INPUT); // Setup for leads off detection LO +
 // pinMode(12, INPUT); // Setup for leads off detection LO -
   //mlx.begin();
//   serial_connection.begin(9600);  
    pox.setOnBeatDetectedCallback(onBeatDetected);


}

void loop() {

  /* Adapt your value array following the structure specified at https://ubidots.com/docs/api/index.html#send-values-to-one-variable */
     pox.update();

  /*---- Simulates the values of the sensors -----*/
  //float sensor_value_ECG =analogRead(SENSOR) ;
  //float sensor_value_Temp =mlx.readObjectTempF();
  //float sensor_value_GPS_lat = gps.location.lat();
  //float sensor_value_GPS_lng = gps.location.lng();
  
  float sensor_value_BPM = pox.getHeartRate();
  float sensor_value_oxymeter = pox.getSpO2();
  /*float sensor_value_6 = random(0, 1000)*1.0;
*/
//sprintf(lat)

  /*---- Transforms the values of the sensors to char type -----*/

  /* 4 is mininum width, 2 is precision; float value is copied onto str_val*/
  //dtostrf(sensor_value_ECG, 4, 2, str_val_ECG);
  //dtostrf(sensor_value_Temp, 4, 2, str_val_Temp);
  //dtostrf(sensor_value_GPS_lat, 4, 2, str_val_GPS_lat);
  //dtostrf(sensor_value_GPS_lng, 4, 2, str_val_GPS_lng);
  dtostrf(sensor_value_BPM, 4, 2, str_val_BPM);
  dtostrf(sensor_value_oxymeter, 4, 2, str_val_oxymeter);
  
  /* Builds the payload with structure: {"temperature":25.00,"humidity":50.00} for first 4 variables*/

  // Important: Avoid to send a very long char as it is very memory space costly, send small char arrays
  sprintf(payload, "{\"");
  //sprintf(payload, "%s%s\":%s", payload, VARIABLE_LABEL_1, str_val_GPS_lat);
  //sprintf(payload, "%s,\"%s\":%s", payload, VARIABLE_LABEL_2, str_val_GPS_lng);
  sprintf(payload, "%s,\"%s\":%s", payload, VARIABLE_LABEL_3, str_val_BPM);
  sprintf(payload, "%s,\"%s\":%s", payload, VARIABLE_LABEL_4, str_val_oxymeter);
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
  delay(1000);
}
