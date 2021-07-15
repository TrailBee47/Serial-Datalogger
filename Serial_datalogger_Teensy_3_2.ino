
/*
 * Intent: Store Serial data to SD card with time stamp
 * Modules: SD Card 
 *          RTC DS1307 I2C
 *          LED for Status
 *          Button for start/stop logging
 * Functionality:
 *          Every restart will create new log file
 *          As Arduino File class needs to be called with in the same function(some code problem on lib side)
 *          one has to be careful with stopping or interrupting file operations. So we need Start/stop button
 *          Start Stop button will stop file operations and will not add to file
 *          Before pulling card out need to stop the operation by pressing button.
 *          On Restart File Write operation will start automatically(ensures device restart in field will not stop sd card storage)
 *File write format:
 *          TIMESTAMP: AVR/GSM : STRING
 *LED status:
 *          blinking RED, sd card storing is stopped
 *          blinking BLUE, sd card is storing data
 *          blinking GREEN, safe to remove sd card(PRESS button).
*/
#include <Bounce.h>
#include "SdFile.h"

#include <Wire.h>
#include "DS3231.h"

RTClib RTC;

#define GSM_BAUD 115200//9600
#define AVR_BAUD 57600//115200//9600

#define LED_R 16
#define LED_G 14//15
#define LED_B 15
#define BUTTON  17//start/stop pin button press, Note: use some library to control debounce issues

void setup() {
  
  Serial.begin(115200);
    while (!Serial) {
    ; // wait for serial port to connect.
  }
  Wire.begin();
  
  Serial1.begin(AVR_BAUD); 
  Serial3.begin(GSM_BAUD); 

  pinMode(BUTTON,INPUT_PULLUP);

  pinMode(LED_R,OUTPUT);
  pinMode(LED_G,OUTPUT);
  pinMode(LED_B,OUTPUT);

  digitalWrite(LED_R,HIGH);//these leds are inverted
  digitalWrite(LED_G,HIGH);
  digitalWrite(LED_B,HIGH);
   
  initSD();

  //its a restart lets just open a new file
  nameNewFile();
   
   Serial.print("BeeSense Datalogging to SD card\nFilename: ");
   Serial.println(fileName);
//   storeToFile("12:34", "Hello world test", AVR_CHANNEL); //test for SD card
}

void loop() {
  // make a string for assembling the data to log:
 if(digitalRead(BUTTON)==HIGH)
 {
  if(Serial1.available()>0)
  {
   digitalWrite(LED_B,LOW);
   while(Serial1.available()>0)
    {
      String str = Serial1.readStringUntil('\n');
      storeToFile(getTime(), str, AVR_CHANNEL); 
      Serial.println(str);
      delayMicroseconds(10);
    }
    digitalWrite(LED_B,HIGH);
    delay(10);
  }else
   {
    digitalWrite(LED_R,LOW);
    delay(10);
    digitalWrite(LED_R,HIGH);
    delay(100);
   }
  
  if(Serial3.available()>0)
  {
    digitalWrite(LED_B,LOW);
    while(Serial3.available()>0)
    {
      String str = Serial3.readStringUntil('\n');
      storeToFile(getTime(), str, GSM_CHANNEL); 
      Serial.println(str);
      delayMicroseconds(10);
    }
    digitalWrite(LED_B,HIGH);
    delay(10);
  }else
   {
    digitalWrite(LED_R,LOW);
    delay(10);
    digitalWrite(LED_R,HIGH);
    delay(100);
   }
 }//end of trigger
 else if(digitalRead(BUTTON)==LOW)
  {//blink green when its safe to remove SD card
    digitalWrite(LED_G,LOW);
    delay(10);
    digitalWrite(LED_G,HIGH);
    delay(200);
  }
}

String getTime()
{
  String timeStamp = "";
  DateTime now = RTC.now();
    
    timeStamp += String(now.year());
    timeStamp += String('/');
    timeStamp += String(now.month());
    timeStamp += String('/');
    timeStamp += String(now.day());
    timeStamp += String(' ');
    timeStamp += String(now.hour());
    timeStamp += String(':');
    timeStamp += String(now.minute());
    timeStamp += String(':');
    timeStamp += String(now.second());
  return timeStamp;
}
