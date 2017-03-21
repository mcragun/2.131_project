//Touche Sensor implementation in arduino and python
//Information from http://www.instructables.com/id/Touche-for-Arduino-Advanced-touch-sensing/

#include <arduino.h>
#include <CmdMessenger.h>


//                              10n
// PIN 9 --[10k]-+-----10mH---+--||-- OBJECT
//               |            |
//              3.3k          |
//               |            V 1N4148 diode
//              GND           |
//                            |
//Analog 0 ---+------+--------+
//            |      |
//          100pf   1MOmhm
//            |      |
//           GND    GND

//
//Macros to set memory registers on the chip
#define SET(x,y) (x |=(1<<y))				//-Bit set/clear macros
#define CLR(x,y) (x &= (~(1<<y)))       		// |
#define CHK(x,y) (x & (1<<y))           		// |
#define TOG(x,y) (x^=(1<<y))            		//-+

#define N 100  //How many frequencies

float results[N];            //-Filtered result buffer
float freq[N];            //-Filtered result buffer
int sizeOfArray = N;

bool nextSweep = 0; //Control to stop freq Sweep

// Blinking led variables
bool ledState                   = 0;   // Current state of Led
const int kBlinkLed             = 13;  // Pin of internal Led

CmdMessenger cmdMessenger = CmdMessenger(Serial, ',',';','/');

enum
{
    SweepFreqStart,
    SweepFreqStop,
    SetLed,        // Turn LED to State (0,1)
    StatusMsg,        // Return Status of LED
    SendList
};


void SweepFreq(){
  //The main function to sweep the frequency
  //This will generate a list of frequencies as well
  // as a list of responses.
  digitalWrite(kBlinkLed, HIGH); //
  unsigned int d;
  while (nextSweep){
    int counter = 0;
    for(unsigned int d=0;d<N;d++)
    {
      int v=analogRead(0);    //-Read response signal
      CLR(TCCR1B,0);          //-Stop generator
      TCNT1=0;                //-Reload new frequency
      ICR1=d;                 // |
      OCR1A=d/2;              //-+
      SET(TCCR1B,0);          //-Restart generator

      results[d]=results[d]*0.5+(float)(v)*0.5; //Filter results

      freq[d] = d;

   //   plot(v,0);              //-Display
   //   plot(results[d],1);
    // delayMicroseconds(1);
    }

    cmdMessenger.sendCmdStart(SendList);
    for(unsigned int d=0;d<N;d++)
    {
      cmdMessenger.sendCmdBinArg(results[d]);
    }
    cmdMessenger.sendCmdEnd();
    //cmdMessenger.sendCmdStart(SendList);
    //for(unsigned int d=0;d<N;d++)
    //{
    //  cmdMessenger.sendCmdArg(freq[d]);
    //}
    //cmdMessenger.sendCmdEnd();
  }
  //TOG(PORTB,0);            //-Toggle pin 8 after each sweep
  digitalWrite(kBlinkLed, LOW);
}

void on_SweepFreqStart(){
  nextSweep = 1;
  SweepFreq();
}

void on_SweepFreqStop(){
  nextSweep = 0;
}

void on_SetLed(){
  bool ledState = cmdMessenger.readBoolArg();
  //cmdMessenger.sendCmd(StatusMsg,ledState);
  digitalWrite(kBlinkLed, ledState?HIGH:LOW);
  // Send back status that describes the led state
  //cmdMessenger.sendCmd(StatusMsg,(int)ledState);
  cmdMessenger.sendCmdStart(SendList);
  cmdMessenger.sendCmdBinArg(1.0);
  cmdMessenger.sendCmdBinArg(2.1);
  cmdMessenger.sendCmdBinArg(3.2);
  cmdMessenger.sendCmdBinArg(4.3);
  cmdMessenger.sendCmdBinArg(5.5);
  cmdMessenger.sendCmdBinArg(6.5);
  cmdMessenger.sendCmdEnd();
}

// Setup function
void setup()
{
  //Setup the Clock Stuff
  TCCR1A=0b10000010;        //-Set up frequency generator
  TCCR1B=0b00011001;        //-+
  ICR1=110;
  OCR1A=55;

  pinMode(9,OUTPUT);        //-Signal generator pin
  pinMode(8,OUTPUT);        //-Sync (test) pin

  Serial.begin(38400);
  //Serial.println("Hello");
  for(int i=0;i<N;i++)      //-Preset results
    results[i]=0;         //-+


  // Listen on serial connection for messages from the PC
  cmdMessenger.sendCmd(StatusMsg, "Started Serial");
  //Attach any methods for the cmdMessenger
  cmdMessenger.attach(SetLed, on_SetLed);
  cmdMessenger.attach(SweepFreqStart, on_SweepFreqStart);
  cmdMessenger.attach(SweepFreqStop, on_SweepFreqStop);
  // set pin for blink LED
  pinMode(kBlinkLed, OUTPUT);
}

// Loop function
void loop()
{
  // Process incoming serial data, and perform callbacks
  cmdMessenger.feedinSerialData();
}
