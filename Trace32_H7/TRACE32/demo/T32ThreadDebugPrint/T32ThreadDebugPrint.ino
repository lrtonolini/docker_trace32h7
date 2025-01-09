/* This example demonstrates how to include the ThreadMRI library which allows debugging of the Portenta H7
   with GDB via a serial interface (for core M7 only)

   Added by Lauterbach
   - Led blink Red & Green 
   - Example C source code -> sieve_main() in source file Sieve.ino
   - Dual serial port over USB, one serial port for GDB and one serial port for print 
   
   This sketch can be debugged using TRACE32 GDB Front-End debugger from Lauterbach
   www.lauterbach.com 
   
   Please read readme.txt for instructions.
*/

#include <ThreadDebug.h>

// Define SerialUSB1 as serial debug port
// use DEBUG_BREAK_IN_SETUP to break the program at setup() entry, waiting for debugger connection.
// use DEBUG_NO_BREAK_IN_SETUP to let the program start normally, debugger can attach later.  
USBSerial SerialUSB1(false, "DebugPort");
UsbDebugCommInterface  debugComm(&SerialUSB1);
ThreadDebug            threadDebug(&debugComm, DEBUG_BREAK_IN_SETUP); 

int Counter=0;
int blinking = true;

void setup() {
  
//while(!SerialUSB);  // uncomment this to wait for a connected terminal (blocking)
SerialUSB.begin(115200);

// Print board info
PrintBoardInfo();

SerialUSB.println("Hit <space> to toggle Red Led blinking ON/OFF");

// Setup the Red & Green led pins as OUTPUT
pinMode(LEDR, OUTPUT);
pinMode(LEDG, OUTPUT);

// Switch off Red & Green led
LedRedOFF();
LedGreenOFF();
}

void loop() {
  Counter++;
  SerialUSB.print("Loop Counter = ");
  SerialUSB.println(Counter);

  if(blinking==true) {
    LedRedON();
    delay(50);    // wait for 50 ms"                       
    LedRedOFF();
  }
  SerialUSB.println("sieve_main()");
  sieve_main();  // Execute C example code 

  LedGreenON();
  delay(100);    // wait for a 100 ms
  LedGreenOFF();

  if (SerialUSB.available()) {
    char c = SerialUSB.read();
    if (c == ' ')
      if(blinking==true)
      {
        blinking=false;
        SerialUSB.println("Red Led blinking suspended, hit <space> to resume");
      }
      else
      {
        blinking=true;
        SerialUSB.println("Red Led blinking, hit <space> to suspend");
      }
    }


  delay(2000);    // wait for a 2 sec
}


void LedRedON(void) {
  SerialUSB.println("LedRed ON");
  digitalWrite(LEDR, LOW);
}

void LedRedOFF(void) {
  SerialUSB.println("LedRed OFF");
  digitalWrite(LEDR, HIGH);
}

void LedGreenON(void) {
  SerialUSB.println("LedGreen ON");
  digitalWrite(LEDG, LOW);
}

void LedGreenOFF(void) {
  SerialUSB.println("LedGreen OFF");
  digitalWrite(LEDG, HIGH);
}


void PrintBoardInfo()
{
  uint8_t* bootloader_data = (uint8_t*)(0x801F000);

  SerialUSB.println("Portenta H7 board running, Cortex-M7 core.");
  SerialUSB.println("Validation: " + String(bootloader_data[0], HEX));
  SerialUSB.println("BL version: " + String(bootloader_data[1]));
  SerialUSB.println("Clock source: " + String(bootloader_data[2]));
  SerialUSB.println("USB Speed: " + String(bootloader_data[3]));
  SerialUSB.println("Ethernet: " + String(bootloader_data[4]));
  SerialUSB.println("Wifi: " + String(bootloader_data[5]));
  SerialUSB.println("RAM size: " + String(bootloader_data[6]));
  SerialUSB.println("QSPI size: " + String(bootloader_data[7]));
  SerialUSB.println("Video: " + String(bootloader_data[8]));
  SerialUSB.println("Crypto: " + String(bootloader_data[9]));
}
