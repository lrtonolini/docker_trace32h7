# Makefile to build Overlay Example
# with GNU GCC and/or with ARM RealView toolchain (RVCT4.0 [Build 488] or higher) 
# (use GNU make)

export SHELLOPTS := igncr


OBJECTS0 := crt0 caller 
OBJECTS1 := task1 task sieve funcasm page loader
OBJECTS2 := blubber page loader
TMP      := ./tmp
VPATH    := . ./src

GNUCOPT  := -gdwarf-2 -g2 -Wall -march=armv4t -mlittle-endian -fshort-enums
GNULOPT  := -mlittle-endian -nostdlib -Wl,--nmagic
GCC      := arm-none-eabi-gcc

ARMCOPT := --debug --dwarf3 --cpu=4T --arm --littleend --diag_suppress=C170
ARMLOPT := --debug --bestdebug --elf --no_startup --diag_suppress=L6314,L6329

help: 
	@echo make gnu : build overlay example with GNU GCC toolchain
	@echo make arm : build overlay example with ARM RealView toolchain

$(TMP):
	mkdir $(TMP)

all: arm gnu

clean: arm.clean gnu.clean
	rm -rf $(TMP)


gnu: $(TMP) caller.elf page1.elf page2.elf

$(TMP)/%.gnu.o: %.gnu.s makefile
	$(GCC) $(GNUCOPT) -c -o $@ $< 

$(TMP)/%.gnu.o: %.c makefile
	$(GCC) $(GNUCOPT) -c -o $@ $< 

$(TMP)/loader2.gnu.o: loader.c makefile
	$(GCC) $(GNUCOPT) -c -o $@ $< 

page1.elf:  page1.gnu.ld  $(addprefix $(TMP)/,$(addsuffix .gnu.o, $(OBJECTS1)))
	$(GCC) $(GNULOPT) -o $@ -Wl,-T $^ -Wl,--emit-relocs

page2.elf:  page2.gnu.ld  $(addprefix $(TMP)/,$(addsuffix .gnu.o, $(OBJECTS2)))
	$(GCC) $(GNULOPT) -o $@ -Wl,-T $^ -Wl,--emit-relocs

caller.elf: caller.gnu.ld $(addprefix $(TMP)/,$(addsuffix .gnu.o, $(OBJECTS0)))
	$(GCC) $(GNULOPT) -o $@ -Wl,-T $^

gnu.clean:
	rm -fv $(TMP)/*.gnu.o
	rm -fv *.elf 
	

arm: $(TMP) caller.axf page1.axf page2.axf

$(TMP)/%.arm.o: %.arm.s makefile
	armasm $(ARMCOPT) -o $@ $< 

$(TMP)/%.arm.o: %.c makefile
	armcc $(ARMCOPT) -O0 -c -o $@ $< 

page1.axf:  page1.arm.ld  $(addprefix $(TMP)/,$(addsuffix .arm.o, $(OBJECTS1))) 
	armlink $(ARMLOPT) -o $@ --scatter $^ --entry=init --emit-relocs

page2.axf:  page2.arm.ld  $(addprefix $(TMP)/,$(addsuffix .arm.o, $(OBJECTS2)))
	armlink $(ARMLOPT) -o $@ --scatter $^ --entry=init --emit-relocs

caller.axf: caller.arm.ld $(addprefix $(TMP)/,$(addsuffix .arm.o, $(OBJECTS0))) 
	armlink $(ARMLOPT) -o $@ --scatter $^

arm.clean:
	rm -fv $(TMP)/*.arm.o
	rm -fv *.axf 
