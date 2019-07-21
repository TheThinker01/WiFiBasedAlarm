// Remotely Generate an alarm using android app and arduino + ESP8266
// Coded by Archisman Pathak
#include <SoftwareSerial.h>
#define RX 10 //Connect TX of wifi module to here
#define TX 11 //Connect RX of wifi module to here
#define ledPin 9
#define LEDFLASH 8
SoftwareSerial esp8266(RX,TX);

char PASS[] = "archis0900";
String nSSID = "DEVICE UNLOCKER";
String PASSW = "1234567890";
String PORT = "8080";
String IP = "192.168.5.1";
String tcpTIMEOUT = "3600";
String GATE = "192.168.5.1";
String SUBNET = "255.255.255.0";

int countTrueCommand;
int countTimeCommand; 
boolean found = false;

int securityUnlock()
{delay(500);
  while(esp8266.available()>0)
    { Serial.println("Reached is Available");
      if(esp8266.find(PASS))//receive password
        {
            Serial.println("DEVICE UNLOCKER FOUND");
            for(int i=0;i<LEDFLASH;i++){
            digitalWrite(ledPin,HIGH);Serial.println("LED ON");
            delay(1000);
            digitalWrite(ledPin,LOW);Serial.println("LED OFF");
            delay(1000);
          }
        return 1; }
    }
  
  return 0;
}

void generateAlarm()
{ espUnlockSetup();
  while(securityUnlock()==0)
  {
    Serial.println("Not Unlocked!!!!"); 
    }
 }
 
void setup() {
  // put your setup code here, to run once:
    Serial.begin(38400);
  esp8266.begin(115200);
  pinMode(ledPin,OUTPUT);
  generateAlarm();
}

void loop() {
  // put your main code here, to run repeatedly:

}

void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while(countTimeCommand < (maxTime*3))//while 1
  {
    esp8266.println(command);//at+cipsend
  
    if(esp8266.find(readReplay))//ok
    {
      found = true;
      
      break;
    }
  
    countTimeCommand++;
  }
  
  if(found == true)
  {
    Serial.println("Done");
    countTrueCommand++;
    countTimeCommand = 0;
  }
  
  if(found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
  
  found = false;
 }

 void espUnlockSetup()  
 {
  sendCommand("AT",5,(char*)"OK");                                         //Set Up The Wifi Module
  sendCommand("AT+CWMODE=2",5,(char*)"OK");                              //Set Up The Wifi Module
  sendCommand("AT+CIPMUX=1",5,(char*)"Link is builded");
  sendCommand("AT+CWSAP=\""+ nSSID +"\",\""+ PASSW +"\","+"5,"+"3",20,(char*)"OK");   //Set Up The Wifi Module
  sendCommand("AT+CIPSERVER=1,"+PORT,5,(char*)"OK");
  sendCommand("AT+CIPSTO="+tcpTIMEOUT,5,(char*)"OK");
  sendCommand("AT+CIPAP=\""+ IP +"\",\""+ GATE +"\",\""+SUBNET+"\"",20,(char*)"OK");   //Set Up The Wifi Module
  sendCommand("AT+CIPDINFO=1",5,(char*)"OK");
 }
