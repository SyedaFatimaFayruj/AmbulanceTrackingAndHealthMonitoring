#include <Adafruit_MLX90614.h>
Adafruit_MLX90614 mlx = Adafruit_MLX90614();


#include <SoftwareSerial.h>
SoftwareSerial espSerial(5, 6);


String str;
void setup(){
Serial.begin(9600);
  espSerial.begin(9600);
  mlx.begin();
  delay(2000);
}

void loop()
{
  float temp = mlx.readObjectTempF();
  
  Serial.print(" T= ");
  Serial.print(temp);
  Serial.println("F");
  str = String(temp);
  espSerial.println(str);
  delay(1000);
}
