.file "isr.s"
.altmacro


.section .text

.macro isr_handler id
    __isr_handler_\id:
    iret
    # Error
    push $0
    # IRQ
    push $\id

    jmp __isr_handler

    .size __isr_handler_\id, . - __isr_handler_\id
.endm

.macro isr_handler_err id
    __isr_handler_\id:
    iret
    # Error
    push $\id
    # IRQ
    push $\id

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
    push %rbp
    mov %rsp, %rbp

    push %rsp
    push %rax
    push %rcx
    push %rdx
    push %rbx
    push %rbp
    push %rsi
    push %rdi

    # Error
    xor %rbx, %rbx
    mov 16(%rbp), %ebx
    push %rbx

    # IRQ
    xorq %rax, %rax
    mov 8(%rbp), %eax
    push %rax

    # Handler
    mov $__isr_handler_table, %rbx
    mov (%rbx, %rax, 8), %rax

    test %rax, %rax
    jz .skip_handler
    call *%rax

.skip_handler:
    pop %rax
    pop %rbx

    pop %rdi
    pop %rsi
    pop %rbp
    pop %rbx
    pop %rdx
    pop %rcx
    pop %rax
    pop %rsp

    mov %rbp, %rsp
    pop %rbp

    add $16, %rsp

    iret
.size __isr_handler, . - __isr_handler


.section .rodata

.macro isr_offset_entry id
    .quad __isr_handler_\id
.endm

.align 8
.global __isr_offset_table
__isr_offset_table:
n = 0
.rept 256
    isr_offset_entry %n
    n = n + 1
.endr
.size __isr_offset_table, . - __isr_offset_table


.section .data

.align 8
.global __isr_handler_table
__isr_handler_table:
.rept 256
    .quad 0
.endr
.size __isr_handler_table, . - __isr_handler_table
