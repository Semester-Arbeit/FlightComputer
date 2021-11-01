#ifndef DATALOGGER_H_
#define DATALOGGER_H_

#include "SDMMCBlockDevice.h"
#include "FATFileSystem.h"

#include <SDRAM.h>

//The Class to handle the data logging
//---------------------------------------------------------------------------
//Version: V0.1
//Author: Tobias Rothlin
//---------------------------------------------------------------------------
//Methods:
//

class DataLogger
{
  public:

    DataLogger()
      : fs("fs")
    {

    }

    bool init()
    {
      SDRAM.begin();
      myVeryBigArray = (char*)SDRAM.malloc(500000);
      int err = fs.mount(&block_device);
      mkdir("fs/LogFiles", 0777);
      if (err == 0)
      {
        isMounted = true;
        return true;
      }
      else
      {
        return false;
      }
    }

    void open(String fileName)
    {
      if (isMounted)
      {
        String("fs/LogFiles/" + fileName).toCharArray(this->fileName, 100);
        fp = fopen(this->fileName, "w");
      }
    }

    int println(char* line)
    {
      for (int i = 0; i < 98; i++)
      {
        myVeryBigArray[lineInArray * 100 + i] = line[i];
      }
      myVeryBigArray[lineInArray * 100 + 98] = '\r';
      myVeryBigArray[lineInArray * 100 + 99] = '\n';
      lineInArray++;
      return lineInArray-1;
    }

    void printToFile(int line)
    {
      for(int m = 0; m < 100; m++)
      {
        fprintf(fp, "%c",myVeryBigArray[line * 100 + m]);
      }
      
      
    }

    void close()
    {
      if (isMounted)
      {
        for (int i = 0; i < 5000; i++)
        {
          printToFile(i);
        }
        fclose(fp);
      }
    }



  private:

    FILE *fp;

    int lineInArray = 0;
    char* myVeryBigArray;
    char fileName[100];
    char lineInFile[100];
    SDMMCBlockDevice block_device;
    mbed::FATFileSystem fs;
    bool isMounted = false;


};

#endif /*DATALOGGER_H_*/
