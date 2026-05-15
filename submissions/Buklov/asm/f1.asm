; f1(x) = 3 / ((x - 1)^2 + 1)
; вариант 9, NASM32, cdecl: x на [ebp+8], результат в st0

global f1

section .data
    three: dq 3.0

section .text
f1:
    push ebp
    mov  ebp, esp

    fld  qword [ebp+8]      ; x
    fld1
    fsubp st1, st0          ; x - 1
    fmul st0, st0           ; (x-1)^2
    fld1
    faddp st1, st0          ; (x-1)^2 + 1
    fld  qword [three]      ; 3
    fdivrp st1, st0         ; 3 / ((x-1)^2 + 1)

    pop  ebp
    ret
