export SHELLOPTS := igncr

NAME :=	cp15
OBJ  := cp15 mrc

.PHONY : help all gnu arm clean
help:
	@echo "Valid targets:"
	@echo "   all   : build 'gnu' and 'arm'"
	@echo "   gnu   : build $(NAME).gnu.bin with GCC"
	@echo "   arm   : build $(NAME).arm.bin with ARM RealView compiler"
	@echo "   clean : delete all o-files, elf-files and bin-files"
all: gnu arm


##### GNU compiler (gcc) #####
CCGRP := arm-none-eabi-
GCPU  := -march=armv4t -marm -mlittle-endian -msoft-float
GAOPT := -Wall $(GCPU) -xassembler-with-cpp
GCOPT := -Wall $(GCPU) -fPIE -fshort-enums -fno-zero-initialized-in-bss
GLOPT := -Wall $(GCPU) -Wl,--nmagic -nostdlib -Wl,--entry=usraccess

ifneq ($(DEBUG),)
  GAOPT += -Wa,--gdwarf2
  GCOPT += -g
else
  GCOPT += -Os
endif

.PHONY : gnu
gnu: $(NAME).gnu.bin

%.gnu.o: %.c $(MAKEFILE_LIST)
	$(CCGRP)gcc $(GCOPT) -c -o $@ $<

%.gnu.o: %.sx $(MAKEFILE_LIST)
	$(CCGRP)gcc $(GAOPT) -c -o $@ $<

.PRECIOUS: %.gnu.elf
%.gnu.elf: usraccess.gnu.ld $(addsuffix .gnu.o, $(OBJ))
	$(CCGRP)gcc $(GLOPT) -o $@ -T $^

%.gnu.bin : %.gnu.elf
	$(CCGRP)objcopy -O binary $< $@


##### ARM RealView or Development Studio compiler (armcc) #####
ACPU  := --cpu=4T --littleend
AAOPT := $(ACPU) --arm
ACOPT := $(ACPU) --apcs=/fpic --bss_threshold=8
ALOPT := $(ACPU) --no_startup --diag_suppress=L6314 --entry=usraccess
ifneq ($(DEBUG),)
  AAOPT += --debug --dwarf3
  ACOPT += --debug --dwarf3 -O0
  ALOPT += --debug --bestdebug
else
  ACOPT += -Ospace
endif

.PHONY : arm
arm: $(NAME).arm.bin

%.arm.o: %.c $(MAKEFILE_LIST)
	armcc $(ACOPT) -c -o $@ $<

%.arm.o: %.sx $(MAKEFILE_LIST)
	armcc $(AAOPT) -c -o $@ $<

.PRECIOUS: %.arm.elf
%.arm.elf: usraccess.arm.ld $(addsuffix .arm.o, $(OBJ))
	armlink $(ALOPT) -o $@ --scatter $^

%.arm.bin: %.arm.elf
	fromelf --bin --output $@ $<


clean:
	rm -fv *.o
	rm -fv *.elf
	rm -fv *.bin
