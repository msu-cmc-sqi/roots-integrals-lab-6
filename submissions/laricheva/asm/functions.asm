; функции 9 варианта
; f1 = 3 / ((x - 1)^2 + 1)
; f2 = sqrt(x + 0.5)
; f3 = exp(-x)

section .text
GLOBAL f1
GLOBAL f2
GLOBAL f3
GLOBAL df1
GLOBAL df2
GLOBAL df3

; float f1(float x) = 3 / ((x - 1)^2 + 1)
f1:
    finit
    push ebp
    mov ebp, esp
    fld dword [ebp + 8]     ; x
    fld1                    ; 1, x
    fsubp st1, st0          ; x - 1
    fld st0                 ; x - 1, x - 1
    fmulp st1, st0          ; (x - 1)^2
    fld1                    ; 1, (x - 1)^2
    faddp st1, st0          ; (x - 1)^2 + 1
    fld1
    fld1
    fld1
    faddp st1, st0
    faddp st1, st0          ; 3, denominator
    fdiv st0, st1           ; 3 / denominator
    fstp st1
    pop ebp
    ret

; float f2(float x) = sqrt(x + 0.5)
f2:
    finit
    push ebp
    mov ebp, esp
    fld dword [ebp + 8]     ; x
    fld1
    fld1
    faddp st1, st0          ; 2, x
    fld1                    ; 1, 2, x
    fdiv st0, st1           ; 0.5, 2, x
    fstp st1                ; 0.5, x
    faddp st1, st0          ; x + 0.5
    fsqrt
    pop ebp
    ret

; float f3(float x) = exp(-x)
f3:
    finit
    push ebp
    mov ebp, esp
    fld dword [ebp + 8]
    fchs
    call exp_st0
    pop ebp
    ret

; float df1(float x) = -6*(x - 1) / (((x - 1)^2 + 1)^2)
df1:
    finit
    push ebp
    mov ebp, esp

    fld dword [ebp + 8]     ; x
    fld1
    fsubp st1, st0          ; t = x - 1
    fld st0                 ; t, t
    fld st0                 ; t, t, t
    fmulp st1, st0          ; t^2, t
    fld1
    faddp st1, st0          ; den = t^2 + 1, t
    fld st0                 ; den, den, t
    fmulp st1, st0          ; den^2, t
    fxch st1                ; t, den^2

    fld1
    fld1
    faddp st1, st0          ; 2
    fld1
    faddp st1, st0          ; 3
    fld1
    fld1
    fld1
    faddp st1, st0
    faddp st1, st0          ; 3, 3, t, den^2
    faddp st1, st0          ; 6, t, den^2
    fchs                    ; -6, t, den^2
    fmulp st1, st0          ; -6*t, den^2
    fdiv st0, st1           ; -6*t / den^2
    fstp st1

    pop ebp
    ret

; float df2(float x) = 1 / (2 * sqrt(x + 0.5))
df2:
    finit
    push ebp
    mov ebp, esp
    fld dword [ebp + 8]
    fld1
    fld1
    faddp st1, st0          ; 2
    fld1
    fdiv st0, st1           ; 0.5, 2, x
    fstp st1                ; 0.5, x
    faddp st1, st0          ; x + 0.5
    fsqrt                   ; sqrt(x + 0.5)
    fadd st0, st0           ; 2 * sqrt(x + 0.5)
    fld1
    fdiv st0, st1           ; 1 / (2 * sqrt(...))
    fstp st1
    pop ebp
    ret

; float df3(float x) = -exp(-x)
df3:
    finit
    push ebp
    mov ebp, esp
    fld dword [ebp + 8]
    fchs
    call exp_st0
    fchs
    pop ebp
    ret

; на входе st0 = x, на выходе st0 = exp(x)
exp_st0:
    fldl2e
    fmulp st1, st0
    fld st0
    frndint
    fsub st1, st0
    fxch st1
    f2xm1
    fld1
    faddp st1, st0
    fscale
    fstp st1
    ret