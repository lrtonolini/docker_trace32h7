RAMSTART=0x00000000
TARGET=term_performance_demo.c
SRC=syscalls.c t32term.c t32term_memory.c
DEFINE=STACKSIZE=0xa00
# -DT32_TERM_MEMORY_BLOCKED_SIZE=7184 -DT32_TERM_NOCHECKSUM -DT32_TERM_WRITEQUEUE
CFLAGS=-ffunction-sections -fdata-sections -std=c99 -DT32_TERM_BLOCKED -DT32_TERM_METHOD_MEMORY -DUSESYSCALLS -DUSETERMCALLS -DBUILDARM
LDFLAGS=--gc-sections
LIB=-lc-debug

include ${T32SYS}/demo/arm/compiler/gnu-make/pic/makefile

