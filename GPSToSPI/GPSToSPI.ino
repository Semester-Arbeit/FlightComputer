#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
#include <SPI.h>

// Connect the GPS Power pin to 5V
// Connect the GPS Ground pin to ground
// Connect the GPS TX (transmit) pin to Digital 8
// Connect the GPS RX (receive) pin to Digital 7

// you can change the pin numbers to match your wiring:
SoftwareSerial mySerial(7, 8);
Adafruit_GPS GPS(&mySerial);

float latitude = 0;
float longitude = 0;

volatile float data [2] = {2.5, 5.6};
volatile byte *pos = (byte*)data;
int i = 0;


void setup()
{
  Serial.begin(115200);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  delay(5000);
  GPS.begin(9600);

  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);


  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate
  // For the parsing code to work nicely and have time to sort thru the data, and
  // print it out we don't suggest using anything higher than 1 Hz
  digitalWrite(4, HIGH);

  pinMode(MISO, OUTPUT);                  //Sets MISO as OUTPUT (Have to Send data to Master IN

  SPCR |= _BV(SPE);                       //Turn on SPI in Slave Mode


  SPI.attachInterrupt();
}

void loop()                     // run over and over again
{
  char c = GPS.read();

  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived())
  {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
    //Serial.println(GPS.lastNMEA());   // this also sets the newNMEAreceived() flag to false

    if (!GPS.parse(GPS.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
      return;  // we can fail to parse a sentence in which case we should just wait for another
    latitude = GPS.latitude;
    longitude = GPS.longitude;
    if (latitude != 0 && longitude != 0)
    {
      data[0] = latitude;
      data[1] = longitude;
      digitalWrite(2, HIGH);
    }
    else
    {
          digitalWrite(2, LOW);
    }

  }

  if (GPS.satellites >= 3)
  {
    digitalWrite(3, HIGH);
  }
  else
  {
    digitalWrite(3, LOW);
  }
}

ISR (SPI_STC_vect)
{
  SPDR = pos[SPDR];
}
