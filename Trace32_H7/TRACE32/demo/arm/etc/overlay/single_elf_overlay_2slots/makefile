# Makefile to build Overlay Example
# with GNU GCC and/or with ARM RealView (>=2.2) toolchain
#
# For linking the examples with any "--emit-..." option
# you have to use GNU GCC or ARM RVCT4.0[Build 488] (or higher)

# (use GNU make)

export SHELLOPTS := igncr


NAME    := ovdemo
OBJECTS := crt0 main ovl_a0	ovl_a1 ovl_b0.thumb ovl_b1
TMP     := ./tmp
VPATH    = ./src

GOBJECTS := $(addprefix $(TMP)/, $(addsuffix .o, $(OBJECTS)))
GNUCOPT  := -Wall -g3 -march=armv7-a -mlittle-endian -fshort-enums
GNULOPT  := -Wall -g -mlittle-endian -nostdlib -Wl,--nmagic
GCC := arm-none-eabi-gcc

AOBJECTS := $(addprefix $(TMP)/, $(addsuffix .obj, $(OBJECTS)))
ARMCOPT := --debug --dwarf2 --cpu=7-A --arm --littleend
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
gnu: $(NAME).elf

$(TMP)/%.o:       %.s  $(MAKEFILE_LIST) | $(TMP)
	$(GCC) $(GNUCOPT) -xassembler-with-cpp -c -o $@ $<

$(TMP)/%.o:       %.c  $(MAKEFILE_LIST) | $(TMP)
	$(GCC) $(GNUCOPT) -c -o $@ $<

$(TMP)/%.thumb.o: %.c  $(MAKEFILE_LIST) | $(TMP)
	$(GCC) $(GNUCOPT) -mthumb -c -o $@ $<

$(NAME).elf:     $(NAME).ld $(GOBJECTS)
	$(GCC) $(GNULOPT) -Wl,--emit-relocs -o $@ -Wl,-T $^

.PHONY : gnu.clean
gnu.clean:
	rm -fv $(TMP)/*.o
	rm -fv $(NAME)*.elf


.PHONY : arm
arm: $(NAME).axf

$(TMP)/%.obj:       %.asm $(MAKEFILE_LIST) | $(TMP)
	armasm $(ARMCOPT) -o $@ $<

$(TMP)/%.obj:       %.c   $(MAKEFILE_LIST) | $(TMP)
	armcc $(ARMCOPT) -O0 -c -o $@ $<

$(TMP)/%.thumb.obj: %.c   $(MAKEFILE_LIST) | $(TMP)
	armcc $(ARMCOPT) -O0 --thumb -c -o $@ $<

$(NAME).axf:             $(NAME).scat $(AOBJECTS)
	armlink $(ARMLOPT) --emit-debug-overlay-section -o $@ --scatter $^

.PHONY : arm.clean
arm.clean:
	rm -fv $(TMP)/*.obj
	rm -fv $(NAME)*.axf

