armcc --arm -O1 -g --apcs /ropi/rwpi -c benchmarkentry.s benchmark.c
armcc --arm -O3 -Otime --apcs /ropi/rwpi --apcs=/rwpi -c dhry_1.c 
armcc --arm -O3 -Otime --apcs /ropi/rwpi --apcs=/rwpi -c dhry_2.c
armlink -o benchmark.x --ropi --rwpi --ro-base 0 --rw-base 0x2000 --first benchmarkentry.o benchmarkentry.o benchmark.o dhry_1.o dhry_2.o
