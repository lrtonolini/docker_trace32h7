REM script for ARM compiler (V2.5.1)
REM system variables below need to be adapted

REM set PATH=<bin_path>
REM set ARMLIB=<lib_path>

armcc -c -apcs /interwork -g t32swi.c t32helper_arm11_cortexr_cortexa.c
armlink -ro-base 0x18000 -o t32swi_arm11_cortexr_cortexa.axf t32swi.o t32helper_arm11_cortexr_cortexa.o


