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
	GLOBAL GetContextID 
	
SetContextID
	; Writes passed argument in the context ID register 
	; (after a Data Synchronization Barrier).
	mcr	p15, #0, r0, c7, c10, #4 ; Data Synchronization Barrier
	mcr	p15, #0, r0, c13, c0, #1 ; Write argument (r0) to ContextID reg.
	bx 	r14
	
GetContextID
	; Reads context ID register 
	mrc p15, #0, r0, c13, c0, #1
	bx 	r14


	AREA |.rodata|, DATA, READONLY, ALIGN=3

	; Overlay informations

	GLOBAL PageA0 
PageA0
	EXTERN ||Image$$.pageA0$$Base||
	EXTERN ||Load$$.pageA0$$Base||
	EXTERN ||Image$$.pageA0$$Length||
	DCD	||Image$$.pageA0$$Base||
	DCD	||Load$$.pageA0$$Base||
	DCD	||Load$$.pageA0$$Base||+||Image$$.pageA0$$Length||
	DCD 0x01, 0x0f

	GLOBAL PageA1 
PageA1
	EXTERN ||Image$$.pageA1$$Base||
	EXTERN ||Load$$.pageA1$$Base||
	EXTERN ||Image$$.pageA1$$Length||
	DCD	||Image$$.pageA1$$Base||
	DCD	||Load$$.pageA1$$Base||
	DCD	||Load$$.pageA1$$Base||+||Image$$.pageA1$$Length||
	DCD 0x02, 0x0f
	GLOBAL PageB0 

PageB0
	EXTERN ||Image$$.pageB0$$Base||
	EXTERN ||Load$$.pageB0$$Base||
	EXTERN ||Image$$.pageB0$$Length||
	DCD	||Image$$.pageB0$$Base||
	DCD	||Load$$.pageB0$$Base||
	DCD	||Load$$.pageB0$$Base||+||Image$$.pageB0$$Length||
	DCD 0x10, 0xf0
	GLOBAL PageB1 

PageB1
	EXTERN ||Image$$.pageB1$$Base||
	EXTERN ||Load$$.pageB1$$Base||
	EXTERN ||Image$$.pageB1$$Length||
	DCD	||Image$$.pageB1$$Base||
	DCD	||Load$$.pageB1$$Base||
	DCD	||Load$$.pageB1$$Base||+||Image$$.pageB1$$Length||
	DCD 0x20, 0xf0


	; A unique ID of the overlay segment
	; The ID will be placed right at the beginning of the overlay segment.
	; With this trick you ensure that sYmbol.OVERLAY.AutoID finds exactly this ID
	AREA |.PageA0|, CODE, READONLY, ALIGN=3
	GLOBAL idPageA0 [DATA]
idPageA0 DCD 0xA0

	AREA |.PageA1|, CODE, READONLY, ALIGN=3
	GLOBAL idPageA1 [DATA]
idPageA1 DCD 0xA1

	AREA |.PageB0|, CODE, READONLY, ALIGN=3
	GLOBAL idPageB0 [DATA]
idPageB0 DCD 0xB0

	AREA |.PageB1|, CODE, READONLY, ALIGN=3
	GLOBAL idPageB1 [DATA]
idPageB1 DCD 0xB1

	END