export SHELLOPTS := igncr

all: arm gnu
clean: arm.clean gnu.clean


ACPU  := --cpu=4 --arm_only --littleend
ACOPT := $(ACPU) -c --debug -O0
ALOPT := $(ACPU)

arm: monitor_arm7_arm9.axf
arm: monitor_arm10.axf
arm: monitor_arm11_cortexr_cortexa.axf

%.obj: %.c $(MAKEFILE_LIST)
	armcc   $(ACOPT) -o $@ $<

monitor_arm7_arm9.axf:             monitor.scat main.obj monitor.obj dcc_driver_arm7_arm9.obj
	armlink $(ALOPT) -o $@ --scatter=$^

monitor_arm10.axf:                 monitor.scat main.obj monitor.obj dcc_driver_arm10.obj
	armlink $(ALOPT) -o $@ --scatter=$^

monitor_arm11_cortexr_cortexa.axf: monitor.scat main.obj monitor.obj dcc_driver_arm11_cortexr_cortexa.obj
	armlink $(ALOPT) -o $@ --scatter=$^

arm.clean:
	-rm -fv *.obj
	-rm -fv monitor_arm7_arm9.axf
	-rm -fv monitor_arm10.axf
	-rm -fv monitor_arm11_cortexr_cortexa.axf


GCPU  := -march=armv4 -marm -mlittle-endian
GCOPT := $(GCPU) -c -g -Wall
GLOPT := $(GCPU) -Wl,--nmagic -nostartfiles -nodefaultlibs
GCC   := arm-none-eabi-gcc

gnu: monitor_arm7_arm9.elf
gnu: monitor_arm10.elf
gnu: monitor_arm11_cortexr_cortexa.elf

%.o: %.c $(MAKEFILE_LIST)
	$(GCC) $(GCOPT) -o $@ $<

%.o: %.sx   $(MAKEFILE_LIST)
	$(GCC) $(GCOPT) -xassembler-with-cpp -o $@ $<

monitor_arm7_arm9.elf:             monitor.ld crt0.o main.o monitor.o dcc_driver_arm7_arm9.o
	$(GCC) $(GLOPT) -o $@ -T $^

monitor_arm10.elf:                 monitor.ld crt0.o main.o monitor.o dcc_driver_arm10.o
	$(GCC) $(GLOPT) -o $@ -T $^

monitor_arm11_cortexr_cortexa.elf: monitor.ld crt0.o main.o monitor.o dcc_driver_arm11_cortexr_cortexa.o
	$(GCC) $(GLOPT) -o $@ -T $^

gnu.clean:
	-rm -fv *.o
	-rm -fv monitor_arm7_arm9.elf
	-rm -fv monitor_arm10.elf
	-rm -fv monitor_arm11_cortexr_cortexa.elf
