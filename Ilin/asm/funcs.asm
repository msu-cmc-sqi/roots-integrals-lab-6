section .data
const_0_35: dt 0.35
const_0_95: dt 0.95
const_2_7: dt 2.7
const_3: dt 3.0
const_1: dt 1.0
const_2: dt 2.0
const_0_7: dt 0.7

section .text
global f1, f2, f3, df1, df2, df3

f1:
	push ebp
	mov ebp, esp
	finit
	fld tword [ebp+8]
	fld st0
	fmul st0, st0
	fld tword [const_0_35]
	fmulp
	fld tword [ebp+8]
	fld tword [const_0_95]
	fmulp
	fsubp
	fld tword [const_2_7]
	faddp
	pop ebp
	ret

f2:
	push ebp
	mov ebp, esp
	finit
	fld tword [ebp+8]
	fld tword [const_3]
	fmulp
	fld tword [const_1]
	faddp
	pop ebp
	ret

f3:
	push ebp
	mov ebp, esp
	finit
	fld tword [ebp+8]
	fld tword [const_2]
	faddp
	fld1
	fdivp
	pop ebp
	ret

df1:
	push ebp
	mov ebp, esp
	finit
	fld tword [ebp+8]
	fld tword [const_0_7]
	fmulp
	fld tword [const_0_95]
	fsubp
	pop ebp
	ret

df2:
	push ebp
	mov ebp, esp
	finit
	fld tword [const_3]
	pop ebp
	ret

df3:
	push ebp
	mov ebp,esp
	finit
	fld tword [ebp+8]
	fld tword [const_2]
	faddp
	fld st0
	fmulp
	fld1
	fdivp
	fchs
	pop ebp
	ret
