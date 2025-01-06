	PRESERVE8
	AREA |.text|, CODE, ALIGN=3
	CODE32
	EXTERN main
	EXTERN ||Image$$.bss$$ZI$$Base||
	EXTERN ||Image$$.bss$$ZI$$Limit||
	EXTERN ||Image$$.stack$$ZI$$Limit||
	GLOBAL __main
	ENTRY
__main 
	; initialize stack pointer
	ldr	sp, _SP
	mov	fp, #0

	; clear .bss-section
	ldr	r1, BSS_S
	ldr	r2, BSS_E	
	subs	r2, r2, r1
	beq	gomain
	mov 	r0, #0
bss_clear
	strb	r0, [r1]
	subs	r2, r2, #1
	add	r1, r1, #1
	bne	bss_clear
gomain
	bl	main
_end
	b	_end

BSS_S	DCD	||Image$$.bss$$ZI$$Base||
BSS_E	DCD	||Image$$.bss$$ZI$$Limit||
_SP	DCD	||Image$$.stack$$ZI$$Limit||


	AREA |.text|, CODE, ALIGN=3
	CODE32
	GLOBAL SetContextID 
SetContextID
	; Writes passed argument in the context ID register 
	; (after a Data Synchronization Barrier).
	; This is only required if you want to record the 
	; program flow with your PowerTrace of your code, which 
	; contains code overlays
	mcr	p15, #0, r0, c7, c10, #4 ; Data Synchronization Barrier
	mcr	p15, #0, r0, c13, c0, #1 ; Write argument (r0) to ContextID reg.
	bx 	r14


	AREA |.rodata|, DATA, READONLY, ALIGN=3

	; Overlay informations
	EXTERN ||Image$$.page1$$Base||
	EXTERN ||Load$$.page1$$Base||
	EXTERN ||Image$$.page1$$Length||
	EXTERN ||Image$$.page2$$Base||
	EXTERN ||Load$$.page2$$Base||
	EXTERN ||Image$$.page2$$Length||
	GLOBAL Page1 
	GLOBAL Page2 
Page1
	DCD	||Image$$.page1$$Base||
	DCD	||Load$$.page1$$Base||
	DCD	||Load$$.page1$$Base||+||Image$$.page1$$Length||
Page2
	DCD	||Image$$.page2$$Base||
	DCD	||Load$$.page2$$Base||
	DCD	||Load$$.page2$$Base||+||Image$$.page2$$Length||


	AREA |.page1|, CODE, READONLY, ALIGN=3
	GLOBAL id1 [DATA]
	; A unique ID of the overlay segment
	; The ID will be placed right at the beginning of the overlay segment.
	; With this trick you ensure that sYmbol.OVERLAY.AutoID finds exactly this ID
id1	DCD	0x41

	AREA |.page2|, CODE, READONLY, ALIGN=3
	GLOBAL id2 [DATA]
id2	DCD	0x42


	END