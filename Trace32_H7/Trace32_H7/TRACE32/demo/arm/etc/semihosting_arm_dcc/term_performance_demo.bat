REM script for ARM compiler (V5.06)
REM system variables below need to be adapted

REM set PATH=<bin_path>
REM set ARMLIB=<lib_path>

setlocal

if "%ROBASE%"=="" set ROBASE=0x100

armcc -c -apcs /interwork -g -O0 -DUSESWI -DUSESYSCALLS term_performance_demo.c
armlink -ro-base %ROBASE% --no_remove -o term_performance_demo.axf term_performance_demo.o

endlocal
