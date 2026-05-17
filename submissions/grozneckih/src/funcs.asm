section .data
    c1 dt 1.0
    c15 dt 1.5
    cs15 dt -1.5
    c25 dt 2.5
    c95 dt 9.5
    c3 dt 3.0
    c5 dt 5.0
    cs5 dt -5.0
    
section .text
global f1
global f2
global f3
global df1
global df2
global df3

f1:
    push ebp
    mov ebp, esp

    fld tword[c15]
    
    fld tword[ebp + 8] ;x
    fld tword[c1]
    faddp ;t0=x+1
    
    fdivp ;t0=1.5/(x+1)
    
    fld tword[c3]
    faddp ;t0=res

    mov esp, ebp
    pop ebp
    ret
    
    
f2:
    push ebp
    mov ebp, esp
    
    fld tword[ebp + 8]
    
    fld tword[c25]
    fmulp ;t0=x*2.5
    
    fld tword[c95]
    fsubp ;t0=2.5*x-9.5=res

    mov esp, ebp
    pop ebp
    ret
    
    
f3:
    push ebp
    mov ebp, esp
    
    fld tword[c5]
    fld tword[ebp + 8]
    
    fdivp ;t0=res

    mov esp, ebp
    pop ebp
    ret
    
    
    
df1:
    push ebp
    mov ebp, esp

    fld tword[cs15]
    
    fld tword[ebp + 8] ;x
    fld tword[c1]
    faddp ;t0=x+1
    fmul st0, st0 ;t0=(x+1)^2
    
    fdivp ;t0=-1.5/(x+1)^2=res
    
    mov esp, ebp
    pop ebp
    ret
    
    
df2:
    push ebp
    mov ebp, esp
    
    fld tword[c25]

    mov esp, ebp
    pop ebp
    ret
    
    
df3:
    push ebp
    mov ebp, esp
    
    fld tword[cs5]
    fld tword[ebp + 8]
    
    fmul st0, st0 ;st0=x^2
    
    fdivp ;t0=res

    mov esp, ebp
    pop ebp
    ret