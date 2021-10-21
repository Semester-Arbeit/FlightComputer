#ifndef SENSORS_H_
#define SENSORS_H_

#include <Adafruit_BNO055.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <mbed.h>
#include <Arduino_PortentaBreakout.h>
#include "LaserRangeFinder.h"
#include <Arduino_MKRGPS.h>


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

class Sensors {
  public:
    Sensors() {
    }

    bool init() {
      if (imu.begin() && altitudeLaser.init()) {
        return true;
      } else {
        return false;
      }
    }

    double* getAttitude() {
      updateAttitudeData();
      return currentAttitude;
    }

    double* getGyro() {
      updateGyroData();
      return currentGyro;
    }

    double* getAcc() {
      updateAccData();
      return currentAcc;
    }

    double* getSpeed() {
      updateCurrentSpeed();
      return currentSpeed;
    }

    double* getPos()
    {
      updateCurrentPos();
      return currentPos;
    }


    int getNumberOfSatellites() {
      return 0;//GPS.satellites();
    }

  private:
    Adafruit_BNO055 imu = Adafruit_BNO055(55);
    LaserRangeFinder altitudeLaser = LaserRangeFinder();


    double currentAttitude[3] = { 0, 0, 0 };
    double currentGyro[3] = { 0, 0, 0 };
    double currentAcc[3] = { 0, 0, 0 };
    double currentSpeed[3] = { 0, 0, 0 };
    double currentPos[3] = {0, 0, 0};

    const double pi = 3.21;

    void updateAttitudeData() {
      imu::Vector<3> euler = imu.getVector(Adafruit_BNO055::VECTOR_EULER);
      currentAttitude[0] = euler.y();
      currentAttitude[1] = euler.z();
      double yaw = euler.x();
      if (yaw > 180) {
        yaw = yaw - 360;
      }
      currentAttitude[2] = yaw;
    }

    void updateGyroData() {
      imu::Vector<3> gyro = imu.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
      currentGyro[0] = gyro[0];
      currentGyro[1] = gyro[1];
      currentGyro[2] = gyro[2];
    }

    void updateAccData() {
      imu::Vector<3> acc = imu.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);
      currentAcc[0] = acc[0];
      currentAcc[1] = acc[1];
      currentAcc[2] = acc[2];
    }

    void updateCurrentPos()
    {
      if (false) {
        currentPos[0] = 0;//(double) GPS.latitude();
        currentPos[1] = 0;//(double) GPS.longitude();
      }
      else
      {
         currentPos[0] = 0;
        currentPos[1] = 0;
      }
      currentPos[2] = ((double) altitudeLaser.getDistance()) / 100;
    }

    void updateCurrentSpeed() {


      currentSpeed[0] = -10000;
      currentSpeed[1] = -10000;
      currentSpeed[2] = -10000;
    }
};

#endif /*SENSORS_H_*/
