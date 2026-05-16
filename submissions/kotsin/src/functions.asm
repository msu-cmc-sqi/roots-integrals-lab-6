; functions.asm - f1, f2, f3 и их производные df1, df2, df3
; Вариант 3: f1=e^(-x)+3, f2=2x-2, f3=1/x
; cdecl: long double лежит на стеке по [ebp+8]
; возвращаемое значение - в ST(0)

section .note.GNU-stack noalloc noexec nowrite progbits

section .data
    const_2:    dq  2.0
    const_3:    dq  3.0

section .text
global f1, f2, f3, df1, df2, df3

; Вычисляет e^x, где x = ST0, результат в ST0
; e^x = 2^(x*log2e) = 2^(N+f), N - целая часть, f - дробная
; f2xm1 работает только при |f| <= 1, благодаря frndint
%macro EXP_ST0 0
    fldl2e
    fmulp st1, st0          ; ST0 = x*log2e
    fld st0
    frndint                 ; ST0=N, ST1=y
    fxch
    fsub st0, st1           ; ST0=f=y-N, ST1=N
    f2xm1                   ; ST0=2^f-1
    fld1
    faddp st1, st0          ; ST0=2^f
    fscale                  ; ST0=2^y=e^x, ST1=N
    fxch st1
    fstp st0                ; убираем N, ST0=e^x
%endmacro

; f1(x) = e^(-x) + 3
f1:
    push ebp
    mov  ebp, esp
    fld tword [ebp+8]
    fchs
    EXP_ST0
    fld qword [const_3]
    faddp st1, st0
    pop  ebp
    ret

; f2(x) = 2x - 2
f2:
    push ebp
    mov  ebp, esp
    fld tword [ebp+8]
    fadd st0, st0           ; 2x
    fld qword [const_2]
    fsubp st1, st0          ; 2x - 2
    pop  ebp
    ret

; f3(x) = 1/x
f3:
    push ebp
    mov  ebp, esp
    fld1
    fld tword [ebp+8]
    fdivp st1, st0
    pop  ebp
    ret

; df1(x) = -e^(-x)
df1:
    push ebp
    mov  ebp, esp
    fld tword [ebp+8]
    fchs
    EXP_ST0
    fchs
    pop  ebp
    ret

; df2(x) = 2
df2:
    push ebp
    mov  ebp, esp
    fld qword [const_2]
    pop  ebp
    ret

; df3(x) = -1/x^2
df3:
    push ebp
    mov  ebp, esp
    fld tword [ebp+8]
    fmul st0, st0           ; x^2
    fld1
    fxch
    fdivp st1, st0          ; 1/x^2
    fchs
    pop  ebp
    ret
