
RAMSTART=0x0
CFLAGS=-ffunction-sections -fdata-sections
LDFLAGS=-gc-sections
TARGET=term_demo.c
SRC=t32term.c t32term_dcc.c t32dcc_wrapper.c
LIB=-lc

export RAMSTART
export CFLAGS
export LDFLAGS
export SRC
export INCLUDEDIR
export LIB

TARGETS=$(TARGET:%.c=%_dcc_pic_arm7_arm9.elf)       $(TARGET:%.c=%_dcc3_pic_arm7_arm9.elf)       $(TARGET:%.c=%_dcc4a_pic_arm7_arm9.elf)        \       
        $(TARGET:%.c=%_dcc_pic_arm11_cortex.elf)    $(TARGET:%.c=%_dcc3_pic_arm11_cortex.elf)    $(TARGET:%.c=%_dcc4a_pic_arm11_cortex.elf)     \
        $(TARGET:%.c=%_dcc_pic_arm11_cortex_be.elf) $(TARGET:%.c=%_dcc3_pic_arm11_cortex_be.elf) $(TARGET:%.c=%_dcc4a_pic_arm11_cortex_be.elf)  \
        $(TARGET:%.c=%_dcc_pic_xscale.elf)          $(TARGET:%.c=%_dcc3_pic_xscale.elf)          $(TARGET:%.c=%_dcc4a_pic_xscale.elf) \
        $(TARGET:%.c=%_dcc_pic_aarch64.elf)         $(TARGET:%.c=%_dcc3_pic_aarch64.elf)         $(TARGET:%.c=%_dcc4a_pic_aarch64.elf)


all: $(TARGETS)

clean:
	-rm $(TARGETS)

%_dcc_pic_arm7_arm9.elf:
	make -f ${T32SYS}/demo/arm/compiler/gnu-make/pic/makefile DEFINE="T32_DCC ARM7"      $(TARGET:%.c=%_pic_arm.elf)
	mv $(TARGET:%.c=%_pic_arm.elf)   $@

%_dcc3_pic_arm7_arm9.elf:
	make -f ${T32SYS}/demo/arm/compiler/gnu-make/pic/makefile DEFINE="T32_DCC3 ARM7"     $(TARGET:%.c=%_pic_arm.elf)
	mv $(TARGET:%.c=%_pic_arm.elf)   $@

%_dcc4a_pic_arm7_arm9.elf:
	make -f ${T32SYS}/demo/arm/compiler/gnu-make/pic/makefile DEFINE="T32_DCC4A ARM7"    $(TARGET:%.c=%_pic_arm.elf)
	mv $(TARGET:%.c=%_pic_arm.elf)   $@

%_dcc_pic_arm11_cortex.elf:
	make -f ${T32SYS}/demo/arm/compiler/gnu-make/pic/makefile DEFINE="T32_DCC CORTEXA"   $(TARGET:%.c=%_pic_arm.elf)
	mv $(TARGET:%.c=%_pic_arm.elf)   $@

%_dcc3_pic_arm11_cortex.elf:
	make -f ${T32SYS}/demo/arm/compiler/gnu-make/pic/makefile DEFINE="T32_DCC3 CORTEXA"  $(TARGET:%.c=%_pic_arm.elf)
	mv $(TARGET:%.c=%_pic_arm.elf)   $@

%_dcc4a_pic_arm11_cortex.elf:
	make -f ${T32SYS}/demo/arm/compiler/gnu-make/pic/makefile DEFINE="T32_DCC4A CORTEXA" $(TARGET:%.c=%_pic_arm.elf)
	mv $(TARGET:%.c=%_pic_arm.elf)   $@

%_dcc_pic_arm11_cortex_be.elf:
	make -f ${T32SYS}/demo/arm/compiler/gnu-make/pic/makefile DEFINE="T32_DCC CORTEXR"   $(TARGET:%.c=%_pic_armbe.elf)
	mv $(TARGET:%.c=%_pic_armbe.elf) $@

%_dcc3_pic_arm11_cortex_be.elf:
	make -f ${T32SYS}/demo/arm/compiler/gnu-make/pic/makefile DEFINE="T32_DCC3 CORTEXR"  $(TARGET:%.c=%_pic_armbe.elf)
	mv $(TARGET:%.c=%_pic_armbe.elf) $@

%_dcc4a_pic_arm11_cortex_be.elf:
	make -f ${T32SYS}/demo/arm/compiler/gnu-make/pic/makefile DEFINE="T32_DCC4A CORTEXR" $(TARGET:%.c=%_pic_armbe.elf)
	mv $(TARGET:%.c=%_pic_armbe.elf) $@

%_dcc_pic_aarch64.elf:
	make -f ${T32SYS}/demo/arm/compiler/gnu-make/pic/makefile DEFINE="T32_DCC CORTEXA"   $(TARGET:%.c=%_pic_aarch64.elf)
	mv $(TARGET:%.c=%_pic_aarch64.elf)   $@

%_dcc3_pic_aarch64.elf:
	make -f ${T32SYS}/demo/arm/compiler/gnu-make/pic/makefile DEFINE="T32_DCC3 CORTEXA"  $(TARGET:%.c=%_pic_aarch64.elf)
	mv $(TARGET:%.c=%_pic_aarch64.elf)  $@

%_dcc4a_pic_aarch64.elf:
	make -f ${T32SYS}/demo/arm/compiler/gnu-make/pic/makefile DEFINE="T32_DCC4A CORTEXA" $(TARGET:%.c=%_pic_aarch64.elf)
	mv $(TARGET:%.c=%_pic_aarch64.elf) $@

%_dcc_pic_xscale.elf:
	make -f ${T32SYS}/demo/arm/compiler/gnu-make/pic/makefile DEFINE="T32_DCC XSCALE"    $(TARGET:%.c=%_pic_arm.elf)
	mv $(TARGET:%.c=%_pic_arm.elf)   $@

%_dcc3_pic_xscale.elf:
	make -f ${T32SYS}/demo/arm/compiler/gnu-make/pic/makefile DEFINE="T32_DCC3 XSCALE"   $(TARGET:%.c=%_pic_arm.elf)
	mv $(TARGET:%.c=%_pic_arm.elf)   $@

%_dcc4a_pic_xscale.elf:
	make -f ${T32SYS}/demo/arm/compiler/gnu-make/pic/makefile DEFINE="T32_DCC4A XSCALE"  $(TARGET:%.c=%_pic_arm.elf)
	mv $(TARGET:%.c=%_pic_arm.elf)   $@
