# Makefile to build Overlay Example
# with GNU GCC and/or with ARM RealView (>=2.2) toolchain
#
# For linking the examples with any "--emit-..." option
# you have to use GNU GCC or ARM RVCT4.0[Build 488] (or higher)

# (use GNU make)

export SHELLOPTS := igncr


NAME    := ovdemo
OBJECTS := crt0 blubber task1 task sieve funcasm ovdemo
TMP     := ./tmp
VPATH    = ./src

GOBJECTS := $(addprefix $(TMP)/, $(addsuffix .o, $(OBJECTS)))
GNUCOPT  := -Wall -g3 -march=armv4t -mlittle-endian -fshort-enums
GNULOPT  := -Wall -g -mlittle-endian -nostdlib -Wl,--nmagic
GCC := arm-none-eabi-gcc

AOBJECTS := $(addprefix $(TMP)/, $(addsuffix .obj, $(OBJECTS)))
ARMCOPT := --debug --dwarf2 --cpu=4T --arm --littleend
ARMLOPT := --bestdebug --elf --no_startup --no_scanlib

.PHONY : help
help:
	@echo make gnu : build overlay example with GNU GCC toolchain
	@echo make arm : build overlay example with ARM RealView toolchain

.PHONY : clean
clean: arm.clean gnu.clean

$(TMP):
	mkdir $(TMP)

.PHONY : gnu
gnu: $(NAME).elf $(NAME).alt.elf

$(TMP)/%.o: %.s  $(MAKEFILE_LIST) | $(TMP)
	$(GCC) $(GNUCOPT) -xassembler-with-cpp -c -o $@ $<

$(TMP)/%.o: %.c  $(MAKEFILE_LIST) | $(TMP)
	$(GCC) $(GNUCOPT) -c -o $@ $<

$(NAME).alt.elf: $(NAME).ld $(GOBJECTS) $(TMP)/func.o $(TMP)/inc.o
	$(GCC) $(GNULOPT)                   -o $@ -Wl,-T $^

$(NAME).elf:     $(NAME).ld $(GOBJECTS) $(TMP)/mix.o
	$(GCC) $(GNULOPT) -Wl,--emit-relocs -o $@ -Wl,-T $^

.PHONY : gnu.clean
gnu.clean:
	rm -fv $(TMP)/*.o
	rm -fv $(NAME)*.elf


.PHONY : arm
arm: $(NAME).axf $(NAME).alt.axf

$(TMP)/%.obj: %.asm $(MAKEFILE_LIST) | $(TMP)
	armasm $(ARMCOPT) -o $@ $<

$(TMP)/%.obj: %.c   $(MAKEFILE_LIST) | $(TMP)
	armcc $(ARMCOPT) -O0 -c -o $@ $<

$(NAME).alt.axf:         $(NAME).scat $(AOBJECTS) $(TMP)/func.obj $(TMP)/inc.obj
	armlink $(ARMLOPT)                              -o $@ --scatter $^

$(NAME).axf:             $(NAME).scat $(AOBJECTS) $(TMP)/mix.obj
	armlink $(ARMLOPT) --emit-debug-overlay-section -o $@ --scatter $^

$(NAME).relocs.axf:      $(NAME).scat $(AOBJECTS) $(TMP)/mix.obj
	armlink $(ARMLOPT) --emit-relocs                -o $@ --scatter $^

$(NAME).debugrelocs.axf: $(NAME).scat $(AOBJECTS) $(TMP)/mix.obj
	armlink $(ARMLOPT) --emit-debug-overlay-relocs  -o $@ --scatter $^

.PHONY : arm.clean
arm.clean:
	rm -fv $(TMP)/*.obj
	rm -fv $(NAME)*.axf

