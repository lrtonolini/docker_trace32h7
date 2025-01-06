#!/bin/bash
make clean
rm -rf tmp

# ARM7, ARM9 builds
make "DEFINEX=DCC3 ARM9" t32term_pic_arm.elf
mv t32term_pic_arm.elf t32term_pic_arm7_arm9.elf 

# ARM11, Cortex-A, Cortex-R builds (AArch32)
make "DEFINEX=DCC3 ARM11" t32term_pic_arm.elf
mv t32term_pic_arm.elf t32term_pic_arm11_cortex.elf 

# Cortex-R big-ednian builds
make "DEFINEX=DCC3 ARM11" t32term_pic_armbe.elf
mv t32term_pic_armbe.elf t32term_pic_arm11_cortex_be.elf 

# Cortex-A, Cortex-R builds (AArch64)
make "DEFINEX=DCC3 CORTEXA" t32term_pic_aarch64.elf

# Cortex-M0,M0+,M1,M23 builds
make "DEFINEX=CORTEXM" t32term_pic_thumb_v6m.elf

# Cortex-M3,M4,M7,M33 builds
make "DEFINEX=CORTEXM" t32term_pic_thumb_ii_v7m.elf
