;=================================================================
;  Background Monitor for TRACE32 ARM JTAG Debugger
;  PEG, July 2008
;
;  If the debugger is switched from halt mode debugging to monitor
;  mode debugging (Go.MONitor, Break.SETMONITOR ON), a trap
;  (PABORT, DABORT) will happen instead of halting the program execution
;  when a breakpoint hits or when the BREAK button gets pushed.
;  (The BREAK button causes an on-chip breakpoint range 0-0xffffffff
;  for user mode.)
;
;  When UDMON3 is selected, all onchip breakpoints will be specified
;  for user mode only. It assumes the application which shall be debugged 
;  is running in user mode. TSMON3 and PTMON3 can theoretically also 
;  be used with the background monitor, but then the BREAK button can
;  not be imlemented by a breakpoint range, since even the monitor code 
;  would cause a re-entry into the monitor. Therefore the other two modes 
;  shall not be used for background monitor debugging.
;
;  This example has been tested on an ARM966E-S and ARM926EJ-S Integrator
;  Core Module from ARM. An ARM9..E-S derivative is needed since on other
;  ARM9 derivatives the on-chip breakpoints can not be specified for user mode.
;  Theoretically it will also work on ARM11 and Cortex, but this has not
;  been tested. Then a few changes of the monitor code might be required,
;  but at least the DCC_ functions need to be adapted.
;
;  This trap routine saves the processor registers in a data block at
;  Monitor_StackBase where the TRACE32 software will read it.
;  See the order below.
;
;  The halt reason will be stored at the first data block location.
;  Bit 0 will be set to signalize the Monitor_Handler that the TRACE32
;  software shall be informed that a debug event has happened. The 
;  Monitor_Handler sends the message and clears bit 0. At the beginning
;  this bit needs to be cleared (see monitor.cmm).
;
;  The monitor waits in a loop until TRACE32 causes a re-start of
;  the application. In the same loop memory accesses requested by 
;  TRACE32 will be serviced by calling the Monitor_Handler.
;  TRACE32 can modify the register values by writing to the data block.
;
;  If you enter 'DIAG 3800' in the TRACE32 command line, the communication
;  of the TRACE32 GUI with the Monitor_Handler will be printed in the 
;  'AREA' window.
;
;  Note that the monitor uses the DCC channel. Therefore it can not additionally 
;  be used for terminal or semihosting, except it is done based on a memory interface
;  instead of DCC.
;
;  Monitor_StackBase
;  + 0x00 entry reason, 'debug event just happened' flag (bit 0), re-start signal
;  + 0x04 R0
;  + 0x08 R1
;  + 0x0c R2
;  + 0x10 R3
;  + 0x14 R4
;  + 0x18 R5
;  + 0x1c R6
;  + 0x20 R7
;  + 0x24 R8_USR
;  + 0x28 R9_USR
;  + 0x2c R10_USR
;  + 0x30 R11_USR
;  + 0x34 R12_USR
;  + 0x38 R15
;  + 0x3c CPSR
;  + 0x40 R13_USR
;  + 0x44 R14_USR
;  + 0x48 R13_SVC
;  + 0x4c R14_SVC
;  + 0x50 SPSR_SVC
;  + 0x54 R8_FIQ
;  + 0x58 R9_FIQ
;  + 0x5c R10_FIQ
;  + 0x60 R11_FIQ
;  + 0x64 R12_FIQ
;  + 0x68 R13_FIQ
;  + 0x6c R14_FIQ
;  + 0x70 SPSR_FIQ
;  + 0x74 R13_IRQ
;  + 0x78 R14_IRQ
;  + 0x7c SPSR_IRQ
;  + 0x80 R13_ABT
;  + 0x84 R14_ABT
;  + 0x88 SPSR_ABT
;  + 0x8c DACR
;
;  R13_UND, R14_UND, SPSR_UND are used by the monitor and are
;  therefore of no importance for the application debugging.
;
;=================================================================

; entrypoint with information header for compile phase informations

  AREA  Monitor, CODE, READONLY

  EXPORT  Monitor_EntryRES
  EXPORT  Monitor_EntryUND
  EXPORT  Monitor_EntrySWI
  EXPORT  Monitor_EntryPABT
  EXPORT  Monitor_EntryDABT
  EXPORT  Monitor_EntryIRQ
  EXPORT  Monitor_EntryFIQ
  EXPORT  Monitor_Entry
  EXPORT  Monitor_Polling

  IMPORT  Monitor_Handler


;=================================================================
;  Sample interrupt vector table
;=================================================================

InitVectors LDR PC, _res_vec    ;0x00 Reset
            LDR PC, _und_vec    ;0x04 UNDEF
            LDR PC, _swi_vec    ;0x08 SWI
            LDR PC, _pabt_vec   ;0x0C PABORT
            LDR PC, _dabt_vec   ;0x10 DABORT
            LDR PC, _rsv_vec    ;0x14 reserved
            LDR PC, _irq_vec    ;0x18 IRQ
            LDR PC, _fiq_vec    ;0x1c FIQ

_res_vec  DCD Monitor_EntryRES    ;0x20
_und_vec  DCD Monitor_EntryUND    ;0x24
_swi_vec  DCD Monitor_EntrySWI    ;0x28
_pabt_vec DCD Monitor_EntryPABT   ;0x2c
_dabt_vec DCD Monitor_EntryDABT   ;0x30
_rsv_vec  DCD Monitor_EntrySWI    ;0x34
_irq_vec  DCD Monitor_EntryIRQ    ;0x38
_fiq_vec  DCD Monitor_EntryFIQ    ;0x3c



;=================================================================
;  Monitor Entry Points
;=================================================================

  IMPORT  Monitor_StackBase
Monitor_RegistersPtr
  DCD Monitor_StackBase


Monitor_EntryRES
  MRS R14, CPSR
  MSR CPSR_c, #0xdb     ; switch to undefined mode

  LDR R13, Monitor_RegistersPtr
  LDR R13, [R13]

  STR   R12, [SP, #0x34]
  MOV R12, #0x41        ; reason for entry is RESET

  STR R14, [SP, #0x3c]  ; CPSR

  MOV R14, #0
  STR R14, [SP, #0x38]  ; PC (=0)
  STR R7,  [SP, #0x20]

  B Monitor_Entry


Monitor_EntryUND
  STR   R12, [SP, #0x34]
  MOV R12, #0x11

  SUB R14, R14, #0x04
  STR R14, [SP, #0x38]  ; R14_und = PC
  MRS R14, SPSR
  STR R14, [SP, #0x3c]  ; SPSR_und = CPSR
  STR R7,  [SP, #0x20]

  B Monitor_Entry


Monitor_EntrySWI
  MSR CPSR_c, #0xDB     ; switch to undefined opcode mode
  STR R12, [SP, #0x34]  ; save r12
  STR R7,  [SP, #0x20]  ; save r7
  MOV R7, SP            ; load SP_und into R7

  MSR CPSR_c, #0xD3     ; switch back to SVC mode

  SUB R14, R14, #0x04   ; calculate and save PC
  STR R14, [R7, #0x38]

  MRS R12, SPSR         ; save spsr
  STR R12, [R7, #0x3c]

  MSR CPSR_c, #0xDB     ; switch to undefined opcode mode

  MOV R12, #0x11
  B Monitor_Entry


Monitor_EntryPABT
  MSR CPSR_c, #0xDB     ; switch to undefined opcode mode
  STR R12, [SP, #0x34]  ; save r12
  STR R7,  [SP, #0x20]  ; save r7
  MOV R7, SP            ; load SP_und into R7

  MSR CPSR_c, #0xD7     ; switch back to ABT mode

  SUB R14, R14, #0x04   ; calculate and save PC
  STR R14, [R7, #0x38]

  MRS R12, SPSR         ; save spsr
  STR R12, [R7, #0x3c]

  MSR CPSR_c, #0xDB     ; switch to undefined opcode mode

  MOV R12, #0x11
  B Monitor_Entry


Monitor_EntryDABT
  MSR CPSR_c, #0xDB     ; switch to undefined opcode mode

  LDR R13,Monitor_RegistersPtr
  LDR R13, [R13]

  LDR R14, [SP]
  CMP R14,#0            ; executed from inside monitor ?
  MOVNE R12, #0xf1
  STRNE R12, [SP]
  BNE Monitor_Polling   ; enter loop directly

  STR R12, [SP, #0x34]  ; save r12
  STR R7,  [SP, #0x20]  ; save r7
  MOV R7, SP            ; load SP_und into R7

  MSR CPSR_c, #0xD7     ; switch back to ABT mode

  SUB R14, R14, #0x04   ; calculate and save PC
  STR R14, [R7, #0x38]

  MRS R12, SPSR         ; save spsr
  STR R12, [R7, #0x3c]

  MSR CPSR_c, #0xDB     ; switch to undefined opcode mode

  MOV R12, #0x11
  B Monitor_Entry


Monitor_EntryIRQ
  MSR CPSR_c, #0xDB     ; switch to undefined opcode mode
  STR R12, [SP, #0x34]  ; save r12
  STR R7,  [SP, #0x20]  ; save r7
  MOV R7, SP            ; load SP_und into R7

  MSR CPSR_c, #0xD2     ; switch back to IRQ mode

  SUB R14, R14, #0x04   ; calculate and save PC
  STR R14, [R7, #0x38]

  MRS R12, SPSR         ; save spsr
  STR R12, [R7, #0x3c]

  MSR CPSR_c, #0xDB     ; switch to undefined opcode mode

  MOV R12, #0x11
  B Monitor_Entry


Monitor_EntryFIQ
  MSR CPSR_c, #0xDB     ; switch to undefined opcode mode
  STR R12, [SP, #0x34]  ; save r12
  STR R7,  [SP, #0x20]  ; save r7
  MOV R7, SP            ; load SP_und into R7

  MSR CPSR_c, #0xD1     ; switch back to FIQ mode

  SUB R14, R14, #0x04   ; calculate and save PC
  STR R14, [R7, #0x38]

  MRS R12, SPSR         ; save spsr
  STR R12, [R7, #0x3c]

  MSR CPSR_c, #0xDB     ; switch to undefined opcode mode

  MOV R12, #0x11
  B Monitor_Entry


;=================================================================
;  Breakpoint Entry
;=================================================================

Monitor_Entry
  STR R11, [SP, #0x30]
  STR R10, [SP, #0x2c]
  STR R9,  [SP, #0x28]
  STR R8,  [SP, #0x24]

  STR R6,  [SP, #0x1c]
  STR R5,  [SP, #0x18]
  STR R4,  [SP, #0x14]
  STR R3,  [SP, #0x10]
  STR R2,  [SP, #0x0c]
  STR R1,  [SP, #0x08]
  STR R0,  [SP, #0x04]


;==============================================================
  MRS R0, CPSR          ; switch to sys mode
  ORR R1, R0, #0x0F
  MSR CPSR_c, R1

  MOV R11, R13
  MOV R4, R14

  MSR CPSR_c, R0        ; switch back

  STR R11, [SP, #0x40]
  STR R4, [SP, #0x44]

;==============================================================
  MRS R0, CPSR          ; switch to svc mode
  ORR R1, R0, #0x03
  AND R1, R1, #0xF3
  MSR CPSR_c, R1

  MOV R11, R13
  MOV R4, R14
  MRS R5, SPSR

  MSR CPSR_c, R0        ; switch back

  STR R11, [SP, #0x48]
  STR R4, [SP, #0x4c]
  STR R5, [SP, #0x50]

;==============================================================
  MRS R0, CPSR          ; switch to fiq mode(1)
  ORR R1, R0, #0x01
  AND R1, R1, #0xf1
  MSR CPSR_c, R1

  MOV R3,  R8
  MOV R4,  R9
  MOV R5,  R10
  MOV R6,  R11

  MSR CPSR_c, R0        ; switch back

  STR R3, [SP, #0x54]
  STR R4, [SP, #0x58]
  STR R5, [SP, #0x5c]
  STR R6, [SP, #0x60]

  MRS R0, CPSR          ; switch to fiq mode(2)
  ORR R1, R0, #0x01
  AND R1, R1, #0xf1
  MSR CPSR_c, R1

  MOV R3,  R12
  MOV R4,  R13
  MOV R5,  R14
  MRS R6,  SPSR

  MSR CPSR_c, R0        ; switch back

  STR R3, [SP, #0x64]
  STR R4, [SP, #0x68]
  STR R5, [SP, #0x6c]
  STR R6, [SP, #0x70]

;==============================================================
  MRS R0, CPSR          ; switch to irq mode
  ORR R1, R0, #0x02
  AND R1, R1, #0xF2
  MSR CPSR_c, R1

  MOV R11, R13
  MOV R4, R14
  MRS R5, SPSR

  MSR CPSR_c, R0        ; switch back

  STR R11, [SP, #0x74]
  STR R4, [SP, #0x78]
  STR R5, [SP, #0x7c]

;==============================================================
  MRS R0, CPSR          ; switch to abt mode
  ORR R1, R0, #0x07
  AND R1, R1, #0xF7
  MSR CPSR_c, R1

  MOV R11, R13
  MOV R4, R14
  MRS R5, SPSR

  MSR CPSR_c, R0        ; switch back

  STR R11, [SP, #0x80]
  STR R4, [SP, #0x84]
  STR R5, [SP, #0x88]

;==============================================================

  IF :DEF: MONITOR_OVERWRITE_PROTCR ; same effect as 'SYStem.Option DACR ON' in halt mode debugging on ARM926EJ-S
  MRC p15, 0, r4, c3, c0
  STR R4, [SP, #0x8c]
  MOV R4, -1
  MCR p15, 0, r4, c3, c0
  ENDIF

;==============================================================

  STR R12, [SP]         ; save monitor entry reason code

Monitor_Polling
  BL  Monitor_Handler   ; wait for go and service memory read/write requests in the meantime
  LDR R12, [SP]
  CMP R12,#0
  BNE Monitor_Polling

;==============================================================

  IF :DEF: MONITOR_OVERWRITE_PROTCR
  LDR R4, [SP, #0x8c]
  MCR p15, 0, r4, c3, c0
  ENDIF


;==============================================================
  LDR R11, [SP, #0x40]
  LDR R4, [SP, #0x44]

  MRS R0, CPSR          ; switch to sys mode
  ORR R1, R0, #0x0F
  MSR CPSR_c, R1

  MOV R13, R11
  MOV R14, R4

  MSR CPSR_c, R0        ; switch back


;==============================================================
  LDR R11, [SP, #0x48]
  LDR R4, [SP, #0x4c]
  LDR R5, [SP, #0x50]

  MRS R0, CPSR          ; switch to svc mode
  ORR R1, R0, #0x03
  AND R1, R1, #0xF3
  MSR CPSR_c, R1

  MOV R13, R11
  MOV R14, R4
  MSR SPSR_cxsf, R5

  MSR CPSR_c, R0        ; switch back

;==============================================================
  LDR R3, [SP, #0x54]
  LDR R4, [SP, #0x58]
  LDR R5, [SP, #0x5c]
  LDR R6, [SP, #0x60]

  MRS R0, CPSR          ; switch to fiq mode(1)
  ORR R1, R0, #0x01
  AND R1, R1, #0xf1
  MSR CPSR_c, R1

  MOV R8, R3
  MOV R9, R4
  MOV R10, R5
  MOV R11, R6

  MSR CPSR_c, R0        ; switch back

  LDR R3, [SP, #0x64]
  LDR R4, [SP, #0x68]
  LDR R5, [SP, #0x6c]
  LDR R6, [SP, #0x70]

  MRS R0, CPSR          ; switch to fiq mode(2)
  ORR R1, R0, #0x01
  AND R1, R1, #0xf1
  MSR CPSR_c, R1

  MOV R12, R3
  MOV R13, R4
  MOV R14, R5
  MSR SPSR_cxsf, R6

  MSR CPSR_c, R0        ; switch back

;==============================================================
  LDR R11, [SP, #0x74]
  LDR R4, [SP, #0x78]
  LDR R5, [SP, #0x7c]

  MRS R0, CPSR          ; switch to irq mode
  ORR R1, R0, #0x02
  AND R1, R1, #0xF2
  MSR CPSR_c, R1

  MOV R13, R11
  MOV R14, R4
  MSR SPSR_cxsf, R5

  MSR CPSR_c, R0        ; switch back

;==============================================================
  LDR R11, [SP, #0x80]
  LDR R4, [SP, #0x84]
  LDR R5, [SP, #0x88]

  MRS R0, CPSR          ; switch to abt mode
  ORR R1, R0, #0x07
  AND R1, R1, #0xF7
  MSR CPSR_c, R1

  MOV R13, R11
  MOV R14, R4
  MSR SPSR_cxsf, R5

  MSR CPSR_c, R0        ; switch back

  LDR R0, [SP, #0x3c]   ; SPSR_und (=CPSR)
  MSR SPSR_cxsf, R0

  LDR R14, [SP, #0x38]  ; R14_und (=PC)
  LDR R0,  [SP, #0x04]  ; R0
  LDR R1,  [SP, #0x08]  ; R1
  LDR R2,  [SP, #0x0c]  ; R2
  LDR R3,  [SP, #0x10]  ; R3
  LDR R4,  [SP, #0x14]  ; R4
  LDR R5,  [SP, #0x18]  ; R5
  LDR R6,  [SP, #0x1c]  ; R6
  LDR R7,  [SP, #0x20]  ; R7
  LDR R8,  [SP, #0x24]  ; R8
  LDR R9,  [SP, #0x28]  ; R9
  LDR R10, [SP, #0x2c]  ; R10
  LDR R11, [SP, #0x30]  ; R11
  LDR R12, [SP, #0x34]  ; R12

  MOVS  PC, R14         ; return to application

;==============================================================

  END
