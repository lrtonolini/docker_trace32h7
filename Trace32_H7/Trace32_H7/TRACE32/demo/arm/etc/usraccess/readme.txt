; --------------------------------------------------------------------------------
; @Title: User Controlled Memory Access - README
; @Description: -
; @Author: HLG
; @Copyright: (C) 1989-2022 Lauterbach GmbH, licensed for use with TRACE32(R) only
; --------------------------------------------------------------------------------
; $Id: readme.txt 18850 2022-01-26 18:41:29Z bschroefel $

User Controlled Memory Access (USR: memory class)

The user controlled memory access can be used to make special memory 
access or to view memory using burst accesses.

To access memory in a user specific way you have to write a target monitor program 
which provides data for TRACE32.
The monitor program is then temporarily loaded to you target system, whenever you
access memory with memory class "USR:" (E.g.: Data.dump USR:0x00--0xff) 

The target monitor program must be compiled in 32-bit ARM code 
except for Cortex-M which accepts only thumb code

To initialize the user controlled memory access use the following command.
   Data.USRACCESS 0x1000++0xfff,,"usraccess.bin"
"usraccess.bin" is the monitor code which collects the requested user data for TRACE32. 
The address range specifies the target addresses to where "usraccess.bin" gets loaded.  
The original data in this address range will be restored, when restarting the CPU. 

The example program simply accesses the main memory at the given address.
See the example working with
    DO "usraccess.cmm"
    Data.dump USR:0x00--0xff

