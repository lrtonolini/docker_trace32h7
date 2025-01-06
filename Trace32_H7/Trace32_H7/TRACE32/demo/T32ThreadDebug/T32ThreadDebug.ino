/* This example demonstrates how to include the ThreadMRI library which allows debugging of the Portenta H7
   with GDB via a serial interface (for core M7 only)

   Added by Lauterbach
   - Led blink Red & Green 
   - Example C source code -> sieve_main() in source file Sieve.ino

   This sketch can be debugged using TRACE32 GDB Front-End debugger from Lauterbach
   www.lauterbach.com 
   
   Please read readme.txt for instructions.
*/

#include <ThreadDebug.h>

// Define SerialUSB as serial debug port
// use DEBUG_BREAK_IN_SETUP to break the program at setup() entry, waiting for debugger connection.
// use DEBUG_NO_BREAK_IN_SETUP to let the program start normally, debugger can attach later.  
UsbDebugCommInterface  debugComm(&SerialUSB);
ThreadDebug            threadDebug(&debugComm, DEBUG_BREAK_IN_SETUP);

int Counter=0;

void setup() {
// Setup the Red & Green led pins as OUTPUT
pinMode(LEDR, OUTPUT);
pinMode(LEDG, OUTPUT);

// Switch off Red & Green led
LedRedOFF();
LedGreenOFF();
}

void loop() {
  Counter++;

  LedRedON();
  delay(50);    // wait for 50 ms"                       
  LedRedOFF();
  
  sieve_main();  // Execute C example code 

  LedGreenON();
  delay(100);    // wait for a 100 ms
  LedGreenOFF();

  delay(500);    // wait for a 500 ms 
}


void LedRedON(void) {
  digitalWrite(LEDR, LOW);
}

void LedRedOFF(void) {
  digitalWrite(LEDR, HIGH);
}

void LedGreenON(void) {
  digitalWrite(LEDG, LOW);
}

void LedGreenOFF(void) {
  digitalWrite(LEDG, HIGH);
}
