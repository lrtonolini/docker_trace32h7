; --------------------------------------------------------------------------------
; @Title: Readme for Arduino Pro Portenta H7 board
; @Description: ThreadDebug demo for TRACE32 GDB FrontEnd debugger
; @Keywords: ARM, Cortex-M7
; @Author: MAU
; @Board: Arduino Pro Portenta H7
; @Chip: STM32H747
; @Props: 
; @Copyright: (C) 1989-2020 Lauterbach GmbH, licensed for use with TRACE32(R) only
; --------------------------------------------------------------------------------
; $Id: readme.txt 11642 2020-10-26 07:58:50Z mmenegotto $

This demo program is based on the Arduino ThreadDebug example. It include the MRI serial monitor for debugging with TRACE32 front-end for GDB, via serial port. 

For TRACE32 and Portenta H7 board configuration, please follow the instructions of the readme.txt file on TRACE32 installation directory.

Once you load and start the demo program, you will see in the List window the program counter stopped at setup() function entry, visible in Mixed mode (C-source mixed to his assembler code). You can change view to HLL or Mixed with the [Mode] button.

You can STEP into the code, or enter a GO command to run the application. 
The application loop() will blink the red LED and green LED on board and call some demo functions Sieve.ino.

Note: the demo application do not include all source file used to build mbed os and Arduino Core, thus stepping into Arduino functions, at a certain point you will get a warning for missing source file, and the source code in the List window will be shown as dashed lines. The best way to return to your code is to go till a known function, for example enter:

	GO loop 

to run till the loop() function, that is called periodically.

The same happen if you enter a manual BREAK while the application is running, you will probably stop into the mbed idle function and you will not see mbed source files.

In order to see Arduino Core source files, please rebuild the application with your Arduino IDE, this will add to the Elf file the path to your Arduino Core source tree and the problem should disappear, except for the mbed OS that is provided as library only.

The demo application is compiled to break at setup() and wait for the debugger attach, and in fact LEDs are not blinking. This is due to this definition in T32ThreadDebug.ino:

	ThreadDebug            threadDebug(&debugComm, DEBUG_BREAK_IN_SETUP);

This is usefull to debug the application since the beginning of the setup() function. If you prefere the application start without waiting for the debugger, then change the definition to:

	ThreadDebug            threadDebug(&debugComm, DEBUG_NO_BREAK_IN_SETUP);
	
In this case the LEDs will start blinking immediately at board power-on, TRACE32 start script will attach and will break the program somewhere else, probably in the mbed idle. Enter this command to move to your code:

	GO loop
	
Please refer to the manual /pdf/debugger_tutorial.pdf and /pdf/training_debugger.pdf to learn about TRACE32 debug functions.


The Lauterbach team.
