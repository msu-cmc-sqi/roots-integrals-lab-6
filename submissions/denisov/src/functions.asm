section .data
    two     dt 2.0
    eight   dt 8.0
    minus2  dt -2.0
    minus5  dt -5.0
    five    dt 5.0

section .text
global f1
global f2
global f3
global df1
global df2
global df3

; long double exp_ld(long double x), result is returned in st0.
; e^x = 2^(x * log2(e)); f2xm1 accepts only fractional arguments,
; so the integer and fractional parts are handled separately.
exp_ld:
    fld     tword [ebp + 8]
    fldl2e
    fmulp   st1, st0
    fld     st0
    frndint
    fsub    st1, st0
    fxch    st1
    f2xm1
    fld1
    faddp   st1, st0
    fscale
    fstp    st1
    ret

; f1(x) = exp(x) + 2
f1:
    push    ebp
    mov     ebp, esp
    call    exp_ld
    fld     tword [two]
    faddp   st1, st0
    pop     ebp
    ret

; f2(x) = -2x + 8
f2:
    push    ebp
    mov     ebp, esp
    fld     tword [ebp + 8]
    fld     tword [minus2]
    fmulp   st1, st0
    fld     tword [eight]
    faddp   st1, st0
    pop     ebp
    ret

; f3(x) = -5 / x
f3:
    push    ebp
    mov     ebp, esp
    fld     tword [minus5]
    fld     tword [ebp + 8]
    fdivp   st1, st0
    pop     ebp
    ret

; df1(x) = exp(x)
df1:
    push    ebp
    mov     ebp, esp
    call    exp_ld
    pop     ebp
    ret

; df2(x) = -2
df2:
    push    ebp
    mov     ebp, esp
    fld     tword [minus2]
    pop     ebp
    ret

; df3(x) = 5 / x^2
df3:
    push    ebp
    mov     ebp, esp
    fld     tword [five]
    fld     tword [ebp + 8]
    fld     st0
    fmulp   st1, st0
    fdivp   st1, st0
    pop     ebp
    ret
