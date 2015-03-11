/*

Purpose: Create a FIRST Robotics robot sonar accessory that will measure
distance and alert the robot controller that a distance threshold has been sensed.
This accessory will be powered by the robot's 12v bus and its output will be 
connected to the robot controller's GPIO. The output to the robot will be 
active high. The accessory will implement an Arduino Uno that will be packaged
with the Maxbotix sonar module in a single polycarb plastic enclosure.

Maxbotix MB1014 data sheet:
http://www.maxbotix.com/documents/LV-ProxSonar-EZ_Datasheet.pdf

Reads the Maxbotix MB1014 TX output connected to the Arduino soft serial pin 2.
Once the distance data is collected, it is compared to a threshold and 
asserts a logic high to an Arduino digital pin. As seen below, pin 7 has been
chosen for this prototype.
Larry Ryan Feb 2014 - modified Mar 2014

*/

#include <SoftwareSerial.h>  // Key to the success of the Arduino reading the Maxbotix
                             // I couldn't get the hardware Arduino serial to work
#define RXPinA 3   // Maxbotix TX output is connected to the Arduino digital pin 3
#define TXPinA 4  // The Arduino will not send serial data but the SoftwareSerial
                 // constructor requires this parameter.
#define RXPinB 5   // Maxbotix TX output is connected to the Arduino digital pin 3
#define TXPinB 6  // The Arduino will not send serial data but the SoftwareSerial
                 // constructor requires this parameter.
#define MaxbotixBaud 9600    // The Maxbotix default serial bit rate
#define inverted 1 // In case Maxbotix serial is inverted (and, in fact, it is)

//following pins are used to turn on and off ranging on the Maxbotics sensors
#define RangePinA 2  // Maxbotic RX input is connected to the Arduino digital pin 2
#define RangePinB 7  //  Maxbotic RX input is connected to the Arduino digital pin 7

//Following are used to interpret the serial string coming from Maxbotic Sensors
  byte startChar = 'R';           //Maxbotix output string starts with an R
  byte endChar = '\r';           //... and ends with a carriage return
  const int msgLength = 5;  //The Maxbotix puts out an 5 character message
  const int distLength = 3;  //The distance value is 3 char long
  char dist[distLength + 1]; //Holds the distance reading plus a terminating null
  int charsRead = 0;             // Controls the while loop and is initialized to zero
  int i;                                  // The integer version of the inches string
  
  long TimeStamp = 0;
  
// The serial connection to the Maxbotix TX pin:
SoftwareSerial MaxbotixA(RXPinA, TXPinA, inverted);  
SoftwareSerial MaxbotixB(RXPinB, TXPinB, inverted);  
                 //3rd arg of the Software Serial constructor inverts data
                //because it is not standard RS-232 out of the Maxbotix
                // See the Maxbotix 1014 data sheet


void setup()
{
  MaxbotixA.begin(MaxbotixBaud);
  MaxbotixB.begin(MaxbotixBaud);
  Serial.begin(38400);
  pinMode(RangePinA, OUTPUT);
  pinMode(RangePinB, OUTPUT);
  digitalWrite(RangePinA, LOW);
  digitalWrite(RangePinB, LOW);
}

void loop()
{
  digitalWrite(RangePinA, HIGH);  //turn on ranging to the first sensor
  delayMicroseconds(50);    //hold pin high for > 20us to command ranging, see specs sheet
  digitalWrite(RangePinA, LOW);
  
  digitalWrite(RangePinB, HIGH);  //turn on ranging to the first sensor
  delayMicroseconds(50);    //hold pin high for > 20us to command ranging, see specs sheet
  digitalWrite(RangePinB, LOW);
  
  //READ SERIAL DATA FROM FIRST SENSOR//
 
  if (MaxbotixA.available() >= msgLength)   // Whole message received?
  {
    if (MaxbotixA.read() == startChar)     //See an R yet?
    {
      charsRead = 0;
      while (charsRead < distLength)   //Read the 3 char distance value 
      {
       char val = MaxbotixA.read();   // Read one of the 3 char of distance
    
       if ((val == startChar) ||(val == endChar))   // Is val an R or a CR?
          break;                                  // If so, stop the while loop
       dist[charsRead] = val;            // Otherwise read val into the array
       charsRead = charsRead +1;   // Bump charsRead
      }      
       i=atoi(dist); // convert the inches characters to an integer
                     // Thank you Wade Hasbrouck for the help with atoi()
       Serial.print("Sensor A:  ");
       Serial.print(i);
 
    } 
  }
  
  //READ SERIAL DATA FROM SECOND SENSOR//
  if (MaxbotixB.available() >= msgLength)   // Whole message received?
  {
    if (MaxbotixB.read() == startChar)     //See an R yet?
    {
      charsRead = 0;
      while (charsRead < distLength)   //Read the 3 char distance value 
      {
       char val = MaxbotixB.read();   // Read one of the 3 char of distance
    
       if ((val == startChar) ||(val == endChar))   // Is val an R or a CR?
          break;                                  // If so, stop the while loop
       dist[charsRead] = val;            // Otherwise read val into the array
       charsRead = charsRead +1;   // Bump charsRead
      }      
       i=atoi(dist); // convert the inches characters to an integer
                     // Thank you Wade Hasbrouck for the help with atoi()
       Serial.print("Sensor B:  ");
       Serial.print(i);
 
    } 
  }
  
}
