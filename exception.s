//================
//
// I_DivException
//
//================
//
// sys_dosa.s
// x86 assembly-language DOS-dependent routines.
	.data

	.align	4
fpenv:
	.long	0, 0, 0, 0, 0, 0, 0, 0

	.text

.globl  _MaskExceptions
_MaskExceptions:
	fnstenv	fpenv
	orl		$0x3F,fpenv
	fldenv	fpenv

	ret

.globl _unmaskexceptions
_unmaskexceptions:
	fnstenv	fpenv
	andl		$0xFFFFFFE0,fpenv
	fldenv	fpenv

	ret

#if 0
.text
        .align 4
.globl _I_DivException
_I_DivException:
	mov	%edx,0x03c9
	mov	%al,0x63
	outw	%al,%dx

	mov	%ebx,0xffffff
	mov	%eax,%ebx
	ret

        .align 4
.globl _I_SetDivException
_I_SetDivException:

	ret
	pusha

	mov	%eax,0x0212
	mov	%ebx,0x0
//	mov	%ecx,cs
	movw    %ecx,%cs:_I_DivException
	movl	_I_DivException, %edx
//	int	0x31
	jnc	good

	popa
	mov	%eax,0x0
	ret

good:
	popa
	mov	%eax,0x1
	ret
#endif
