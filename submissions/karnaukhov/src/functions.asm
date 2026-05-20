global f1
global f2
global f3

section .data
    one     dd 1.0
    three   dd 3.0

section .text

; float f1(float x)
; f1 = 2^x + 1

f1:
    push ebp
    mov ebp, esp

    fld dword [ebp + 8]     ; st0 = x

    fld st0
    frndint
    fsub st1, st0

    fxch st1
    f2xm1

    fld1
    faddp

    fscale
    fstp st1

    fld dword [one]
    faddp

    mov esp, ebp
    pop ebp
    ret


; float f2(float x)
; f2 = x^5

f2:
    push ebp
    mov ebp, esp

    fld dword [ebp + 8] ; x
    fld st0             ; x x
    fmul st0, st1       ; x^2

    fld st1             ; x x^2 x
    fmul st0, st1       ; x^3

    fmul st0, st1       ; x^5

    fstp st1
    fstp st1

    mov esp, ebp
    pop ebp
    ret


; float f3(float x)
; f3 = (1 - x)/3

f3:
    push ebp
    mov ebp, esp

    fld dword [one]
    fld dword [ebp + 8]

    fsubp               ; 1 - x

    fld dword [three]
    fdivp               ; (1 - x)/3

    mov esp, ebp
    pop ebp
    ret