.file "isr.s"
.altmacro

.section .text
.macro isr_handler id
    __isr_handler_\id:
    # Error
    pushl $0
    # IRQ
    pushl $\id

    jmp __isr_handler

    .size __isr_handler_\id, . - __isr_handler_\id
.endm

.macro isr_handler_err id
    __isr_handler_\id:
    # Error
    pushl $\id
    # IRQ
    pushl $\id

    jmp __isr_handler

    .size __isr_handler_\id, . - __isr_handler_\id
.endm

n = 0
.rept 8
    isr_handler %n
    n = n + 1
.endr
.rept 1
    isr_handler_err %n
    n = n + 1
.endr
.rept 1
    isr_handler %n
    n = n + 1
.endr
.rept 5
    isr_handler_err %n
    n = n + 1
.endr

.rept 256 - (5 + 1 + 1 + 8)
    isr_handler %n
    n = n + 1
.endr

__isr_handler:
    push %ebp
    mov %esp, %ebp

    pusha

    # Error
    mov 8(%ebp), %ebx
    push %ebx

    # IRQ
    mov 4(%ebp), %eax
    push %eax

    # Handler
    mov $__isr_handler_table, %ebx
    mov (%ebx, %eax, 4), %eax

    test %eax, %eax
    jz .skip_handler
    call *%eax

.skip_handler:
    pop %eax
    pop %ebx

    popa

    mov %ebp, %esp
    pop %ebp

    add $8, %esp

    iret
.size __isr_handler, . - __isr_handler


.section .rodata

.macro isr_offset_entry id
    .long __isr_handler_\id
.endm

.align 4
.global __isr_offset_table
__isr_offset_table:
n = 0
.rept 256
    isr_offset_entry %n
    n = n + 1
.endr
.size __isr_offset_table, . - __isr_offset_table

.align 1
debug_message:
.asciz "Eax: %x\n"
.size debug_message, . - debug_message

.section .data

.align 4
.global __isr_handler_table
__isr_handler_table:
.rept 256
    .long 0
.endr
.size __isr_handler_table, . - __isr_handler_table
