#ifndef FLIGHTPLANER_H_
#define FLIGHTPLANER_H_

//The Class to handle the current flight state
//---------------------------------------------------------------------------
//Version: V0.1
//Autor: Tobias Rothlin
//---------------------------------------------------------------------------
//Methods:
//

class FlightPlaner
{
  public:

    FlightPlaner(Config* configurationData)
    {
      this->keyFrameTimeStmps = configurationData->getFlightKeyFrames_ExecutionTime();
      this->keyFrameAttitude = configurationData->getFlightKeyFrames_attitude();
    }

    void start(unsigned long currentTime)
    {
      startTime = currentTime;
    }

    void updateCurrentFlightParameters(unsigned long currentTime)
    {
      int indexOfCurrentFlightParameter = 0;
      while (keyFrameTimeStmps[indexOfCurrentFlightParameter] < (currentTime-startTime))
      {
        indexOfCurrentFlightParameter++;
      }
      indexOfCurrentFlightParameter--;
      currentFlightParameters = keyFrameAttitude[indexOfCurrentFlightParameter];
    }

    double * getTargetValues()
    {
      return currentFlightParameters;
    }



  private:
    unsigned long startTime = 0;
    double* currentFlightParameters = nullptr;
    unsigned long* keyFrameTimeStmps = nullptr;
    double** keyFrameAttitude = nullptr;
};

#endif /*FLIGHTPLANER_H_*/
