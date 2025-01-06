
RAMSTART=0x0
CFLAGS=-ffunction-sections -fdata-sections
LDFLAGS=-gc-sections
TARGET=term_demo.c
SRC=t32term.c t32term_memory.c
LIB=-lc

export RAMSTART
export CFLAGS
export LDFLAGS
export SRC
export INCLUDEDIR
export LIB

TARGETS=$(TARGET:%.c=%_single_pic_arm.elf)          $(TARGET:%.c=%_block_pic_arm.elf)         \
        $(TARGET:%.c=%_single_pic_armbe.elf)        $(TARGET:%.c=%_block_pic_armbe.elf)       \
        $(TARGET:%.c=%_single_pic_thumb_v4.elf)     $(TARGET:%.c=%_block_pic_thumb_v4.elf)    \
        $(TARGET:%.c=%_single_pic_thumb_v6m.elf)    $(TARGET:%.c=%_block_pic_thumb_v6m.elf)   \
        $(TARGET:%.c=%_single_pic_thumb_ii_v7m.elf) $(TARGET:%.c=%_block_pic_thumb_ii_v7m.elf)

all: $(TARGETS)

clean:
	-rm $(TARGETS)
	
%_single_pic_arm.elf: 
	make -f ${T32SYS}/demo/arm/compiler/gnu-make/pic/makefile $(subst _single,,$@)
	mv $(subst _single,,$@) $@

%_single_pic_armbe.elf:
	make -f ${T32SYS}/demo/arm/compiler/gnu-make/pic/makefile $(subst _single,,$@)
	mv $(subst _single,,$@) $@

%_single_pic_thumb_v4.elf:
	make -f ${T32SYS}/demo/arm/compiler/gnu-make/pic/makefile $(subst _single,,$@)
	mv $(subst _single,,$@) $@

%_single_pic_thumb_v6m.elf:
	make -f ${T32SYS}/demo/arm/compiler/gnu-make/pic/makefile $(subst _single,,$@)
	mv $(subst _single,,$@) $@

%_single_pic_thumb_ii_v7m.elf:
	make -f ${T32SYS}/demo/arm/compiler/gnu-make/pic/makefile $(subst _single,,$@)
	mv $(subst _single,,$@) $@

%_block_pic_arm.elf:
	make -f ${T32SYS}/demo/arm/compiler/gnu-make/pic/makefile DEFINE="T32_TERM_BLOCKED T32_TERM_MEMORY_BLOCKED_SIZE=0x100 STACKSIZE=0x1800" $(subst _block,,$@)
	mv $(subst _block,,$@) $@

%_block_pic_armbe.elf:
	make -f ${T32SYS}/demo/arm/compiler/gnu-make/pic/makefile DEFINE="T32_TERM_BLOCKED T32_TERM_MEMORY_BLOCKED_SIZE=0x100 STACKSIZE=0x1800" $(subst _block,,$@)
	mv $(subst _block,,$@) $@

%_block_pic_thumb_v4.elf:
	make -f ${T32SYS}/demo/arm/compiler/gnu-make/pic/makefile DEFINE="T32_TERM_BLOCKED T32_TERM_MEMORY_BLOCKED_SIZE=0x100 STACKSIZE=0x1800" $(subst _block,,$@)
	mv $(subst _block,,$@) $@

%_block_pic_thumb_v6m.elf:
	make -f ${T32SYS}/demo/arm/compiler/gnu-make/pic/makefile DEFINE="T32_TERM_BLOCKED T32_TERM_MEMORY_BLOCKED_SIZE=0x100 STACKSIZE=0x1800" $(subst _block,,$@)
	mv $(subst _block,,$@) $@

%_block_pic_thumb_ii_v7m.elf:
	make -f ${T32SYS}/demo/arm/compiler/gnu-make/pic/makefile DEFINE="T32_TERM_BLOCKED T32_TERM_MEMORY_BLOCKED_SIZE=0x100 STACKSIZE=0x1800" $(subst _block,,$@)
	mv $(subst _block,,$@) $@

#%.elf:
#	# build file term_demo.c
#	# remove _single & _block from filename $(subst _single,,$(subst _block,,$@))
#	# if _block is part of the filename, add -DT32_TERM_BLOCK
#	make -f ${T32SYS}/demo/arm/compiler/gnu-make/pic/makefile DEFINE=$(if $(findstring _block,$@),"T32_TERM_BLOCKED T32_TERM_MEMORY_BLOCKED_SIZE=0x100 STACKSIZE=0x1800") $(subst _single,,$(subst _block,,$@))
#	mv $(subst _single,,$(subst _block,,$@)) $@
