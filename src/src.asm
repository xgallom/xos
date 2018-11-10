    ; 3.ASM
    ; Load a program off the disk and jump to it

    ; Tell the compiler that this is offset 0.
    ; It isn't offset 0, but it will be after the jump.

    %define data_stack  0x0050      ; 0x0500 start of usable memory
    %define call_stack  0x02d0      ; 0x2d00 10kB later
    %define trans_stack 0x0550      ; 0x5500 10kB later
                                    ;       :0 = data_stack current index
                                    ;       :2 = call_stack current index

    [ORG 0]
    bits 16

            jmp 0x07C0:start     ; Goto segment 07C0

    start:
            ; Update the segment registers
            mov ax, cs
            mov ds, ax
            mov es, ax

            mov ah, 1
            mov cx, 0x2607
            int 0x10

    mov ax, 0xb800              ; Console memory
            mov gs, ax          ; [b800:0000]
            mov di, 0           ;   0xb8000

clear:      mov word [gs:di], 0x6f20
                                ; white on orange space

            inc di              ; next character
            inc di              ;
            ;inc ah

            cmp di, 2*80*25     ; 2*cols*rows -10
            jnz clear

            jmp started

callstr:    mov si, 0
   .str:    mov byte al, [bx+si]
            mov byte [gs:di], al
            inc di
            inc di
            inc si

            cmp si, dx
            jne .str
            ret

    bootstr db "LOADING PROGRAM FROM DEVICE"    ;27
    psstr   db "PRESS ANY KEY OR QUIT"          ;21

    started:

            mov di, (2*80*12) + (2*30)
            mov bx, psstr
            mov dx, 21
            call callstr

            xor ah, ah
            int 0x16;

            mov di, (2*80*12) + (2*27)
            mov dx, 27
            mov bx, bootstr
            call callstr

;    reset:                      ; Reset the floppy drive
;            xor ax, ax          ;
;            mov dl, 0           ; Drive=0 (=A)
;            int 0x13            ;
;            jc reset            ; ERROR => reset again

    read:
            mov ax, 0x07e0      ; ES:BX = 07e0:0000
            mov es, ax          ;
            mov bx, 0           ;

            mov ah, 2           ; Load disk data to ES:BX
            mov al, 64          ; Load 64 sectors [32kB]
            mov ch, 0           ; Cylinder=0
            mov cl, 2           ; Sector=2
            mov dh, 0           ; Head=0

            mov dl, 0x00        ; FLOPPY A
            ;mov dl, 0x80        ; BOOTING DRIVE
            int 0x13             ; Read

            jc read             ; ERROR => Try again

            jmp 0x07e0:0000     ; Jump to the program


    times 510-($-$$) db 0
    dw 0AA55h

;-------------------------------------------------------------------------------------------------------------------------------
;                                                                OS
    os_start:
        mov ax, cs
        mov ds, ax
        mov es, ax

    mov ax, 0xb800              ; Console memory
            mov gs, ax          ; [b800:0000]
            mov di, 0           ;   0xb8000

    mov ax, trans_stack         ; Transition stack (not really a stack)
            mov fs, ax          ; [5500:0000] - data_stack current index
                                ; [    :0002] - call_stack current index
            mov di, 0
            mov word [fs:di], 0
            mov di, 2
            mov word [fs:di], 0

    mov ax, data_stack          ; Data stack (because program calls interfere with program_id passing)
            mov ss, ax          ; [0050:0000] - [02cf:000f]
            mov sp, 0           ; 10 kB

        jmp os_snake

dispclear:  mov di, 0
   .clear:  mov word [gs:di], 0x0f20
                                ; white on black space

            inc di              ; next character
            inc di              ;
            ;inc ah

            cmp di, 2*80*25     ; 2*cols*rows
            jne .clear
            ret

dispwall:   mov di, 0
            push ax
;   .wall:   mov word [gs:di], 0x0f20
                                ; white on orange space

;            inc di              ; next character
;            inc di              ;
            ;inc ah

;            cmp di, 2*80*21     ; 2*cols*rows -10
;            jne .wall

            mov di, (2*80*22)
            mov al, 220         ; polovicny blok
            mov dx, 80
            call callchar

            mov di, (2*80*22) +1
            mov al, 0x06
            mov dx, 80
            call callchar

            mov di, (2*80*24)
            mov al, 223         ; polovicny blok
            mov dx, 80
            call callchar

            mov di, (2*80*24) +1
            mov al, 0x06
            mov dx, 80
            call callchar

            pop ax

            ret

callstring: mov si, 0
            push ax
   .string: mov byte al, [bx+si]
            mov byte [gs:di], al
            inc di
            inc di
            inc si

            cmp si, dx
            jne .string

            pop ax
            ret

callchar:   mov si, 0
   .char:  ;mov byte al, [bx+si]
            mov byte [gs:di], al
            inc di
            inc di
            inc si

            cmp si, dx
            jne .char
            ret

callstack:  mov si, 0
   .stack:  ;mov byte al, [bx+si]
            pop ax
            mov byte [gs:di], al
            inc di
            inc di
            inc si

            cmp si, dx
            jne .stack
            ret

updatetime: push cx
            mov ah, 0x02
            int 0x1a    ; GET RTC TIME
            jnc .ok
            pop cx
            ret
       .ok:

            xor ax, ax  ; reset ax
            mov al, ch  ; hrs -> al
            mov bl, 0x10
            div bl
            add al, '0'

            mov di, (2*80*23) +(2*8*8) +2
            mov dx, 1
            call callchar

            xor ax, ax  ; reset ax
            mov al, ch  ; hrs -> al
            mov bl, 0x10
            mul bl

            xor ah, ah
            mov bl, 0x10
            div bl
            add al, '0'

            mov di, (2*80*23) +(2*8*8) +4
            mov dx, 1
            call callchar

            xor ax, ax  ; reset ax
            mov al, ch  ; hrs -> al
            mov bl, 0x10
            div bl
            add al, '0'

            ;-----------------------------
            ;           minutes

            xor ax, ax  ; reset ax
            mov al, cl  ; hrs -> al
            mov bl, 0x10
            div bl
            add al, '0'

            mov di, (2*80*23) +(2*8*8) +8
            mov dx, 1
            call callchar

            xor ax, ax  ; reset ax
            mov al, cl  ; mins -> al
            mov bl, 0x10
            mul bl

            xor ah, ah
            mov bl, 0x10
            div bl
            add al, '0'

            mov di, (2*80*23) +(2*8*8) +10
            mov dx, 1
            call callchar

            pop cx
            ret

    os_snake:
        call dispclear

        mov di, (2*80*12) + (160 -88)
        mov bx, loadstr - os_start
        mov dx, 9
        call callstring

        mov di, (2*80*13) + (160 - 88)
        mov bx, loadst2 - os_start
        mov dx, 9
        call callstring

        mov ax, 0x000a
        push ax     ; last program = 000a (non-existant)
        xor cx, cx  ; choice = 0

    os_while:       ; DO WHILE 1

        mov ax, ss  ; IF(stack = call_stack) stack = data_stack
        cmp ax, call_stack
        jne os_notcall
        mov di, 2
        mov word [fs:di], sp
        mov ax, data_stack
        mov ss, ax
        mov di, 0
        mov sp, word [fs:di]
    os_notcall:

        call dispwall

        mov di, (2*80*23) +1
        mov al, 0x60
        mov dx, 80
        call callchar

        mov ax, cx
        mov bx, 2*8
        mul bx
        add ax, (2*80*23) +1
        mov di, ax
        mov al, 0x06
        mov dx, 8
        call callchar

        mov di, (2*80*23)
        mov bx, progone - os_start
        mov dx, 80
        call callstring

        call updatetime
                    ; TIME UPDATE
                    ; Problem: time update only on keypress

    os_ugh:
        mov ah, 0
        int 0x16    ; READ KEYBOARD, SWITCH

        cmp al, '4' ; CASE 4: IF(cx != 0) cx--
        jne os_x4
        cmp cx, 0
        je  os_x4
        cmp cx, 8
        jne os_4
        dec cx
    os_4:
        dec cx
    os_x4:

        cmp al, '5' ; CASE 5: IF(cx == 9) reboot() ELSE program(cx)
        jne os_x5
        cmp cx, 9
        jne  os_5
        int 0x19
    os_5_t:
        jmp os_x5
    os_5:           ; program(cx)
        cmp cx, 8
        je os_5_t
        pop ax      ; running program
        cmp cx, ax
        jne os_5_r  ; IF(cx != ax) kill(ax), run(cx) ELSE resume(cx)

;-------------------------------------------------------------------------------------------------------------------------------
;                                                              RESUME
        mov ax, cx
        mov bx, 2*8
        mul bx
        add ax, (2*80*23) +1
        mov di, ax
        mov al, 0x0f
        mov dx, 8
        call callchar

        mov di, 0
        mov word [fs:di], sp
        mov ax, call_stack
        mov ss, ax
        mov di, 2
        mov sp, word [fs:di]
        ret         ; ax popped, push current choice, change to call_stack and return to caller - resume(cx)
                    ; ! program must pop registers after return !
        jmp os_x5   ; END OF resume(cx)
    os_5_r:
        cmp ax, 0x000a
                    ; no caller, just run cx
        je os_5_run ; IF(ax == 000a) run(cx) ELSE kill(ax), run(cx)
                    ; ax is popped, must pop call_stack so it doesn't overflow from killing a lot

;-------------------------------------------------------------------------------------------------------------------------------
;                                                               KILL
        pop ax
        pop ax
        pop ax
        pop ax

        mov di, 0   ;back to call_stack
        mov word [fs:di], sp
        mov ax, call_stack
        mov ss, ax
        mov di, 2
        mov sp, word [fs:di]

        pop ax
        jmp os_5_kill
    os_5_run:

;-------------------------------------------------------------------------------------------------------------------------------
;                                                                RUN
        mov di, 0
        mov word [fs:di], sp
        mov ax, call_stack
        mov ss, ax
        mov di, 2
        mov sp, word [fs:di]
                    ; change to call stack
    os_5_kill:
        mov ax, cx
        mov bx, 2*8
        mul bx
        add ax, (2*80*23) +1
        mov di, ax
        mov al, 0x0f
        mov dx, 8
        call callchar

        mov ax, cx
        mov bx, 0x1000
        mul bx      ; cx * 1024 [program size] = skipped programs size
        add ax, 0x300
;        mov word [jmptgt - os_start], ax  ; program offset
        ; xor bx, bx  ; segment is start

        call ax       ; call [cs:ax] <- 0x200 + ax <- addr FOR p0 = 0x500 <- ax for p0 = 0x300

        mov di, 2
        mov word [fs:di], sp
        mov ax, data_stack
        mov ss, ax
        mov di, 0
        mov sp, word [fs:di]

        push 0x000a

        call dispclear

;        call dispclear

        jmp os_while

;-------------------------------------------------------------------------------------------------------------------------------
;                                                                END
    os_x5:

        cmp al, '6' ; CASE 6: IF(cx != 9) cx++
        jne os_x6
        cmp cx, 9
        je  os_x6
        cmp cx, 6
        jne os_6
        inc cx
    os_6:
        inc cx
    os_x6:

        jmp os_while

    loadstr db 27 , "  > <  ", 26   ;9
    loadst2 db 52 , "   5   ", 54   ;9
    jmptgt  dw 0
    progone db " XXXXXX "       ;8
    progtwo db " XXXXXX "       ;8
    progthr db " XXXXXX "       ;8
    progfou db " XXXXXX "       ;8
    progfiv db " XXXXXX "       ;8
    progsix db " XXXXXX "       ;8
    progsev db " XXXXXX "       ;8
    progeig db "         "      ;9
    progtim db "00:00  "        ;7
    progten db " REBOOT "       ;8

    times 0x0500-($-$$) db 0

;-------------------------------------------------------------------------------------------------------------------------------
;                                                                OS END
    p1_start:                   ; in call_stack, just got here
        push cx

        mov di, 2
        mov word [fs:di], sp
        mov ax, data_stack
        mov ss, ax
        mov di, 0
        mov sp, word [fs:di]

;        mov di, (2*80*12) + (160 -88)
;        mov bx, loadstr - os_start
;        mov dx, 9
;        call callstring

;        mov di, (2*80*13) + (160 - 88)
;        mov bx, loadst2 - os_start
;        mov dx, 9
;        call callstring

        mov di, (2*80*5)
        mov bx, progone - os_start
        mov dx, 80
        call callstring

        mov ah, 0
        int 0x16

        mov cx, 5

        push ax
        push bx
        push cx
        push dx

        push 0      ; program pausing

        mov di, 0   ;back to call_stack
        mov word [fs:di], sp
        mov ax, call_stack
        mov ss, ax
        mov di, 2
        mov sp, word [fs:di]

        pop cx

        call os_while

        push cx

        mov di, 2
        mov word [fs:di], sp
        mov ax, data_stack
        mov ss, ax
        mov di, 0
        mov sp, word [fs:di]

        pop dx  ; get data back from data_stack after resume
        pop cx
        pop bx
        pop ax

        mov di, (2*80*15)
        mov al, 0x60
        mov dx, 80
        call callchar

        mov di, 0   ;back to call_stack
        mov word [fs:di], sp
        mov ax, call_stack
        mov ss, ax
        mov di, 2
        mov sp, word [fs:di]

        mov ah, 0
        int 0x16

        pop cx

        ret

    times 0x1500-($-$$) db 0

    p2_start:
        mov di, (2*80*3)
        mov bx, progone - os_start
        mov dx, 80
        call callstring

        ret

    times 0x2500-($-$$) db 0
        ret

    times 0x3500-($-$$) db 0
        ret

    times 0x4500-($-$$) db 0
        ret

    times 0x5500-($-$$) db 0
        ret

    times 0x6500-($-$$) db 0

    p7_start:
        mov di, (2*80*3)
        mov bx, progone - os_start
        mov dx, 80
        call callstring

        xor ax, ax
        int 0x16

        ret
