REM script for ARM compiler (V2.5.1)
REM system variables below need to be adapted

REM set PATH=<bin_path>
REM set ARMLIB=<lib_path>

setlocal

if "%ROBASE%"=="" set ROBASE=0x100

armcc -c -apcs /interwork -g dhry_1.c dhry_2.c -DTIME
armlink -ro-base %ROBASE% -o dhrystone.axf dhry_1.o dhry_2.o

endlocal
