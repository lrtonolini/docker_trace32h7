;=================================================================
;=== Benchmark entry for ARM benchmark
;=== 
;=== 
;=================================================================

; entrypoint with information header for compile phase informations

	AREA	Vectors, CODE, READONLY

	EXPORT	__entry
	IMPORT	BenchmarkTest

;=================================================================
;=	Sample interrupt vector table
;=================================================================

	ENTRY

__entry	
	add	r9,r0,#0x200		;pointer for PI-Data
	b	BenchmarkTest
	nop
	nop				;just to maintain 16byte alignment

	END

