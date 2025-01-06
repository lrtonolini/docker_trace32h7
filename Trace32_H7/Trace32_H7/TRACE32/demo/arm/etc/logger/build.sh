#!/bin/bash
make clean
rm -rf tmp

# ARM7, ARM9, ARM11, Cortex-A, Cortex-R builds (AArch32)
make DEFINE="LOGGER_TIMER_DEMONSTRATION" logger_pic_arm.elf

# Cortex-R big-ednian builds
make DEFINE="LOGGER_TIMER_DEMONSTRATION" logger_pic_armbe.elf
mv logger_pic_armbe.elf logger_pic_arm11_cortex_be.elf 

# Cortex-A, Cortex-R builds (AArch64)
make DEFINE="LOGGER_64BIT LOGGER_TIMER_DEMONSTRATION" logger_pic_aarch64.elf

# Cortex-A, Cortex-R builds, THUMB-2 (AArch32)
make DEFINE="LOGGER_TIMER_DEMONSTRATION" logger_pic_thumb_ii_v7.elf

# Cortex-M0,M0+,M1,M23 builds
make DEFINE="LOGGER_TIMER_DEMONSTRATION" logger_pic_thumb_v6m.elf

# Cortex-M3,M4,M7,M33 builds
make DEFINE="LOGGER_TIMER_DEMONSTRATION" logger_pic_thumb_ii_v7m.elf
