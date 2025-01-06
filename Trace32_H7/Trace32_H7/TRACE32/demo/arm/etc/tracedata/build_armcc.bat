armcc -c -g sieve.c
armasm --cpreproc -g tracedata.s
armlink --ro-base 0x40300000 -o sieve_omap4430.axf sieve.o tracedata.o

armcc -c -g --thumb sieve.c
armasm --cpreproc -g --thumb tracedata.s
armlink --ro-base 0x40300000 -o sieve_thumb_omap4430.axf sieve.o tracedata.o