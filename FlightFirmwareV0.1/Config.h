#ifndef CONFIG_H_
#define CONFIG_H_


//The Class to handle all configurtion Parameters
//---------------------------------------------------------------------------
//Version: V0.1
//Autor: Tobias Rothlin
//---------------------------------------------------------------------------
//Methods:
//            Config() -> Initalies the defaut values


class Config
{
  public:
    Config()
    {

    }

    double* getServoOffsets()
    {
      return servoOffset;
    }

    void setServoOffset(int index, double offset)
    {
      servoOffset[index] = offset;
    }
    


  private:

    //FLight Controls values:
    const double defaultServoOffset = 90;
    double servoOffset[4] = {defaultServoOffset + 2, defaultServoOffset + 2.5, defaultServoOffset + 8.5, defaultServoOffset + 10};

    
    //Control System: Kp,Ki,Kd
    double pitchK[3] = {0,0,0};
    double rollK[3] = {0,0,0};
    double yawK[3] = {0,0,0};

    //FlightParameters:
    const int numberOfKeyframes = 10;
    
    struct flightParameters{
      unsigned long executionTime;
      double attitude[3];
      double altitude;

    //Wifi Settings:


    //LoggingSettings:
      
    };

    flightParameters FlightKeyFrames[10];
    
    //

};

#endif /*CONFIG_H_*/
