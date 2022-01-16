
#include <CanSatKit.h>
#include <Wire.h>

using namespace CanSatKit;
bool new_data = false;
int counter = 1;
bool led_state = false;
const int led_pin = 13;
typedef union //needed for data type conversion (4 bytes to float)
{
  float przecinkowa;
  uint8_t bitowa[4];
} suka;
suka pressure, temperature, altitude;

Radio radio(Pins::Radio::ChipSelect,
            Pins::Radio::DIO0,
            433.0,
            Bandwidth_125000_Hz,
            SpreadingFactor_9,
            CodingRate_4_8);

// create (empty) radio frame object that can store data
// to be sent via radio
int data[12];

Frame frame;

void setup() {
  SerialUSB.begin(115200);
  pinMode(led_pin, OUTPUT);
  Wire.begin(9);
  Wire.onReceive(receiveEvent);
  radio.begin();
}


void loop() {
  digitalWrite(led_pin, led_state);

  delay(20);
  if (new_data)
  {
    new_data = false;
    // store ("print") value of counter variable into radio frame
    frame.print("T");
    frame.println(temperature.przecinkowa);
    frame.print("P");
    frame.println(pressure.przecinkowa);
    frame.print("A");
    frame.println(altitude.przecinkowa);
    SerialUSB.println(frame);
    radio.transmit(frame);  
    led_state = !led_state; //change state every send
    frame.clear();
  }

  // clear frame to make it ready for new data
  //frame.clear();

  // wait for 1 s
  delay(50);
}
void receiveEvent(int how_many) // executed on interupt, can't be too large,
{                               // if extra data is received a new solution may need to be implemented
  int x = 0;
  while (Wire.available() > 0)
  {
    if (x > 12)
    {
      return;
    }
    if (x >= 8)
    {
      altitude.bitowa[x % 4] = Wire.read();
    }
    else if (x >= 4)
    {
      pressure.bitowa[x % 4] = Wire.read();
    }
    else if (x >= 0)
    {
      temperature.bitowa[x % 4] = Wire.read();
    }
    x++;
  }

  new_data = true;
}
