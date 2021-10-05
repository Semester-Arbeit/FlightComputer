#ifndef CONTROLSYSTEM_H_
#define CONTROLSYSTEM_H_

#include <PID_v1.h>

//The Class to handle all configurtion Parameters
//---------------------------------------------------------------------------
//Version: V0.1
//Autor: Tobias Rothlin
//---------------------------------------------------------------------------
//Methods:
//            ControlSystem(double** kValues, double** currentValues, double** aleronPoition, double** targetValue) -> Initalies PID Controllers
//                          kValues -> double [[KpPitch, KiPitch, KdPitch],[KpRoll, KiRoll, KdRoll], [KpYaw, KiYaw, Kdyaw]
//                          currentValues -> double [&pitch,&roll,&yaw]
//                          aleronPosition -> double [&pitch,&roll,&yaw]
//                          targetValues -> double [&pitch,&roll,&yaw]


class ControlSystem
{
  public:
    ControlSystem(double** kValues, double** currentValues, double** aleronPoition, double** targetValue)
    {
      for( int i = 0; i < 3; i++)
      {
        controlInstances[i] = PID(currentValues[i],aleronPoition[i],targetValue[i],kValues[i][0],kValues[i][1],,kValues[i][2], DIRECT);
        controlInstances[i].SetMode(AUTOMATIC);
      }
    }

    updateValues()
    {
      for( int i = 0; i < 3; i++)
      {
        controlInstances[i]..Compute();
      }
    }
    
  private:
    PID controlInstances[3];
};

#endif /*CONTROLSYSTEM_H_*/
