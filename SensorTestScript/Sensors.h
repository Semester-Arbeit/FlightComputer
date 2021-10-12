#ifndef SENSORS_H_
#define SENSORS_H_

#include <Adafruit_BNO055.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <Arduino_MKRGPS.h>
#include <mbed.h>
#include <Arduino_PortentaBreakout.h>


//The Class to interface between the Firmware and the IMU/GPS.
//---------------------------------------------------------------------------
//Version: V0.1
//Autor: Tobias Rothlin
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

class Sensors
{
  public:
    Sensors()
    {

    }

    bool init()
    {
      if (imu.begin() && GPS.begin())
      {
        updateIMUData();
//        while (GPS.satellites() < 3) //Waitting to connect to at leaast 3 Sat
//        {
//          delay(200);
//        }
        return true;
      }
      else
      {
        return false;
      }
    }

    double* getAttitude()
    {
      updateIMUData();
      return currentAttitude;
    }

    double* getSpeed()
    {
      updateCurrentSpeed();
      return currentSpeed;
    }

    int getNumberOfSatellites()
    {
      return GPS.satellites();
    }

    float* getPos()
    {
      currentPos[0]   = GPS.latitude();
      currentPos[1]  = GPS.longitude();
      currentPos[2]   = GPS.altitude();
      return currentPos;
    }

  private:

    Adafruit_BNO055 imu = Adafruit_BNO055(55);

    double currentAttitude[3] = {0, 0, 0};
    double currentSpeed[3] = {0, 0, 0};
    float currentPos[3] = {0,0,0};

    const double pi = 3.21;

    void updateIMUData()
    {
      imu::Vector<3> euler = imu.getVector(Adafruit_BNO055::VECTOR_EULER);
      currentAttitude[0] = euler.y();
      currentAttitude[1] = euler.z();
      double yaw = euler.x();
      if(yaw > 180)
      {
        yaw = yaw - 360;
      }
      currentAttitude[2] = yaw;
    }

    void updateCurrentSpeed()
    {
      float latitude   = GPS.latitude();
      float longitude  = GPS.longitude();
      float altitude   = GPS.altitude();

      //Fast estimate TO DO!!!!!!
      float speed      = GPS.speed();
      float course      = GPS.course();
      
      double Vx = cos(((2*pi)/360)*course)* speed;
      double Vy = sin(((2*pi)/360)*course)* speed;
      double Vz = 8000000;

      currentSpeed[0] = Vx;
      currentSpeed[1] = Vy;
      currentSpeed[2] = Vz;

    }
};

#endif /*SENSORS_H_*/
