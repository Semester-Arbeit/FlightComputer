#include "DataLogger.h"

DataLogger myDataLogger;

void setup() {
  Serial.begin(9600);
  delay(2500);
  Serial.println("Start");
  myDataLogger.init();
  for(int i = 0; i < 5000; i++)
  {
    char line[100] = "9111,-1.11,11.00,-1.11,-0.11,-1.00,1.11,-0.01,-0.11,-0.01,0.00,0.00,1.19";
    Serial.println(myDataLogger.println(line));
  }
  myDataLogger.open("test.csv");
  myDataLogger.close();
  Serial.println("Done");
}

void loop() {
  // put your main code here, to run repeatedly:

}
