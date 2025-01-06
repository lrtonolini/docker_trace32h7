set PATH=c:\arm251\bin;%PATH%
set ARMLIB=c:\arm251\lib

armcc -g -o monitor_arm9xxe.axf monitor_entry.s monitor.c dcc_driver_arm7_arm9.c

