DEFINEX ?=

RAMSTART=0x00000000
SRC = t32term_arm.c t32term.c term_demo.c

DEFINE=STACKSIZE=0x1400 T32_TERM_BLOCKED $(DEFINEX)
LIB=-lc


include ${T32SYS}/demo/arm/compiler/gnu-make/pic/makefile
