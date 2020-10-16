.file "isr.s"
.altmacro

.section .text

.macro isr_impl id
    isr\id:
    pusha
    pushl $\id
    jmp isr_handler
.endm

n = 0
.rept 256
    isr_impl %n
    n = n + 1
.endr

isr_handler:
    pushl $format_string
    call printf

    popa

    iret

.section .rodata

.macro isr_table_entry id
    .long isr\id
.endm

.align 4
.global __isr_handler_table
__isr_handler_table:

n = 0
.rept 256
    isr_table_entry %n
    n = n + 1
.endr

.size __isr_handler_table, . - __isr_handler_table

.align 1
format_string:
.asciz "%d "
.size format_string, . - format_string
