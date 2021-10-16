#ifndef DATALOGGER_H_
#define DATALOGGER_H_

#include "SDMMCBlockDevice.h"
#include "FATFileSystem.h"

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

      int err = fs.mount(&block_device);
      mkdir("fs/LogFiles", 0777);
      if (err == 0)
      {
        return true;
        isMounted = true;
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

    void println(String line)
    {
      if (isMounted)
      {
        String(line + "\r\n").toCharArray(this->lineInFile, 100);
        fprintf(fp, this->lineInFile);
      }
    }

    void close()
    {
      if (isMounted)
      {
        fclose(fp);
      }
    }



  private:
    FILE *fp;
    char fileName[100];
    char lineInFile[100];
    SDMMCBlockDevice block_device;
    mbed::FATFileSystem fs;
    bool isMounted = false;
};

#endif /*DATALOGGER_H_*/
