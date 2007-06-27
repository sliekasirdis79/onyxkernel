[BITS 32]

; This will set up our new segment registers. We need to do
; something special in order to set CS. We do what is called a
; far jump. A jump that includes a segment as well as an offset.
; This is declared in C as 'extern void gdtflush();'
global _gdt_flush
extern _gp
_gdt_flush:
    lgdt [_gp]
	;mov ax, 0x10
    ;mov ds, ax
    ;mov es, ax
    ;mov fs, ax
    ;mov gs, ax
    ;mov ss, ax
    jmp 0x08:flush2
flush2:
    ret
    
global _read_cr0
_read_cr0:
	mov eax, cr0
	retn

global _write_cr0
_write_cr0:
	push ebp
	mov ebp, esp
	mov eax, [ebp+8]
	mov cr0,  eax
	pop ebp
	retn

global _read_cr3
_read_cr3:
	mov eax, cr3
	retn

global _write_cr3
_write_cr3:
	push ebp
	mov ebp, esp
	mov eax, [ebp+8]
	mov cr3, eax
	pop ebp
	retn

; Loads the IDT defined in 'idtp' into the processor.
; This is declared in C as 'extern void idtload();'
global _idt_load
extern _idtp
_idt_load:
    lidt [_idtp]
    ret

; In just a few pages in this tutorial, we will add our Interrupt
; Service Routines (_isrs) right here!
global _isr0
global _isr1
global _isr2
global _isr3
global _isr4
global _isr5
global _isr6
global _isr7
global _isr8
global _isr9
global _isr10
global _isr11
global _isr12
global _isr13
global _isr14
global _isr15
global _isr16
global _isr17
global _isr18
global _isr19
global _isr20
global _isr21
global _isr22
global _isr23
global _isr24
global _isr25
global _isr26
global _isr27
global _isr28
global _isr29
global _isr30
global _isr31

;  0: Divide By Zero Exception
_isr0:
    cli
    push byte 0
    push byte 0
    jmp _isrcommonstub

;  1: Debug Exception
_isr1:
    cli
    push byte 0
    push byte 1
    jmp _isrcommonstub

;  2: Non Maskable Interrupt Exception
_isr2:
    cli
    push byte 0
    push byte 2
    jmp _isrcommonstub

;  3: Int 3 Exception
_isr3:
    cli
    push byte 0
    push byte 3
    jmp _isrcommonstub

;  4: INTO Exception
_isr4:
    cli
    push byte 0
    push byte 4
    jmp _isrcommonstub

;  5: Out of Bounds Exception
_isr5:
    cli
    push byte 0
    push byte 5
    jmp _isrcommonstub

;  6: Invalid Opcode Exception
_isr6:
    cli
    push byte 0
    push byte 6
    jmp _isrcommonstub

;  7: Coprocessor Not Available Exception
_isr7:
    cli
    push byte 0
    push byte 7
    jmp _isrcommonstub

;  8: Double Fault Exception (With Error Code!)
_isr8:
    cli
    push byte 0
    push byte 8
    jmp _isrcommonstub

;  9: Coprocessor Segment Overrun Exception
_isr9:
    cli
    push byte 0
    push byte 9
    jmp _isrcommonstub

; 10: Bad TSS Exception (With Error Code!)
_isr10:
    cli
    push byte 10
    jmp _isrcommonstub

; 11: Segment Not Present Exception (With Error Code!)
_isr11:
    cli
    push byte 11
    jmp _isrcommonstub

; 12: Stack Fault Exception (With Error Code!)
_isr12:
    cli
    push byte 12
    jmp _isrcommonstub

; 13: General Protection Fault Exception (With Error Code!)
_isr13:
    cli
    push byte 13
    jmp _isrcommonstub

; 14: Page Fault Exception (With Error Code!)
_isr14:
    cli
    push byte 14
    jmp _isrcommonstub

; 15: Reserved Exception
_isr15:
    cli
    push byte 0
    push byte 15
    jmp _isrcommonstub

; 16: Floating Point Exception
_isr16:
    cli
    push byte 0
    push byte 16
    jmp _isrcommonstub

; 17: Alignment Check Exception
_isr17:
    cli
    push byte 0
    push byte 17
    jmp _isrcommonstub

; 18: Machine Check Exception
_isr18:
    cli
    push byte 0
    push byte 18
    jmp _isrcommonstub

; 19: Reserved
_isr19:
    cli
    push byte 0
    push byte 19
    jmp _isrcommonstub

; 20: Reserved
_isr20:
    cli
    push byte 0
    push byte 20
    jmp _isrcommonstub

; 21: Reserved
_isr21:
    cli
    push byte 0
    push byte 21
    jmp _isrcommonstub

; 22: Reserved
_isr22:
    cli
    push byte 0
    push byte 22
    jmp _isrcommonstub

; 23: Reserved
_isr23:
    cli
    push byte 0
    push byte 23
    jmp _isrcommonstub

; 24: Reserved
_isr24:
    cli
    push byte 0
    push byte 24
    jmp _isrcommonstub

; 25: Reserved
_isr25:
    cli
    push byte 0
    push byte 25
    jmp _isrcommonstub

; 26: Reserved
_isr26:
    cli
    push byte 0
    push byte 26
    jmp _isrcommonstub

; 27: Reserved
_isr27:
    cli
    push byte 0
    push byte 27
    jmp _isrcommonstub

; 28: Reserved
_isr28:
    cli
    push byte 0
    push byte 28
    jmp _isrcommonstub

; 29: Reserved
_isr29:
    cli
    push byte 0
    push byte 29
    jmp _isrcommonstub

; 30: Reserved
_isr30:
    cli
    push byte 0
    push byte 30
    jmp _isrcommonstub

; 31: Reserved
_isr31:
    cli
    push byte 0
    push byte 31
    jmp _isrcommonstub


; We call a C function in here. We need to let the assembler know
; that 'faulthandler' exists in another file
extern _fault_handler

; This is our common _isr stub. It saves the processor state, sets
; up for kernel mode segments, calls the C-level fault handler,
; and finally restores the stack frame.
_isrcommonstub:
    pusha
    push ds
    push es
    push fs
    push gs
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp
    push eax
    mov eax, _fault_handler
    call eax
    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8
    iret

global _irq0
global _irq1
global _irq2
global _irq3
global _irq4
global _irq5
global _irq6
global _irq7
global _irq8
global _irq9
global _irq10
global _irq11
global _irq12
global _irq13
global _irq14
global _irq15

; 32: _irq0
_irq0:
    cli
    push byte 0
    push byte 32
    jmp _irqcommonstub

; 33: _irq1
_irq1:
    cli
    push byte 0
    push byte 33
    jmp _irqcommonstub

; 34: _irq2
_irq2:
    cli
    push byte 0
    push byte 34
    jmp _irqcommonstub

; 35: _irq3
_irq3:
    cli
    push byte 0
    push byte 35
    jmp _irqcommonstub

; 36: _irq4
_irq4:
    cli
    push byte 0
    push byte 36
    jmp _irqcommonstub

; 37: _irq5
_irq5:
    cli
    push byte 0
    push byte 37
    jmp _irqcommonstub

; 38: _irq6
_irq6:
    cli
    push byte 0
    push byte 38
    jmp _irqcommonstub

; 39: _irq7
_irq7:
    cli
    push byte 0
    push byte 39
    jmp _irqcommonstub

; 40: _irq8
_irq8:
    cli
    push byte 0
    push byte 40
    jmp _irqcommonstub

; 41: _irq9
_irq9:
    cli
    push byte 0
    push byte 41
    jmp _irqcommonstub

; 42: _irq10
_irq10:
    cli
    push byte 0
    push byte 42
    jmp _irqcommonstub

; 43: _irq11
_irq11:
    cli
    push byte 0
    push byte 43
    jmp _irqcommonstub

; 44: _irq12
_irq12:
    cli
    push byte 0
    push byte 44
    jmp _irqcommonstub

; 45: _irq13
_irq13:
    cli
    push byte 0
    push byte 45
    jmp _irqcommonstub

; 46: _irq14
_irq14:
    cli
    push byte 0
    push byte 46
    jmp _irqcommonstub

; 47: _irq15
_irq15:
    cli
    push byte 0
    push byte 47
    jmp _irqcommonstub

extern _irq_handler

_irqcommonstub:
    pusha
    push ds
    push es
    push fs
    push gs

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp

    push eax
    mov eax, _irq_handler
    call eax
    pop eax

    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8
    iret
