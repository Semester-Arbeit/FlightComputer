#ifndef SPIGPS_H_
#define SPIGPS_H_

#include <SPI.h>


//The Class to interface between the Firmware and the IMU/GPS.
//---------------------------------------------------------------------------
//Version: V0.1
//Author: Tobias Rothlin
//---------------------------------------------------------------------------
//Methods:
//            Sensors() -> Constructor Does nothing.
//
//            bool init() -> initalises the GPS and IMU will not return until the GPS is read to use!
//                           Returns True if both Sensors work else it will return False
//
//            double* getAttitude() -> Returns the current Attitude as a double Array(pitch,roll,yaw)
//
//            double* getSpeed() -> Returns the current Speed from the GPS as a double Array(Vx,Vy,Vz)

class SPIGPS {
  public:
    SPIGPS() {
    }

    bool init()
    {
      digitalWrite(SS, HIGH);  // ensure SS stays high for now
      SPI.begin ();
      return true;
    }

    double* getPos()
    {
      digitalWrite(SS, LOW);    // SS is pin 10
      SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));

      SPI.transfer(0);

      for (int i = 1; i <= 8; i++)
      {
        pos[i - 1] = SPI.transfer(i);
      }

      if(buf[0] > 1 && buf[1] > 1)
      {
        positionValues[0]= buf[0];
        positionValues[1] = buf[1];
      }
      return positionValues;
    }

  private:
    volatile float buf [2];
    volatile byte *pos = (byte*)buf;
    double positionValues[2];


};

#endif /*SPIGPS_H_*/
