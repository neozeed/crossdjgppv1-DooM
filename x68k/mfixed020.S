* NO_APP
	.cpu 68030
	.include doscall.equ
* X68 GCC Develop
	.even
	.data
_rcsid:
	.dc.b '$Id: m_bbox.c,v 1.1 1997/02/03 22:45:10 b1 Exp $',$00
	.even
	.text
	.even
	.globl _FixedMul
_FixedMul:
	link a6,#0
	movem.l d3/d4,-(sp)
	move.l (8,a6),d0
	move.l (12,a6),d4
	clr.l d3
	move.l d0,d2
	jbge ?2
	not.l d3
?2:
	clr.l d1
	move.l d4,d0
	jbge ?3
	not.l d1
?3:
	move.l d1,-(sp)
	move.l d0,-(sp)
	move.l d3,-(sp)
	move.l d2,-(sp)
	jbsr ___muldi3
	lea (16,sp),sp
	clr.l -(sp)
	move.l #16,-(sp)
	move.l d1,-(sp)
	move.l d0,-(sp)
	jbsr ___ashrdi3
	movem.l -8(a6),d3/d4
	unlk a6
	rts
	.even
	.globl _FixedDiv
_FixedDiv:
	link a6,#0
	movem.l d3/d4,-(sp)
	move.l (8,a6),d2
	move.l (12,a6),d1
	tst.l d2
	jblt ?6
	move.l d2,d0
	jbra ?7
?6:
	move.l d2,d0
	neg.l d0
?7:
	move.l d0,d3
	moveq.l #14,d4
	asr.l d4,d3
	tst.l d1
	jblt ?8
	move.l d1,d0
	jbra ?9
?8:
	move.l d1,d0
	neg.l d0
?9:
	cmp.l d3,d0
	jbgt ?5
	move.l d2,d0
	eor.l d1,d0
	jbge ?10
	move.l #-2147483648,d0
	jbra ?4
?10:
	move.l #2147483647,d0
	jbra ?4
?5:
	move.l d1,-(sp)
	move.l d2,-(sp)
	jbsr _FixedDiv2
?4:
	movem.l -8(a6),d3/d4
	unlk a6
	rts
	.even
	.globl _FixedDiv2
_FixedDiv2:
	link a6,#0
	movem.l d3/d4/a3,-(sp)
	move.l (8,a6),-(sp)
	lea ___floatsidf,a3
	jbsr (a3)
	addq.w #4,sp
	move.l d0,d3
	move.l d1,d4
	move.l (12,a6),-(sp)
	jbsr (a3)
	addq.w #4,sp
	move.l d1,-(sp)
	move.l d0,-(sp)
	move.l d4,-(sp)
	move.l d3,-(sp)
	jbsr ___divdf3
	lea (16,sp),sp
	clr.l -(sp)
	move.l #1089470464,-(sp)
	move.l d1,-(sp)
	move.l d0,-(sp)
	jbsr ___muldf3
	lea (16,sp),sp
	move.l d0,d3
	move.l d1,d4
	clr.l -(sp)
	move.l #1105199104,-(sp)
	move.l d4,-(sp)
	move.l d3,-(sp)
	lea ___cmpdf2,a3
	jbsr (a3)
	lea (16,sp),sp
	tst.l d0
	jbge ?14
	clr.l -(sp)
	move.l #-1042284544,-(sp)
	move.l d4,-(sp)
	move.l d3,-(sp)
	jbsr (a3)
	lea (16,sp),sp
	tst.l d0
	jbge ?13
?14:
	moveq.l #1,d0
	jbra ?12
?13:
	move.l d4,-(sp)
	move.l d3,-(sp)
	jbsr ___fixdfsi
?12:
	movem.l -12(a6),d3/d4/a3
	unlk a6
	rts
	.even
	.end
