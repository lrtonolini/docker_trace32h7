.text
.global func3
.type   func3, %function
.func   func3

func3:
	mov	%r0, #5
	mov	%pc, %lr

.endfunc
.size func3,.-func3
