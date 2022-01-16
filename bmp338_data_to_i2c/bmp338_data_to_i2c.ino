#include <BMP388_DEV.h>                           // sensor library

float temperature, pressure, altitude;            // variables to hold data
BMP388_DEV bmp388;                                // object needed for communication with sensor
//changing the sensor settings is possible, changing them may increase the quality of data 
typedef union //custom data type, used for conversion between float and byte
{
  float przecinkowa;
  uint8_t bitowa[4];
} t;
t tymczasowy;
void setup()
{
  Serial.begin(9600);                           // used only for displaying the data
  bmp388.begin();
  Wire.begin();
}

void loop()
{
  bmp388.startForcedConversion();                 // start the conversation, send data only when requested
  if (bmp388.getMeasurements(temperature, pressure, altitude))    // data request
  {
    Serial.print(temperature);                    //displaying the data
    Serial.print(("*C   "));
    Serial.print(pressure);
    Serial.print(("hPa   "));
    Serial.print(altitude);
    Serial.println(("m"));
  }
  tymczasowy.przecinkowa = temperature; //save as float
  Wire.beginTransmission(9); //ID must match receiver
  for(int x = 0; x < 4; x++)
  {
    Wire.write(tymczasowy.bitowa[x]); //convert to 4 bytes
  }
  tymczasowy.przecinkowa = pressure;
   for(int x = 0; x < 4; x++)
  {
    Wire.write(tymczasowy.bitowa[x]);
  }
  tymczasowy.przecinkowa = altitude;
   for(int x = 0; x < 4; x++)
  {
    Wire.write(tymczasowy.bitowa[x]);
  }
  Wire.endTransmission();
  delay(400); //delay influences the frequency of request
}
