

#if defined(__CC_ARM)
/* ARM RealView assembler (armasm) */
/* armasm --cpreproc ...*/
#  define SECTION(x,t1,t2) AREA |x|,t1
#  define ARM              CODE32
#  define THUMB            CODE16
#  define ALIGN(x)         ALIGN x
#  define LABEL(x)         x
#  define TYPE(x,t)
#  define GLOBAL(x)        GLOBAL x
#  define EXTERN(x)        EXTERN x
#  define WORD(x)          DCD x
#  define PRESERVE8        PRESERVE8
#  define MAKETHUMBADDR(x) x
#  define PUSH(x)          push x
#  define POP(x)           pop x
#elif defined(__GNUC__)
/* GNU assembler */
/* ARM CLANG assembler (armclang GNU syntax assember) */
/* gcc -xassembler-with-cpp ...*/
#  define SECTION(x,t1,t2) .section x,t2
#  define ARM              .arm
#  define THUMB            .thumb
#  define ALIGN(x)         .align x
#  define LABEL(x)         x:
#  define TYPE(x,t)        .type x,t
#  define GLOBAL(x)        .global x
#  define EXTERN(x)        .extern x
#  define WORD(x)          .word x
#  define PRESERVE8
#  define END              .end
#  define MAKETHUMBADDR(x) x+1
#  define PUSH(x)          push x
#  define POP(x)           pop x
#elif defined(MASM_EC2013)
/* MASM Windows Embedded Compact 2013 */
/* cpp -P -undef -DMASM_EC2013 -o tracedata.ec2013.asm tracedata.sx */
#  define SECTION(x,t1,t2) AREA    |x|,ALIGN=2,t1,READONLY,THUMB
#  define ALIGN(x)         
#  define LABEL(x)         x
#  define TYPE(x,t)
#  define GLOBAL(x)        GLOBAL x
#  define EXTERN(x)        
#  define WORD(x)          DCD x
#  define PRESERVE8     
#  define MAKETHUMBADDR(x) x
#  define PUSH(x)          push x
#  define POP(x)           pop x
#  define __thumb__
#elif defined(MASM_CE5_CE6_CE7)
/* cpp -P -undef -DMASM_CE5_CE6_CE7 -o tracedata.ce5_to_ce7.asm tracedata.sx */
#  define SECTION(x,t1,t2) AREA    |x|,ALIGN=4,t1,READONLY,ARM
#  define ALIGN(x)         
#  define LABEL(x)         x
#  define TYPE(x,t)
#  define GLOBAL(x)        GLOBAL x
#  define EXTERN(x)        
#  define WORD(x)          DCD x
#  define PRESERVE8
#  define PUSH(x)          stmfd r13!, x
#  define POP(x)           ldmfd r13!, x
#  define MAKETHUMBADDR(x) x
#  undef __thumb__
#else
*** ERROR ***
#endif



    SECTION(.text,CODE,"ax")
    GLOBAL(T32_DataTrace_Write32)
    GLOBAL(T32_DataTrace_Write8)
    GLOBAL(T32_DataTrace_Data8)
    GLOBAL(T32_DataTrace_Data32)
    GLOBAL(T32_DataTrace_Address)
    GLOBAL(T32_DataTrace_Data3)
    GLOBAL(T32_DataTrace_Data2)
    GLOBAL(T32_DataTrace_Data1)
    GLOBAL(T32_DataTrace_Data0)
    GLOBAL(T32_DataTrace_Address3)
    GLOBAL(T32_DataTrace_Address2)
    GLOBAL(T32_DataTrace_Address1)
    GLOBAL(T32_DataTrace_Address0)

#if defined(ARM)||defined(THUMB)
#ifdef __thumb__
    THUMB
#else
    ARM
#endif
#endif

LABEL(T32_DataTrace_Write32)
    PUSH({r4-r7})
    mov   r7, r14
    bl    T32_DataTrace_Data32
    bl    T32_DataTrace_Address
    mov   r14, r7
    POP({r4-r7})
    bx    r14
    
LABEL(T32_DataTrace_Write8)
    PUSH({r4-r7})
    mov   r7, r14
    bl    T32_DataTrace_Data8
    bl    T32_DataTrace_Address
    mov   r14, r7
    POP({r4-r7})
    bx    r14
    
LABEL(T32_DataTrace_Data8)
    mov   r6, #0xff
    and   r1, r1, r6
#ifdef __thumb__
    lsl   r1, r1, #0x1
#else
    mov   r1, r1, lsl #0x2
#endif
    ldr   r2, DataTraceData0
    add   r1, r1, r2
    mov   r2, r14
    bx    r1

LABEL(T32_DataTrace_Data32)
#ifdef __thumb__
    mov   r4, #0xff
    lsr   r5, r1, #16
    lsr   r3, r1, #8
    and   r5, r5, r4
    and   r3, r3, r4
    and   r4, r4, r1
    lsr   r1, r1, #24

    lsl   r1, r1, #0x1
    lsl   r5, r5, #0x1
    lsl   r3, r3, #0x1
    lsl   r4, r4, #0x1
#else
    mov   r4, #0xff
    mov   r5, r1, lsr #16
    mov   r3, r1, lsr #8
    and   r5, r5, r4
    and   r3, r3, r4
    and   r4, r4, r1
    mov   r1, r1, lsr #24

    mov   r1, r1, lsl #0x2
    mov   r5, r5, lsl #0x2
    mov   r3, r3, lsl #0x2
    mov   r4, r4, lsl #0x2
#endif

    ldr   r2, DataTraceData3
    add   r1, r1, r2
    ldr   r2, DataTraceData2
    add   r5, r5, r2
    ldr   r2, DataTraceData1
    add   r3, r3, r2
    ldr   r2, DataTraceData0
    add   r4, r4, r2

    mov   r2, r14

    bx    r1
    
    ALIGN(8)
#ifdef __thumb__
LABEL(DataTraceData3)
    WORD(MAKETHUMBADDR(T32_DataTrace_Data3))
LABEL(DataTraceData2)
    WORD(MAKETHUMBADDR(T32_DataTrace_Data2))
LABEL(DataTraceData1)
    WORD(MAKETHUMBADDR(T32_DataTrace_Data1))
LABEL(DataTraceData0)
    WORD(MAKETHUMBADDR(T32_DataTrace_Data0))
#else
LABEL(DataTraceData3)
    WORD(T32_DataTrace_Data3)
LABEL(DataTraceData2)
    WORD(T32_DataTrace_Data2)
LABEL(DataTraceData1)
    WORD(T32_DataTrace_Data1)
LABEL(DataTraceData0)
    WORD(T32_DataTrace_Data0)
#endif

LABEL(T32_DataTrace_Address)
#ifdef __thumb__
    mov   r4, #0xff
    lsr   r1, r0, #24
    lsr   r2, r0, #16
    lsr   r3, r0, #8
    and   r2, r2, r4
    and   r3, r3, r4
    and   r4, r4, r0

    lsl   r1, r1, #0x1
    lsl   r2, r2, #0x1
    lsl   r3, r3, #0x1
    lsl   r4, r4, #0x1
#else
    mov   r4, #0xff
    mov   r1, r0, lsr #24
    mov   r2, r0, lsr #16
    mov   r3, r0, lsr #8
    and   r2, r2, r4
    and   r3, r3, r4
    and   r4, r4, r0

    mov   r1, r1, lsl #0x2
    mov   r2, r2, lsl #0x2
    mov   r3, r3, lsl #0x2
    mov   r4, r4, lsl #0x2
#endif

    ldr   r0, DataTraceAddress3
    add   r1, r1, r0
    ldr   r0, DataTraceAddress2
    add   r2, r2, r0
    ldr   r0, DataTraceAddress1
    add   r3, r3, r0
    ldr   r0, DataTraceAddress0
    add   r4, r4, r0

    mov   r0, r14

    bx    r1
    
    ALIGN(8)
#ifdef __thumb__
LABEL(DataTraceAddress3)
    WORD(MAKETHUMBADDR(T32_DataTrace_Address3))
LABEL(DataTraceAddress2)
    WORD(MAKETHUMBADDR(T32_DataTrace_Address2))
LABEL(DataTraceAddress1)
    WORD(MAKETHUMBADDR(T32_DataTrace_Address1))
LABEL(DataTraceAddress0)
    WORD(MAKETHUMBADDR(T32_DataTrace_Address0))
#else
LABEL(DataTraceAddress3)
    WORD(T32_DataTrace_Address3)
LABEL(DataTraceAddress2)
    WORD(T32_DataTrace_Address2)
LABEL(DataTraceAddress1)
    WORD(T32_DataTrace_Address1)
LABEL(DataTraceAddress0)
    WORD(T32_DataTrace_Address0)
#endif

LABEL(T32_DataTrace_Data3)
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5
    bx    r5

LABEL(T32_DataTrace_Data2)
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3


LABEL(T32_DataTrace_Data1)
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4

    
LABEL(T32_DataTrace_Data0)
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2

LABEL(T32_DataTrace_Address3)
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2
    bx    r2

LABEL(T32_DataTrace_Address2)
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3
    bx    r3

LABEL(T32_DataTrace_Address1)
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4
    bx    r4

LABEL(T32_DataTrace_Address0)
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0
    bx    r0

    END

