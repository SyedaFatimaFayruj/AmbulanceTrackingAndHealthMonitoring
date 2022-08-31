#include <ESP8266WiFi.h>
#include <TinyGPS++.h>
#include <WiFiClient.h>
#include <SoftwareSerial.h>
#include <ESP8266WebServer.h>

 
        unsigned long start;
        double lat_val=0, lng_val=0, alt_m_val=0;
        uint8_t hr_val=0, min_val=0, sec_val=0;
        bool loc_valid, alt_valid, time_valid;
        String lat_val_s = "";
        String lng_val_s = "";
        


TinyGPSPlus gps;
SoftwareSerial GPS_SoftSerial(4, 5); 

float Latitude , Longitude;
int year , month , date, hour , minute , second;
String DateString , TimeString , LatitudeString , LongitudeString;

volatile float minutes, seconds;
volatile int degree, secs, mins;

double i=1;

// Replace with your network credentials
const char* ssid = "BW"; //Enter Wi-Fi SSID
const char* password =  "12345678R"; //Enter Wi-Fi Password

ESP8266WebServer server(80);   //instantiate server at port 80 (http port)

String page = "";
String text = "";
String text2 = "";
double data;
void setup(void) {
  
  delay(1000);
  Serial.begin(115200);
  
  GPS_SoftSerial.begin(9600);
  WiFi.begin(ssid, password); //begin WiFi connection
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  server.on("/data.txt", []() {
       
    text = "LONG: "+  lng_val_s +"<p>" +" LAT: " +lat_val_s +"</p>";
    text = text + "<p>"+"Runtime: "+(String)data + "</p>"+"<p>"+ "<a style=""color:RED;font-size:125%;"" href=""http://maps.google.com/maps?&z=15&mrt=yp&t=k&q=";
    text = text +lat_val_s;
    text = text +"+";
    text = text +lng_val_s;
    text = text +""" target=""_top"">Click here</a> to open the location in Google Maps.</p>";
    text = text + "<p> </p>";
    text = text + "<div class=""gmap_canvas"">";
    text = text + "<iframe width=""8000"" height=""600"" id=""gmap_canvas"" s";
    text = text + "rc=""http://maps.google.com/maps?&z=15&mrt=yp&t=k&q=";
    text = text +lat_val_s;
    text = text +"+";
    text = text +lng_val_s;
    
    text = text + "&ie=UTF8&iwloc=&output=embed"" frameborder=""0"" scrolling=""no"" marginheight=""0"" marginwidth=""0""></iframe><a href=""https://www.embedgooglemap.net"">embedding google maps on website</a><style>.gmap_canvas {overflow:hidden;background:none!important;height:500px;width:600px;}</style></div>";

    
    server.send(200, "text/html", text);
    
  });
  server.on("/", []() {
    page = "<h1>EEE416 Project</h1><h1>AMBULANCE TRACKING WITH PATIENT HEALTH MONITORING SYSTEM​</h1> <h1 id=\"data\">""</h1>\r\n";
    page += "<script>\r\n";
    page += "var x = setInterval(function() {loadData(\"data.txt\",updateData)}, 2000);\r\n";
    page += "function loadData(url, callback){\r\n";
    page += "var xhttp = new XMLHttpRequest();\r\n";
    page += "xhttp.onreadystatechange = function(){\r\n";
    page += " if(this.readyState == 4 && this.status == 200){\r\n";
    page += " callback.apply(xhttp);\r\n";
    page += " }\r\n";
    page += "};\r\n";
    page += "xhttp.open(\"GET\", url, true);\r\n";
    page += "xhttp.send();\r\n";
    page += "}\r\n";
    page += "function updateData(){\r\n";
    page += " document.getElementById(\"data\").innerHTML = this.responseText;\r\n";
    page += "}\r\n";
    page += "</script>\r\n";
    server.send(200, "text/html", page);
  });

  server.begin();
  Serial.println("Web server started!");
}

void loop(void) {

/////GPS
        smartDelay(1000); /* Generate precise delay of 1ms */

        lat_val = gps.location.lat(); /* Get latitude data */
        lat_val_s = String(lat_val, 6);
        loc_valid = gps.location.isValid(); /* Check if valid location data is available */
        lng_val = gps.location.lng();/* Get longtitude data */
        lng_val_s = String(lng_val, 6);
        alt_m_val = gps.altitude.meters();  /* Get altitude data in meters */
        alt_valid = gps.altitude.isValid(); /* Check if valid altitude data is available */
        hr_val = gps.time.hour(); /* Get hour */
        min_val = gps.time.minute();  /* Get minutes */
        sec_val = gps.time.second();  /* Get seconds */
        time_valid = gps.time.isValid();  /* Check if valid time data is available */
        if (!loc_valid)
        {          
          Serial.print("Latitude : ");
          Serial.println("*****");
          
          Serial.print("Longitude : ");
          Serial.println("*****");
        }
        else
        {
          DegMinSec(lat_val);
          Serial.print("Latitude in Decimal Degrees : ");
          Serial.println(lat_val, 6);
          Serial.print("Latitude in Degrees Minutes Seconds : ");
          Serial.print(degree);
          Serial.print("\t");
          Serial.print(mins);
          Serial.print("\t");
          Serial.println(secs);
          DegMinSec(lng_val); /* Convert the decimal degree value into degrees minutes seconds form */
          Serial.print("Longitude in Decimal Degrees : ");
          Serial.println(lng_val, 6);
          Serial.print("Longitude in Degrees Minutes Seconds : ");
          Serial.print(degree);
          Serial.print("\t");
          Serial.print(mins);
          Serial.print("\t");
          Serial.println(secs);
        }
        if (!alt_valid)
        {
          Serial.print("Altitude : ");
          Serial.println("*****");
        }
        else
        {
          Serial.print("Altitude : ");
          Serial.println(alt_m_val, 6);    
        }
        if (!time_valid)
        {
          Serial.print("Time : ");
          Serial.println("*****");
        }
        else
        {
          char time_string[32];
          sprintf(time_string, "Time : %02d/%02d/%02d \n", hr_val, min_val, sec_val);
          Serial.print(time_string);    
        }




    data = i;
    i = i+1;
  
  delay(5000);
  server.handleClient();
}

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (GPS_SoftSerial.available())  /* Encode data read from GPS while data is available on serial port */
      gps.encode(GPS_SoftSerial.read());
/* Encode basically is used to parse the string received by the GPS and to store it in a buffer so that information can be extracted from it */
  } while (millis() - start < ms);
}

void DegMinSec( double tot_val)   /* Convert data in decimal degrees into degrees minutes seconds form */
{  
  degree = (int)tot_val;
  minutes = tot_val - degree;
  seconds = 60 * minutes;
  minutes = (int)seconds;
  mins = (int)minutes;
  seconds = seconds - minutes;
  seconds = 60 * seconds;
  secs = (int)seconds;
}
