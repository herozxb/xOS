[bits 32]

[ global load_gdt ]
[ extern _gdtr ]

load_gdt:
	lgdt [ _gdtr ]
	mov ax, 0x10                ; kernel data selector
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	jmp 0x08:load_gdt_ret
load_gdt_ret:
	ret


[ global load_idt ]
;[ global keyboard_handler ]
[ global pit_handler ]
[ global sys_interrupt_handler ]
[ global sys_pthread_handler ]
[ global sys_file_handler ]
[ extern keyboard_handler_main ]
[ extern pit_handler_main ]
[ extern sys_interrupt_handler_main ]
[ extern sys_pthread_handler_main ]
[ extern sys_file_handler_main ]


load_idt:
	mov edx, [ esp + 4 ]
	lidt [ edx ]        ; load interrupt description table (IDT)
	sti                 ; turn on interrupts
	ret

;keyboard_handler:
;	cli
;	call    keyboard_handler_main
;	sti
;	iretd               ; 32-bit return

;pit_handler:            ; handle process switching
;	cli
;	jmp pit_handler_entry ; DO NOT USE CALL!!! WILL DESTROY STACK
;pit_handler_entry_ret:
;	sti
;	iretd

;sys_interrupt_handler:
;	cli
;	jmp save_proc_entry
;save_proc_entry_ret:
;	push eax            ; Functional number
;	call sys_interrupt_handler_main
;	pop ebx             ; Remember to pop out
	; eax store the return value
;	sti
;	iretd

;sys_pthread_handler:
;	cli
;	pusha ; ax,cx,dx,bx,sp,bp,si,di
;	push ds
;	push es
;	push fs
;	push gs

;	call save_proc

;	pop gs
;	pop fs
;	pop es
;	pop ds
;	popa

;	push eax            ; Functional number
;	call sys_pthread_handler_main
;	pop ebx             ; Remember to pop out
	; eax store the return value
;	sti
;	iretd

;sys_file_handler:
;	cli
;	pusha ; ax,cx,dx,bx,sp,bp,si,di
;	push ds
;	push es
;	push fs
;	push gs

;	call save_proc

;	pop gs
;	pop fs
;	pop es
;	pop ds
;	popa

;	push eax            ; Functional number
;	call sys_file_handler_main
;	pop ebx             ; Remember to pop out
	; eax store the return value
;	sti
;	iretd
	
	

[ global pit_handler_entry ]
[ extern save_proc ]
[ global restart_proc ]

;;;;;
; From Intel manual p2880:
; If the handler procedure is going to be executed
; at the same privilege level as the interrupted procedure:
; a. The processor saves the current state of the
;    EFLAGS, CS, and EIP registers on the current stack
; b. If an exception causes an error code to be saved,
;    it is pushed on the current stack after the EIP value
;
; If the handler procedure is going to be executed at a
; numerically lower privilege level, a stack switch occurs.
; When the stack switch occurs:
; a. The segment selector and stack pointer for the stack
;    to be used by the handler are obtained from the TSS
;    for the currently executing task.
;    On this new stack, the processor pushes
;    the stack segment selector and stack pointer
;    of the interrupted procedure.
; b. The processor then saves the current state of the
;    EFLAGS, CS, and EIP registers on the new stack
; c. If an exception causes an error code to be saved,
;    it is pushed on the new stack after the EIP value.
;;;;;
; STACK CHANGE! (different privilege level)
; | ss     |
; | esp    | ; user stack esp!!!
; NO STACK CHANGE! (the same privilege level)
; | eflags |
; | cs     |
; | eip    |
; NO ERROR CODE!
; | ax     |
; | cx     |
; | dx     |
; | bx     |
; | sp     | ; kernel stack esp
; | bp     |
; | si     |
; | di     |
; | ds     |
; | es     |
; | fs     |
; | gs     |
; | ret-add| ; pit_handler_main
;pit_handler_entry:
;	pusha ; ax,cx,dx,bx,sp,bp,si,di
;	push ds
;	push es
;	push fs
;	push gs

;	call pit_handler_main

;	pop gs
;	pop fs
;	pop es
;	pop ds
;	popa
;	jmp pit_handler_entry_ret

;save_proc_entry:
;	pusha ; ax,cx,dx,bx,sp,bp,si,di
;	push ds
;	push es
;	push fs
;	push gs

;	call save_proc

;	pop gs
;	pop fs
;	pop es
;	pop ds
;	popa
;	jmp save_proc_entry_ret

; | ss     | ; esp+16
; | esp    | ; esp+12
; | eflags | ; esp+8
; | cs     | ; esp+4
; | eip    | ; esp (after `popa`)
; | eax    |
; | ecx    |
; | edx    |
; | ebx    |
; | esp    |
; | ebp    |
; | esi    |
; | edi    |
; | ds     |
; | es     |
; | fs     |
; | gs     | <- now esp
; | ret-add|
restart_proc:
	cli
	add esp, 4   ; skip return address
	pop gs
	pop fs
	pop es
	pop ds
	popa
	iretd ; flush cs:eip

;;;;; read process info ;;;;;

[ global read_eip ]

read_eip:
	pop eax ; get return address
	jmp eax ; return by jmp

testdata:
	msg db "This is a test message!", 0
	
	

[extern main] ; Define calling point. Must have same name as kernel.c 'main' function
call main ; Calls the C function. The linker will know where it is placed in memory
jmp $




	
	
        
        
