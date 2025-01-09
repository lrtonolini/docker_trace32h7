; --------------------------------------------------------------------------------
; @Title: Readme for Arduino Pro Portenta H7 board
; @Description: ThreadDebug & t32Logger demo for TRACE32 GDB FrontEnd debugger
; @Keywords: ARM, Cortex-M7
; @Author: MAU
; @Board: Arduino Pro Portenta H7
; @Chip: STM32H747
; @Props: 
; @Copyright: (C) 1989-2020 Lauterbach GmbH, licensed for use with TRACE32(R) only
; --------------------------------------------------------------------------------
; $Id: readme.txt 11642 2020-10-26 07:58:50Z mmenegotto $

This demo program is based on the Arduino ThreadDebug example. It include the MRI serial monitor for debugging with TRACE32 front-end for GDB, via serial port. In addition we added to the project t32logger.ino and t32logger.h, that implement a software trace logger for TRACE32 debuggers.

This is useful in case you need to observe function calls sequences, or variable values, and your application do not have serial port available for printing. But even if you have a second serial port for printing (as in the T32ThreadDebugPrint demo), the software logger can be a better solution than printing, because it is much less intrusive, it uses no other resources than a ram buffer, record accurate timestamps, can be controlled (Arm/Off) by the debugger, can be listed and analyzed by the debugger.

The main program T32ThreadDebugLogger.ino define the T32_Logger struct, and has been instrumented with Logger calls to record function calls and variable values. See the code for calls to log function like T32_Logger.*

T32_Logger functions are called with the address of what you need to log, eg:

T32_Logger.Function(&setup);	// Log the entry address of function setup()
T32_Logger.Write(&FastCount);	// Log the address value and size as write cycle to the variable FastCount
T32_Logger.LogPC(); 	// Log the next Program Counter, it can be useful for tracing the passage from a particular point in the program. Note: due to compiler optimizations, the logged PC may be different than expected.

All available log methods are visibile in t32logger.h

The logger ring buffer size is defined in t32logger.h, see T32_LOGGER_SIZE, by default is 1024 records.

For each logged bus-cycle, T32_Logger add a time stamp in microseconds, TRACE32 debugger will use it to show trace timing in Trace.List or Trace.Chart Functions. 
By default the T32_Logger is automatically armed when you enter a GO or STEP to the program, and is off when you BREAK or hit a breakpoint. You can change logger settings with the command LOGGER.state.

Once you break, you can see the recorded trace with commands TRACE.LIST or TRACE.CHART.

You will see relative time on the left column. Time 0.0 us means the difference between two records is less than 1 microsecond, measured by micros() Arduino Core function. In order to have an additional time column with absolute time, you can enter this command:

	Trace.List DEFault TIme.ZERO // or T.L DEF TI.ZERO in short form) 
	
Then right-click on a record and select Set Zero.

For more information about the TRACE Logger, please refer to the manual debugger_user.pdf, chapter "Software Trace" (https://www.lauterbach.com/manual.html)
	

	
For TRACE32 and Portenta H7 board configuration, please follow the instructions of the readme.txt file on TRACE32 installation directory.

Once you load and start the demo program, you will see in the List window the program counter stopped at setup() function entry, visible in Mixed mode (C-source mixed to his assembler code). You can change view to HLL or Mixed with the [Mode] button.

You can STEP into the code, or enter a GO command to run the application. 
The application loop() will blink the red LED and green LED on board and call some demo functions Sieve.ino.

Note: the demo application do not include all source file used to build mbed os and Arduino Core, thus stepping into Arduino functions, at a certain point you will get a warning for missing source file, and the source code in the List window will be shown as dashed lines. The best way to return to your code is to go till a known function, for example enter:

	GO loop 

to run till the loop() function, that is called periodically.

The same happen if you enter a manual BREAK while the application is running, you will probably stop into the mbed idle function and you will not see mbed source files.

In order to see Arduino Core source files, please rebuild the application with your Arduino IDE, this will add to the Elf file the path to your Arduino Core source tree and the problem should disappear, except for the mbed OS that is provided as library only.

The demo application is compiled to break at setup() and wait for the debugger attach, and in fact LEDs are not blinking. This is due to this definition in T32ThreadDebugLogger.ino:

	ThreadDebug            threadDebug(&debugComm, DEBUG_BREAK_IN_SETUP);

This is useful to debug the application since the beginning of the setup() function. If you prefere the application start without waiting for the debugger, then change the definition to:

	ThreadDebug            threadDebug(&debugComm, DEBUG_NO_BREAK_IN_SETUP);
	
In this case the LEDs will start blinking immediately at board power-on, TRACE32 start script will attach and will break the program somewhere else, probably in the mbed idle. Enter this command to move to your code:

	GO loop
	
Please refer to the manual and /pdf/training_debugger.pdf to learn about TRACE32 debug functions.


The Lauterbach team.

