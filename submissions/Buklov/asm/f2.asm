; f2(x) = sqrt(x + 0.5)
; вариант 9, NASM32, cdecl

global f2

section .data
    half: dq 0.5

section .text
f2:
    push ebp
    mov  ebp, esp

    fld  qword [ebp+8]      ; x
    fadd qword [half]       ; x + 0.5
    fsqrt                   ; sqrt(x + 0.5)

    pop  ebp
    ret
