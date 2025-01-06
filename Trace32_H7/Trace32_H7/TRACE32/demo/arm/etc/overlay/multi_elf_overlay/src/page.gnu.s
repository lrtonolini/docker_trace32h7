
.section .text
.globl   SetContextID

SetContextID:
	# Writes passed argument in the context ID register 
	# (after a Data Synchronization Barrier).
	# This is only required if you want to record the 
	# program flow with your PowerTrace of your code, which 
	# contains code overlays
	mcr	%p15, #0, %r0, %c7, %c10, #4
	mcr	%p15, #0, %r0, %c13, %c0, #1
	bx 	%r14



.section .id
.globl   id

id:
	# A unique ID of the overlay segment
	# The ID will be placed right at the beginning of the
	# overlay segment.
	# With this trick you ensure that sYmbol.OVERLAY.AutoID
	# finds exactly this ID
	# We use here the storage address of the ELF, but
	# in fact it can be any number, which is unique for 
	# a particular overlay page
	.word	_text_store
