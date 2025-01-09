;*****************************************************************************
; *                                                                           *
; *        Test application for DCC communication on armv8 processor.         * 
; *        This test is intended to be used with the AArch64 mode.            *
; *                        Lauterbach GmbH (2012)                             *
; *                                                                           *
; *****************************************************************************/

	AREA ARMex, CODE, READONLY	
	ENTRY		

string  DCB	"Hello world!  \n\r"	

start
	MOV x3, #0x0             ; status control
	MOV x5, #0x0             ; masked control bits
	MOV x2, #0x0             ; Used for data transfer
	ADR x1, string
	MOV x4, #0x3             ; Wait counter
	MRS x3, MDSCR_EL1        ;read comms control reg
	AND x5, x3,#0x40000000   ; Check RX, i.e. wait for debugger
	CBZ x5, checkin
        MRS x2, DBGDTR_EL0       ;read data to empty DTR, if there is something in there
	B checkin

checkout
	
	ADR x1, string
	MOV x4, #0x10
loop
	MRS x3, MDSCR_EL1        ;read comms control reg
	AND x5, x3,#0x20000000   ; Check TX, i.e. wait to send to debugger
	CBNZ x5, loop
	
	LDRB w2, [x1], #1
	MSR DBGDTR_EL0, x2       ; write data
	SUB	x4, x4, #1
	CBNZ x4, loop
	
	MOV x4, #0x3             ; Reload counter for next wait
	b checkin

checkin

	MRS x3, MDSCR_EL1        ;read comms control reg
	AND x5, x3,#0x40000000   ; Check RX, i.e. wait for debugger
	CBZ x5, checkin
        MRS x2, DBGDTR_EL0       ;read data
        SUB	x4, x4, #1
        CBNZ x4, checkin
        B checkout
        
        END

