#ifndef FILEREADER_H_
#define FILEREADER_H_

#include "SDMMCBlockDevice.h"
#include "FATFileSystem.h"

//The Class to handle the data logging
//---------------------------------------------------------------------------
//Version: V0.1
//Author: Tobias Rothlin
//---------------------------------------------------------------------------
//Methods:
//

class FileReader
{
  public:

    FileReader()
      : fs("fs")
    {

    }

    bool init()
    {
      int err = fs.mount(&block_device);
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
        fp = fopen("fs/AltitudeProfile.txt", "r");
      }
    }

    char read()
    {
      char c = fgetc(fp);
      return c;
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

#endif /*FILEREADER_H_*/
