void setup() {
// Open serial communications and wait for port to open:

Serial.begin(115200);
while (!Serial) {
; // wait for serial port to connect. Needed for native USB port only
}
}

void loop() { // run over and over
if (Serial.available()) {
 

  float ind1, ind2;
  String ambient, object;
  String lalala = Serial.readString();
  Serial.println(lalala);
//  delay(2000);
//  ind1 = readString.indexOf(';');  //finds location of first ,
//  ambient = readString.substring(0, ind1);   //captures first data String
//  ind2 = readString.indexOf(':', ind1+1 );   //finds location of second ,
//  object = readString.substring(ind1+1);   //captures second data String
//
// 
//  Serial.print("Ambient = ");
//  Serial.print(ambient); 
//  Serial.print(" Object = ");
//  Serial.println(object);
//  delay(1000);
//  
//  readString=""; //clears variable for new input
//  ambient="";
//  object="";
//Serial.println(Serial2.readString());
}
}

/*char * get_token(char * src, char * delim, char * buf)
{
    buf[0] = '\0';
    if(src==NULL) return NULL;

    int i = 0;
    while( is_white_space(src[i]) || is_delim(src[i], delim) ) i++; //skip initial delim chars or initial whitespaces
    int j = i;
    while( !(is_white_space(src[i]) || is_delim(src[i], delim)) && src[i] )
    {
        buf[i-j] = src[i];
        i++;
    }
    buf[i-j] = '\0';
    while( is_white_space(src[i]) ||  is_delim(src[i], delim) ) i++;

    if(!src[i]) return NULL;
    else return src+i;
}*/
