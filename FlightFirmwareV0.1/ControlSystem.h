#ifndef CONTROLSYSTEM_H_
#define CONTROLSYSTEM_H_

#include "P.h"

//The Class to handle all configuration Parameters
//---------------------------------------------------------------------------
//Version: V0.1
//Author: Tobias Rothlin
//---------------------------------------------------------------------------
//Methods:
//            ControlSystem(double** kValues, double** currentValues, double** aleronPosition, double** targetValue) . Initalies PID Controllers
//                          kValues . double [[KpPitch, KiPitch, KdPitch],[KpRoll, KiRoll, KdRoll], [KpYaw, KiYaw, Kdyaw]



class ControlSystem
{
  public:
    ControlSystem()
    {

    }
    
    void init(double** attitude, double** gyro, double** position, double** speed, double** targetAttitude, double** targetGyro, double** targetPosition, double** targetSpeed, double* outputPitch, double* outputRoll, double* outputYaw, double* outputPower, double** kAtt, double** kGyro, double** kPos, double** kSpeed)
    {
      pPitch.init(&pOutPitch, attitude[0], targetAttitude[0], kAtt[0]);
      pRoll.init(&pOutRoll, attitude[1], targetAttitude[1], kAtt[1]);
      pYaw.init(&pOutYaw, attitude[2], targetAttitude[2], kAtt[2]);

      pGyroX.init(&pOutGyroX, gyro[0], targetGyro[0], kGyro[0]);
      pGyroY.init(&pOutGyroY, gyro[1], targetGyro[1], kGyro[1]);
      pGyroZ.init(&pOutGyroZ, gyro[2], targetGyro[2], kGyro[2]);

      pPosX.init(&pOutPosX, position[0], targetPosition[0], kPos[0]);
      pPosY.init(&pOutPosY, position[1], targetPosition[1], kPos[1]);
      pPosZ.init(&pOutPosZ, position[2], targetPosition[2], kPos[2]);

      pSpeedX . init(&pOutSpeedX, speed[0], targetSpeed[0], kSpeed[0]);
      pSpeedY . init(&pOutSpeedY, speed[1], targetSpeed[1], kSpeed[1]);
      pSpeedZ . init(&pOutSpeedZ, speed[2], targetSpeed[2], kSpeed[2]);

      driverOutputPitch = outputPitch;
      driverOutputRoll = outputRoll;
      driverOutputYaw = outputYaw;
      driverOutPower = outputPower;
    }

    void updateValues()
    {
      pPitch.updateValues();
      pRoll.updateValues();
      pYaw.updateValues();

      pGyroX.updateValues();
      pGyroY.updateValues();
      pGyroZ.updateValues();

      pPosX.updateValues();
      pPosY.updateValues();
      pPosZ.updateValues();

      pSpeedX.updateValues();
      pSpeedY.updateValues();
      pSpeedZ.updateValues();

      *driverOutputPitch = pOutPitch + pOutGyroX + pOutPosX + pOutSpeedX;
      *driverOutputRoll = pOutRoll + pOutGyroY + pOutPosY + pOutSpeedY;
      *driverOutputYaw = pOutYaw + pOutGyroZ;
      *driverOutPower = pOutPosZ + pOutSpeedZ;
    }

    String getStatus()
    {
      String status = "ControlSystem: Status Report \n";
      status += "Pitch I:" + pPitch.getStatus();
      status += "Pitch P:" + pGyroX.getStatus();
      status += "--------------------------------\n";
      status += "Roll I:" + pRoll.getStatus();
      status += "Roll P:" + pGyroY.getStatus();
      status += "--------------------------------\n";
      status += "Yaw I:" + pYaw.getStatus();
      status += "Yaw P:" + pGyroZ.getStatus();
      status += "--------------------------------\n";
      status += "Pitch I:" + pPosX.getStatus();
      status += "Pitch P:" + pSpeedX.getStatus();
      status += "--------------------------------\n";
      status += "Pitch I:" + pPosY.getStatus();
      status += "Pitch P:" + pSpeedY.getStatus();
      status += "--------------------------------\n";
      status += "Pitch I:" + pPosZ.getStatus();
      status += "Pitch P:" + pSpeedZ.getStatus();
      status += "--------------------------------\n";
      return status;
    }

  private:
    P pPitch = P();
    P pGyroX = P();
    double pOutPitch = 0;
    double pOutGyroX = 0;

    P pRoll = P();
    P pGyroY = P();
    double pOutRoll = 0;
    double pOutGyroY = 0;

    P pYaw = P();
    P pGyroZ = P();
    double pOutYaw = 0;
    double pOutGyroZ = 0;

    P pPosX = P();
    P pSpeedX = P();
    double pOutPosX = 0;
    double pOutSpeedX = 0;

    P pPosY = P();
    P pSpeedY = P();
    double pOutPosY = 0;
    double pOutSpeedY = 0;

    P pPosZ = P();
    P pSpeedZ = P();
    double pOutPosZ = 0;
    double pOutSpeedZ = 0;

    double* driverOutputPitch;
    double* driverOutputRoll;
    double* driverOutputYaw;
    double* driverOutPower;

};

#endif /*CONTROLSYSTEM_H_*/
