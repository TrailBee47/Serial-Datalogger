#include <SD.h>
#include <SPI.h>


//hardware pins
const int CS_PIN = 10;

#define GSM_CHANNEL 1
#define AVR_CHANNEL 2

// Base name must be six characters or less for short file names.
#define FILE_BASE_NAME "RBlog"
const uint8_t BASE_NAME_SIZE = sizeof(FILE_BASE_NAME) - 1;
char fileName[] = FILE_BASE_NAME "000.txt";

void initSD();
void nameNewFile();
void storeToFile();

inline void initSD()
{
  
  if (!SD.begin(CS_PIN)) {
    Serial.println(F("begin failed"));
    return;
  }
}
//creates new File name on Restart
void nameNewFile()
{
  while (SD.exists(fileName)) {
    if (fileName[BASE_NAME_SIZE + 2] != '9') { //last digit
      fileName[BASE_NAME_SIZE + 2]++;
    }else if (fileName[BASE_NAME_SIZE + 1] != '9') {//middle digit
      fileName[BASE_NAME_SIZE + 2] = '0';
      fileName[BASE_NAME_SIZE + 1]++;
    } else if (fileName[BASE_NAME_SIZE] != '9') {//first digit
      fileName[BASE_NAME_SIZE + 1] = '0';
      fileName[BASE_NAME_SIZE]++;
    } else {
      Serial.println(F("Can't create file name"));
      return;
    }
  }
  File file = SD.open(fileName, FILE_WRITE);
  if (!file) {
    Serial.println(F("first open failed"));
    return;
  }
  Serial.print(F("opened: "));
  Serial.println(fileName);
  file.close();
}

void storeToFile(String TimeStamp, String indata, int channel)
{
 File file = SD.open(fileName, FILE_WRITE);

  if (file)//else blink led to denote failing to create file
   {
    file.print("> ");
    file.print(TimeStamp);//store timestamp
    if(channel == GSM_CHANNEL)
      file.print(" GSM\t:");
    else if(channel == AVR_CHANNEL)
      file.print(" AVR\t:");
    file.print(indata); 
   }else
   {//blink led
    Serial.println("error opening datalog.txt");
   }   
   file.close();
}
