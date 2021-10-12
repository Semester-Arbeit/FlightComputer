#ifndef CONTROLSYSTEM_H_
#define CONTROLSYSTEM_H_

#include <PID_v1.h>

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
    void init(double** kValues, double* sensPitch, double* sensRoll, double* sensYaw, double* correctionPitch, double* correctionRoll, double* correctionYaw, double* targetPitch, double* targetRoll, double* targetYaw)
    {
      PIDpitch = new PID(sensPitch, correctionPitch, targetPitch, kValues[0][0], kValues[0][1], kValues[0][2], DIRECT);
      PIDpitch->SetMode(AUTOMATIC);

      PIDroll = new PID(sensRoll, correctionRoll, targetRoll, kValues[1][0], kValues[1][1], kValues[1][2], DIRECT);
      PIDroll->SetMode(AUTOMATIC);

      PIDyaw = new PID(sensYaw, correctionYaw, targetYaw, kValues[2][0], kValues[2][1], kValues[2][2], DIRECT);
      PIDyaw->SetMode(AUTOMATIC);
    }

    void updateValues()
    {
      PIDpitch->Compute();
      PIDroll->Compute();
      PIDyaw->Compute();
    }

    String getStatus()
    {
      String returnString = "";
      returnString += ("Pitch: Kp:" + String(PIDpitch->GetKp()) + "\n   Ki:" + String(PIDpitch->GetKi()) + "\n   Kd:" + String(PIDpitch->GetKd()) + "\n");
      returnString += ("Roll : Kp:" + String(PIDroll->GetKp()) + "\n   Ki:" + String(PIDroll->GetKi()) + "\n   Kd:" + String(PIDroll->GetKd()) + "\n");
      returnString += ("Yaw  : Kp:" + String(PIDyaw->GetKp()) + "\n   Ki:" + String(PIDyaw->GetKi()) + "\n   Kd:" + String(PIDyaw->GetKd()) + "\n");
      return returnString;
    }

  private:
    PID* PIDpitch;
    PID* PIDroll;
    PID* PIDyaw;
};

#endif /*CONTROLSYSTEM_H_*/
