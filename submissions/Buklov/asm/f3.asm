; f3(x) = exp(-x)
; считаем через тождество e^y = 2^(y * log2 e):
;   z = -x * log2(e), z = n + f где n целое, |f| <= 0.5
;   2^z = 2^f * 2^n  -- f2xm1 даёт 2^f - 1, fscale умножает на 2^n

global f3

section .text
f3:
    push ebp
    mov  ebp, esp

    fld   qword [ebp+8]     ; x
    fchs                    ; -x
    fldl2e                  ; log2(e), st1 = -x
    fmulp st1, st0          ; z = -x * log2(e)

    fld   st0               ; z, z
    frndint                 ; n, z
    fxch                    ; z, n
    fsub  st0, st1          ; f = z - n, n
    f2xm1                   ; 2^f - 1, n
    fld1
    faddp st1, st0          ; 2^f, n
    fscale                  ; 2^f * 2^n = e^(-x), n
    fstp  st1               ; убрать n, оставить результат на вершине

    pop  ebp
    ret
