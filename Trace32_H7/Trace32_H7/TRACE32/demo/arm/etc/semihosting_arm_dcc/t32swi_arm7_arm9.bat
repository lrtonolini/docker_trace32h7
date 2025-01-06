REM script for ARM compiler (V2.5.1)
REM system variables below need to be adapted

REM set PATH=<bin_path>
REM set ARMLIB=<lib_path>

setlocal

if "%ROBASE%"=="" set ROBASE=0x18000

armcc -c -apcs /interwork -g -DT32_TERM_BLOCKED -DT32_TERM_TYPEAHEADBUFFER -DSWI2DCC3 t32swi.c t32helper_arm7_arm9.c t32term.c
armlink -ro-base %ROBASE% -o t32swi_arm7_arm9.axf t32swi.o t32helper_arm7_arm9.o t32term.o

endlocal
