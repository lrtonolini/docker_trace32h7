REM script for ARM compiler (V2.5.1)
REM system variables below need to be adapted

REM set PATH=<bin_path>
REM set ARMLIB=<lib_path>

setlocal

if "%ROBASE%"=="" set ROBASE=0x18000
set TERMOPTS=-DT32_TERM_BLOCKED -DT32_TERM_TYPEAHEADBUFFER
REM set TERMOPTS=%TERMOPTS% -DT32_TERM_NOCHECKSUM -DT32_TERM_WRITEQUEUE

armcc -c -apcs /interwork -g %TERMOPTS% -DSWI2DCC3 t32swi.c t32helper_arm11_cortexr_cortexa.c t32term.c
armlink -ro-base %ROBASE% -o t32swi_arm11_cortexr_cortexa.axf t32swi.o t32helper_arm11_cortexr_cortexa.o t32term.o

armcc -c -apcs /interwork -g %TERMOPTS% -DT32_TERM_BLOCKED -DT32_TERM_METHOD_MEMORY t32swi.c t32term.c t32term_memory.c
armlink -ro-base %ROBASE% -o t32swi_arm11_cortexr_cortexa_buffere.axf t32swi.o t32term.o t32term_memory.o

endlocal
