#ifndef COMPLEMENTARYFILTER_H_
#define COMPLEMENTARYFILTER_H_


//This class implements a complementaryFilter to estiamte the speed
//---------------------------------------------------------------------------
//Version: V0.1
//Author: Tobias Rothlin
//---------------------------------------------------------------------------
//Methods:
//            Config() -> Initalies the defaut values


class complementaryFilter
{
  public:

    complementaryFilter()
    {
      this->speedEstIntegrated = 0;
      this->firstExecution = 0;
    }

    bool init(double alpha , double staticAccOffset)
    {
      this->alpha = alpha;
      this->staticAccOffset = staticAccOffset;
    }

    double* estimateSpeed(unsigned long* time, double* acc, double* pos)
    {
      if (this->firstExecution > 2)
      {
        double deltaT = (double)(*time - this->lastExecutionTime) / 1000000;
        this->speedEstIntegrated  = this->speedEstIntegrated + (*acc - this->staticAccOffset) * deltaT;
        this->speedEstDifferentiator = (*pos - this->lastPosValue) / deltaT;
        clacAverage(&this->speedEstDifferentiator);
        
        this->speedEst = (1 - this->alpha) * this->speedEstIntegrated + (this->alpha) * average;
      }
      else
      {
        this->firstExecution++;
      }

      this->lastExecutionTime = *time;
      this->lastPosValue = *pos;
      return &speedEst;
    }

    void clacAverage(double* newValue)
    {
      average += (*newValue - ringBuffer[0])/bufferSize;
      for(int i = 1 ; i < bufferSize; i++)
      {
        ringBuffer[i-1] = ringBuffer[i];
      }
      ringBuffer[bufferSize-1] = *newValue;
    }


  private:

    int firstExecution = 0;
    unsigned long lastExecutionTime = 0;
    double alpha = 0;
    double speedEstIntegrated = 0;
    double speedEstDifferentiator = 0;
    double speedEst = 0;
    double lastPosValue = 0;
    double staticAccOffset = 0;
    
    enum FilterConstants{
      bufferSize = 10
    };
    
    double ringBuffer[bufferSize]={0};
    
    

    double average = 0;

};

#endif /*COMPLEMENTARYFILTER_H_*/
