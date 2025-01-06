;*****************************************************************************
; *                                                                           *
; *        Test application for DCC communication on armv8 processor.         * 
; *        This test is intended to be used with the AArch32 ARM mode.        *
; *                        Lauterbach GmbH (2012)                             *
; *                                                                           *
; *****************************************************************************/

	AREA ARMex, CODE, READONLY	
	ENTRY

string  DCB	"Hello world!  \n\r"	

start
	MOV 	r3, #0x0             	; status control
	MOV 	r5, #0x0             	; masked control bits
	MOV 	r2, #0x0             	; Used for data transfer
	ADR 	r1, string
	MOV 	r4, #0x3             	; Wait counter
	MRC 	p14, 0, r3, c0, c1   	; read comms control reg
	AND 	r5, r3,#0x40000000   	; Check RX, i.e. wait for debugger
	CMP 	r5, #0x0
	BEQ 	checkin
        MRC	p14, 0, r2, c0, c5	; read data to empty DTR, if there is something in there
	B 	checkin

checkout
	
	ADR 	r1, string
	MOV 	r4, #0x10
loop
	MRC 	p14, 0, r3, c0, c1    	; read comms control reg
	AND 	r5, r3,#0x20000000   	; Check TX, i.e. wait to send to debugger
	CMP 	r5, #0x0
	BNE 	loop
	
	LDRB 	r2, [r1], #1
	MCR	p14, 0, r2, c0, c5	; write data
	SUB	r4, r4, #1
	CMP	r4, #0x0
	BNE 	loop
	
	MOV 	r4, #0x3             	; Reload counter for next wait
	B 	checkin

checkin

	MRC 	p14, 0, r3, c0, c1  	; read comms control reg
	AND 	r5, r3,#0x40000000 	; Check RX, i.e. wait for debugger
	CMP 	r5, #0x0
	BEQ	checkin
        MRC	p14, 0, r2, c0, c5  	;read data
        SUB	r4, r4, #1
        CMP	r4, #0x0
        BNE 	checkin
        B 	checkout
        
        END

