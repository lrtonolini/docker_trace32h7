TARGET=sieve.c
SRC=tracedata.s
include ${T32SYS}/demo/arm/compiler/gnu-make/pic/makefile

%.s : %.sx
	cp $< $@
	
%.ec2013.asm : %.sx
	$(CCGRP)cpp -P -undef -DMASM_EC2013 -o $@ $<

%.ce5_to_ce7.s : %.sx
	$(CCGRP)cpp -P -undef -DMASM_CE5_CE6_CE7 -o $@ $<

