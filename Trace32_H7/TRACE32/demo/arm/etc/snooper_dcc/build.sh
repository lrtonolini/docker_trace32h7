#!/bin/bash
make clean
# ARM7, ARM9 builds
make DEFINE="ARM7" snooper_dcc_pic_arm.elf
mv snooper_dcc_pic_arm.elf snooper_dcc_pic_arm7_arm9.elf 

# ARM11, Cortex-A, Cortex-R builds
make DEFINE="ARM11" snooper_dcc_pic_arm.elf
mv snooper_dcc_pic_arm.elf snooper_dcc_pic_arm11_cortex.elf

# Cortex-R big-endian builds
make DEFINE="ARM11" snooper_dcc_pic_armbe.elf
mv snooper_dcc_pic_armbe.elf snooper_dcc_pic_arm11_cortex_be.elf 

# XSCALE builds
make DEFINE="XSCALE" snooper_dcc_pic_arm.elf
mv snooper_dcc_pic_arm.elf snooper_dcc_pic_xscale.elf 
