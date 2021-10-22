#ifndef LASERANGEFINDER_H_
#define LASERANGEFINDER_H_

#include <mbed.h>
#include <Arduino_PortentaBreakout.h>
#include <Wire.h>

//The Class to handle the data logging
//---------------------------------------------------------------------------
//Version: V0.1
//Author: Tobias Rothlin
//---------------------------------------------------------------------------
//Methods:
//

class LaserRangeFinder
{
  public:

    LaserRangeFinder()
    {

    }

    bool init()
    {
      Wire.begin();
      write(0x02, 0x80, 0x62);
      write(0x04, 0x08, 0x62);
      write(0x1c, 0x00, 0x62);
      delay(1500);
      return true;
    }

    int getDistance()
    {
      write(0x00, 0x04, 0x62);
      byte distanceArray[2];
      // Read two bytes from register 0x8f (autoincrement for reading 0x0f and 0x10)
      read(0x8f, 2, distanceArray, true, 0x62);
      // Shift high byte and add to low byte
      int distance = (distanceArray[0] << 8) + distanceArray[1];
      return distance;
    }


    void write(char myAddress, char myValue, char lidarliteAddress)
    {
      Wire.beginTransmission((int)lidarliteAddress);
      Wire.write((int)myAddress);
      Wire.write((int)myValue);

      int nackCatcher = Wire.endTransmission();
      if (nackCatcher != 0)
      {
        Serial.println("> nack");
      }

      delay(1);
    }


    void read(char myAddress, int numOfBytes, byte arrayToSave[2], bool monitorBusyFlag, char lidarliteAddress)
    {
      int busyFlag = 0;
      if (monitorBusyFlag)
      {
        busyFlag = 1;
      }
      int busyCounter = 0;

      while (busyFlag != 0)
      {

        Wire.beginTransmission((int)lidarliteAddress);
        Wire.write(0x01);

        int nackCatcher = Wire.endTransmission();

        Wire.requestFrom((int)lidarliteAddress, 1);
        busyFlag = bitRead(Wire.read(), 0);

        busyCounter++;

        if (busyCounter > 9999)
        {
          goto bailout;
        }
      }


      if (busyFlag == 0)
      {
        Wire.beginTransmission((int)lidarliteAddress);
        Wire.write((int)myAddress);

        int nackCatcher = Wire.endTransmission();

        Wire.requestFrom((int)lidarliteAddress, numOfBytes);
        int i = 0;
        if (numOfBytes <= Wire.available())
        {
          while (i < numOfBytes)
          {
            arrayToSave[i] = Wire.read();
            i++;
          }
        }
      }


      if (busyCounter > 9999)
      {
bailout:
        busyCounter = 0;
      }
    } /* LIDARLite::read */



  private:

};

#endif /*LASERANGEFINDER_H_*/
