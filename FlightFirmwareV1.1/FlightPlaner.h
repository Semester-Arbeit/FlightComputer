#ifndef FLIGHTPLANER_H_
#define FLIGHTPLANER_H_

//The Class to handle the current flight state
//---------------------------------------------------------------------------
//Version: V0.1
//Author: Tobias Rothlin
//---------------------------------------------------------------------------
//Methods:
//

class FlightPlaner
{
  public:

    FlightPlaner(Config* configurationData)
    {
      
    }

    void start(unsigned long currentTime)
    {
      startTime = currentTime;
    }

   



  private:
    unsigned long startTime = 0;
    double* currentFlightParameters = nullptr;
    unsigned long* keyFrameTimeStmps = nullptr;
    double** keyFrameAttitude = nullptr;
};

#endif /*FLIGHTPLANER_H_*/
