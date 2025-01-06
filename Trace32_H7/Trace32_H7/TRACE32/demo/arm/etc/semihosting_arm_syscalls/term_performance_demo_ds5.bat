REM script for ARM compiler (V5.06)
REM system variables below need to be adapted

REM set PATH=<bin_path>
REM set ARMLIB=<lib_path>

setlocal

set TERMOPTS=-DT32_TERM_BLOCKED -DT32_TERM_METHOD_MEMORY -DT32_TERM_TYPEAHEADBUFFER -DT32_TERM_NOCHECKSUM
REM set TERMOPTS=%TERMOPTS% -DT32_TERM_NOCHECKSUM -DT32_TERM_WRITEQUEUE

armcc -g -O0 -c %TERMOPTS% -DUSESYSCALLS -DUSETERMCALLS retarget.c t32term.c t32term_memory.c term_performance_demo.c
armlink --no_remove --scatter=scatter.scat -o term_performance_demo_ds5.axf term_performance_demo.o retarget.o t32term.o t32term_memory.o

endlocal
