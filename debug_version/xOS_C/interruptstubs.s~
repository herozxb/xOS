

.set IRQ_BASE, 0x20

.section .text

.extern test
.extern test1
.extern test2
.extern debug


.global _Z5debugv

_Z5debugv:

    call test1
    #jmp int_bottom




.global  _Z19HandleException0x9Fv
_Z19HandleException0x9Fv:
	call test2

_stop:
    call test
    cli
    hlt
    jmp _stop

#.macro HandleException num
#.global _ZL19HandleException\num\()v
#_ZL19HandleException\num\()v:
#    movb $\num, (interruptnumber)
#    call test1
#    jmp int_bottom
#.endm


#HandleException 0x00



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
    call test
    add %esp, 6
    #call test2
    mov %eax, %esp # den stack wechseln


    # register laden
    pop %gs
    pop %fs
    pop %es
    pop %ds
    popa


.global debug

debug:

    call test1

.data
    interruptnumber: .byte 0
