/* This example demonstrates how to include the ThreadMRI library which allows debugging of the Portenta H7
   with GDB via a serial interface (for core M7 only)

   Added by Lauterbach
   - Led blink Red & Green 
   - Example C source code -> sieve_main() in source file Sieve.ino
   - T32_Logger functions for TRACE32 debugger, view trace in TRACE32 with Logger command
   
   This sketch can be debugged using TRACE32 GDB Front-End debugger from Lauterbach
   www.lauterbach.com 
   
   Please read readme.txt for instructions.
*/

#include <ThreadDebug.h>
#include <Scheduler.h>
#include "t32logger.h"

// Define SerialUSB as serial debug port
// use DEBUG_BREAK_IN_SETUP to break the program at setup() entry, waiting for debugger connection.
// use DEBUG_NO_BREAK_IN_SETUP to let the program start normally, debugger can attach later.  
UsbDebugCommInterface  debugComm(&SerialUSB);
ThreadDebug            threadDebug(&debugComm, DEBUG_BREAK_IN_SETUP);

// Define T32_Logger, used by TRACE32 by selecting Trace.METHOD LOGGER 
T32_LoggerC T32_Logger;

char SlowCount=0;
int FastCount=0;
int Counter=0;

void setup() {
T32_Logger.Function(&setup);
// Setup the Red & Green led pins as OUTPUT
pinMode(LEDR, OUTPUT);
pinMode(LEDG, OUTPUT);

// Switch off Red & Green led
LedRedOFF();
LedGreenOFF();

// loop() is always started by default
// start additional fast_loop() & slow_loop() threads
Scheduler.startLoop(fast_loop);
Scheduler.startLoop(slow_loop);
}

void loop() {
  T32_Logger.Function(&loop);
  Counter++;
  delay(500);     // wait for 0.5 sec
  
  T32_Logger.Function(&sieve_main);
  sieve_main();   // Execute C example code 
  delay(1500);    // wait for a 1.5 sec 
}


void fast_loop(){
  FastCount++;
  T32_Logger.Function(&fast_loop);
  T32_Logger.Write(&FastCount);

  // show fast_loop() it's running
  LedRedON();
  delay(50);                       
  LedRedOFF();
  delay(500);                   
}

void slow_loop(){
  SlowCount++;
  T32_Logger.Function(&slow_loop);
  T32_Logger.Write(&SlowCount);

  // Show slow_loop() it's running
  LedGreenON();
  delay(100);    // wait for a 100ms
  LedGreenOFF();
  delay(1000);    // wait for a 2000ms 
}


void LedRedON(void) {
  T32_Logger.Function(&LedRedON);
  digitalWrite(LEDR, LOW);
}

void LedRedOFF(void) {
  T32_Logger.Function(&LedRedOFF);
  digitalWrite(LEDR, HIGH);
}

void LedGreenON(void) {
  T32_Logger.Function(&LedGreenON);
  digitalWrite(LEDG, LOW);
}

void LedGreenOFF(void) {
  T32_Logger.Function(&LedGreenOFF);
  digitalWrite(LEDG, HIGH);
}
