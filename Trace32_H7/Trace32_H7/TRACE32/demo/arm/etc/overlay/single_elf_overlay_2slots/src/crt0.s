.text
.globl  _start
.globl  _exit

#ifdef __thumb__
# define subs sub
#endif


_start:
	/* set stack and frame pointer */
	ldr  	r1, SP
	mov  	r0, #0
	mov  	sp, r1
	mov  	fp, r0

	/* clear .bss-section */
	ldr  	r4, BSS_S
	ldr  	r5, BSS_E
	subs 	r5, r5, r4
	beq  	gomain
	mov  	r0, #0
bss_clear:
	strb 	r0, [r4]
	add  	r4, r4, #1
	subs 	r5, r5, #1
	bne  	bss_clear
gomain:
	mov  	r0, #0
	mov  	r1, #0
	bl   	main
_exit:
	b    	_exit


.align  2
BSS_S:
	.long	__bss_start
BSS_E:
	.long	__bss_end
SP:
	.long	__stack_end


.section .text
.globl   SetContextID
.globl   GetContextID

SetContextID:
	# Writes passed argument in the context ID register
	# (after a Data Synchronization Barrier).
	mcr	%p15, #0, %r0, %c7, %c10, #4
	mcr	%p15, #0, %r0, %c13, %c0, #1
	bx 	%r14

GetContextID:
	# Reads context ID register
	mrc p15, #0, r0, c13, c0, #1
	bx 	%r14
