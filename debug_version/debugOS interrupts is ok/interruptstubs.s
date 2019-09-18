  

.set IRQ_BASE, 0x20

.section .text

.extern test
.extern test1
.extern test2


.extern HandleInterrupt



#_Z19HandleException0x00v

.macro HandleException num

.global _Z19HandleException\num\()v
_Z19HandleException\num\()v:
    call test
    movb $\num, (interruptnumber)
    jmp int_bottom
.endm


#_Z26HandleInterruptRequest0x00v

.macro HandleInterruptRequest num

.global _Z26HandleInterruptRequest\num\()v
_Z26HandleInterruptRequest\num\()v:
    call test1
    movb $\num + IRQ_BASE, (interruptnumber)
    jmp int_bottom
.endm


HandleException 0x00
HandleException 0x01
HandleException 0x02
HandleException 0x03
HandleException 0x04
HandleException 0x05
HandleException 0x06
HandleException 0x07
HandleException 0x08
HandleException 0x09
HandleException 0x0A
HandleException 0x0B
HandleException 0x0C
HandleException 0x0D
HandleException 0x0E
HandleException 0x0F
HandleException 0x10
HandleException 0x11
HandleException 0x12
HandleException 0x13

HandleInterruptRequest 0x00
HandleInterruptRequest 0x01
HandleInterruptRequest 0x02
HandleInterruptRequest 0x03
HandleInterruptRequest 0x04
HandleInterruptRequest 0x05
HandleInterruptRequest 0x06
HandleInterruptRequest 0x07
HandleInterruptRequest 0x08
HandleInterruptRequest 0x09
HandleInterruptRequest 0x0A
HandleInterruptRequest 0x0B
HandleInterruptRequest 0x0C
HandleInterruptRequest 0x0D
HandleInterruptRequest 0x0E
HandleInterruptRequest 0x0F
HandleInterruptRequest 0x31





int_bottom:

    # register sichern
    pusha
    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs

    # ring 0 segment register laden
    #cld
    #mov $0x10, %eax
    #mov %eax, %eds
    #mov %eax, %ees

    # C++ Handler aufrufen
    pushl %esp
    push (interruptnumber)

    call test2
    call HandleInterrupt


    add %esp, 6
    mov %eax, %esp # den stack wechseln




    # register laden
    pop %gs
    pop %fs
    pop %es
    pop %ds
    popa

.global _Z15InterruptIgnorev
_Z15InterruptIgnorev:
    iret


.data
    interruptnumber: .byte 0
