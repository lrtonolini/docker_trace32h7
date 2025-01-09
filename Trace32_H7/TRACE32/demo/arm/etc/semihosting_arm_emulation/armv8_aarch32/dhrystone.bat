REM generates semihosting demo with ARM C/C++ Compiler for ARMv8, 4.9

armcc --c99 -g -c --interleave -O0 --arm --cpu=8-A.32 -DTIME dhry_1.c dhry_2.c
armlink --scatter scatter.cfg --cpu=8-A.32 --output dhrystone.axf dhry_1.o dhry_2.o

DEL *.o