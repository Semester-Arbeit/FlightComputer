#ifndef CONTROLSYSTEM_H_
#define CONTROLSYSTEM_H_

#include "P.h"

//The Class to handle all configurtion Parameters
//---------------------------------------------------------------------------
//Version: V0.1
//Autor: Tobias Rothlin
//---------------------------------------------------------------------------
//Methods:
//            ControlSystem(double** kValues, double** currentValues, double** aleronPosition, double** targetValue) -> Initalies PID Controllers
//                          kValues -> double [[KpPitch, KiPitch, KdPitch],[KpRoll, KiRoll, KdRoll], [KpYaw, KiYaw, Kdyaw]



class ControlSystem
{
  public:
    ControlSystem()
    {

    }
    void init(double* attitude, double* gyro, double* targetAttitude, double* targetPitch, double* )
    {

    }

    void updateValues()
    {
     
    }

    String getStatus()
    {
     
    }

  private:
    P* pPitch;
    P* pGyroX;
    double piResultPitch = 0;

    P* pRoll;
    P* pGyroY;
    double piResultRoll = 0;

    P* pYaw;
    P* pGyroZ;
    double piResultYaw = 0;
    
};

#endif /*CONTROLSYSTEM_H_*/
