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



	AREA |.id|, CODE, READONLY, ALIGN=3
	EXTERN ||Load$$TEXT$$Base||
	GLOBAL id [DATA]

	; A unique ID of the overlay segment
	; The ID will be placed right at the beginning of the
	; overlay segment.
	; With this trick you ensure that sYmbol.OVERLAY.AutoID
	; finds exactly this ID
	; We use here the storage address of the ELF, but
	; in fact it can be any number, which is unique for 
	; a particular overlay page
id	DCD	||Load$$TEXT$$Base||


	END