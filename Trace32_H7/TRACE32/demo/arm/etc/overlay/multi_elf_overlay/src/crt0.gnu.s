.text
.globl  _start


_start:
	# clear context ID register 
	mov 	%r0, #0
	mcr	%p15, #0, %r0, %c13, %c0, #1

	# initialize stack pointer
	ldr	%sp, .SP
	mov	%fp, #0

	# clear .bss-section
	ldr	%r1, .BSS_S
	ldr	%r2, .BSS_E	
	subs	%r2, %r2, %r1
	beq	gomain
	mov 	%r0, #0
bss_clear:
	strb	%r0, [%r1]
	subs	%r2, %r2, #1
	add	%r1, %r1, #1
	bne	bss_clear
gomain:
	bl	main
_end:
	b	_end


.BSS_S:
	.word	_bss_start
.BSS_E:
	.word	_bss_end
.SP:
	.word	_sp
