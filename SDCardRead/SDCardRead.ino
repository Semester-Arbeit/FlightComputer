#include "FileReader.h"

FileReader myFile;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  myFile.init();
  myFile.open("AltitudeProfile.txt");
  Serial.println(myFile.read());
  myFile.close();
  
  Serial.println("Done");
}

void loop() {
  // put your main code here, to run repeatedly:

}
