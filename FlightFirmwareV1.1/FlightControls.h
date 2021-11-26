#ifndef FLIGHTCONTROLS_H_
#define FLIGHTCONTROLS_H_

#include <Servo.h>
#include <mbed.h>
#include <Arduino_PortentaBreakout.h>
#include "Config.h"

//The Class to interface between the Firmware and Hardware FLight Controls.
//---------------------------------------------------------------------------
//Version: V0.1
//Author: Tobias Rothlin
//---------------------------------------------------------------------------
//Methods:
//            FlightControls() -> Constructor initalises the 5 PWM signals and sets the start values: ESC -> 0
//                                                                                                    Servos -> Custom offset (default 90)
//
//            void enterTransportMode() -> Turns all ailerons to a vertival positio so the do not break during transport.
//
//            bool setAilerons(double pitch, double roll, double yaw) -> Sets the Pitch, roll and yaw angle of the ailerons.
//
//            bool setThrotle(double power) -> sets the Throtle power ( 0 <= power <= 100)
//
//            void setServoNullPos(int ServoIndex, double changeValue) -> changes the offsets of the Servo at ServoIndex)

class FlightControls
{
  public:
    FlightControls(Config* configurationData)
    {
      this->configurationData = configurationData;
      for (int i = 0; i < 4; i++)
      {
        servoFlightControls[i] = Servo();
        servoFlightControls[i].attach(servoPins[i],1000,2000);
        servoFlightControls[i].write(0.9*configurationData->getServoOffsets()[i]+ 46.935);
      }
      ESC.attach(MotorPin,1000,2000);
      MotorEnable.attach(MotorEnablePin,1000,2000);
      ESC.write(0.9*0 + 46.935);
      MotorEnable.write(0.9*0 + 46.935);
    }

    void enterTransportMode()
    {
      for (int i = 0; i < 4; i++)
      {
        servoFlightControls[i].write(0.9 * ((configurationData->getServoOffsets()[i] - 90)) + 46.935);
      }
      ESC.write(0.9*0 + 46.935);
    }

    void startMotor()
    {
      setAilerons(0, 0, 0);
      MotorEnable.write(0.9*100 + 46.935);
      delay(1000);
      for ( int i = 0; i < 30; i++)
      {
        ESC.write(0.9*i + 46.935);
        delay(10);
      }
    }

    void stopMotor()
    {
      MotorEnable.write(0.9*0 + 46.935);
      enterTransportMode();
    }

    void setThrotle(double power)
    {
      if (power < 0)
      {
        ESC.write(0.9*0 + 46.935);
      }
      else if (power > 100)
      {
        ESC.write(0.9*100 + 46.935);
      }
      else
      {
        ESC.write(0.9*power + 46.935);
      }
    }

    bool setAilerons(double pitch, double roll, double yaw)
    {
      // Pitch, Roll, Yaw setup as seen from the top of the Vehicle.
      //
      //                      rollXp
      //                 , - ~ ~ ~ - ,                    ^ x
      //             , '               ' ,                |
      //           ,                       ,              |
      //          ,                         ,             |
      //         ,                           ,            |
      // pitchYn,                             ,  pitchYP  |
      //         ,                           ,            |________________>  y
      //          ,                         ,
      //           ,                       ,
      //             ,                  , '
      //               ' - , _ _ _ ,  '
      //                    rollXn
      //
      // the yaw value is added to all 4 Servos values.
      // if the final servo Value is out of bounds the function will prioritise the yaw value and neglegt the overflow and return a false value!

      double pitchYn = yaw - pitch;
      double pitchYp = yaw + pitch;

      double rollXp = yaw + roll;
      double rollXn = yaw - roll;

      bool maxReached = false;

      if (pitchYn * pitchYn > squareStallAngle)
      {
        maxReached = true;
        if (pitchYn > 0)
        {
          pitchYn = stallAngle;
        }
        else
        {
          pitchYn = -stallAngle;
        }
      }

      if (pitchYp * pitchYp > squareStallAngle)
      {
        maxReached = true;
        if (pitchYp > 0)
        {
          pitchYp = stallAngle;
        }
        else
        {
          pitchYp = -stallAngle;
        }
      }

      if (rollXp * rollXp > squareStallAngle)
      {
        maxReached = true;
        if (rollXp > 0)
        {
          rollXp = stallAngle;
        }
        else
        {
          rollXp = -stallAngle;
        }
      }

      if (rollXn * rollXn > squareStallAngle)
      {
        maxReached = true;
        if (rollXn > 0)
        {
          rollXn = stallAngle;
        }
        else
        {
          rollXn = -stallAngle;
        }
      }

      servoFlightControls[0].write(0.9 * (configurationData->getServoOffsets()[0] - rollXn) + 46.935);
      servoFlightControls[1].write(0.9 * (configurationData->getServoOffsets()[1] - pitchYn) + 46.935);
      servoFlightControls[2].write(0.9 * (configurationData->getServoOffsets()[2] - rollXp) + 46.935);
      servoFlightControls[3].write(0.9 * (configurationData->getServoOffsets()[3] - pitchYp) + 46.935);
      return !maxReached;
    }

    double* setServoNullPos(int ServoIndex, double changeValue)
    {
      if (escPower == 0 && ServoIndex < 4)
      {
        configurationData->setServoOffset(ServoIndex, configurationData->getServoOffsets()[ServoIndex] + changeValue);
        servoFlightControls[ServoIndex].write(configurationData->getServoOffsets()[ServoIndex]);
      }
      return configurationData->getServoOffsets();
    }

    void restAilerons()
    {
      setAilerons(0, 0, 0);
    }

    bool testAilerons()
    {
      setAilerons(20, 0, 0);
      delay(500);
      setAilerons(-20, 0, 0);
      delay(500);
      setAilerons(0, 20, 0);
      delay(500);
      setAilerons(0, -20, 0);
      delay(500);
      setAilerons(0, 0, 20);
      delay(500);

      setAilerons(20, 0, 20);
      delay(250);
      setAilerons(-20, 0, -20);
      delay(250);
      setAilerons(20, 0, -20);
      delay(250);
      setAilerons(-20, 0, 20);

      delay(250);
      setAilerons(0, 20, 20);
      delay(250);
      setAilerons(0, -20, -20);
      delay(250);
      setAilerons(0, -20, 20);
      delay(250);
      setAilerons(0, 20, -20);
      delay(250);

      restAilerons();
      return true;
    }

  private:
    const double stallAngle = 25;
    const double squareStallAngle = stallAngle * stallAngle; //used to decide if the courrent Angle is greater than the stall angle.

    Config* configurationData = nullptr;
    
    double escPower = 0;

    breakoutPin MotorPin = PWM5;
    breakoutPin MotorEnablePin = PWM0;
    breakoutPin servoPins[4] = {PWM1, PWM2, PWM3, PWM4};

    Servo servoFlightControls[4];
    Servo ESC;
    Servo MotorEnable;

};

#endif /*FLIGHTCONTROLS_H_*/
