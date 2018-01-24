//================
//
// R_DrawColumn
//
//================
//
// 2/15/98 Lee Killough
//
// Converted C code with TFE fix to assembly and tuned
//
// 2/21/98 killough: added translucency support
//
// 11/98 killough: added hires support
//
//================

 .text
 .align 8
 .globl _R_DrawColumn
_R_DrawColumn:

 pushl %ebp
 pushl %esi
 pushl %edi
 pushl %ebx
 movl _dc_yh,%esi
 movl _dc_yl,%edx
 incl %esi
 movl _ylookup(,%edx,4),%ebx
 subl %edx,%esi
 jle 9f
 addl _dc_x,%ebx
 movl _dc_texheight,%eax
 subl _centery,%edx
 movl _dc_source,%ebp
 imull _dc_iscale,%edx
 leal -1(%eax),%ecx
 movl _dc_colormap,%edi
 addl _dc_texturemid,%edx
 cmpl $128,%eax
 je 7f
 testl %eax,%ecx
 je 5f
 sall $16,%eax

0:
 subl %eax,%edx
 jge 0b

1:
 addl %eax,%edx
 jl 1b

 .align 8,0x90
2:
 movl %edx,%ecx
 sarl $16,%ecx
 addl _dc_iscale,%edx
 movzbl (%ecx,%ebp),%ecx
 movb (%edi,%ecx),%cl
 movb %cl,(%ebx)
 addl $320,%ebx
 cmpl %eax,%edx
 jge 3f
 decl %esi
 jg 2b
 popl %ebx
 popl %edi
 popl %esi
 popl %ebp
 ret

 .align 8
3:
 subl %eax,%edx
 decl %esi
 jg 2b
 popl %ebx
 popl %edi
 popl %esi
 popl %ebp
 ret

 .align 8
9:
 popl %ebx
 popl %edi
 popl %esi
 popl %ebp
 ret

 .align 8
4:
 movl %edx,%eax
 addl _dc_iscale,%edx
 sarl $16,%eax
 andl %ecx,%eax
 movzbl (%eax,%ebp),%eax
 movb (%eax,%edi),%al
 movb %al,(%ebx)
 movl %edx,%eax
 addl _dc_iscale,%edx
 sarl $16,%eax
 andl %ecx,%eax
 movzbl (%eax,%ebp),%eax
 movb (%eax,%edi),%al
 movb %al,320(%ebx)
 addl $640,%ebx

5:
 addl $-2,%esi
 jge 4b
 jnp 9b
 sarl $16,%edx
 andl %ecx,%edx
 xorl %eax,%eax
 movb (%edx,%ebp),%al
 movb (%eax,%edi),%al
 movb %al,(%ebx)
 popl %ebx
 popl %edi
 popl %esi
 popl %ebp
 ret

7:				
; movl _dc_iscale, %ecx
 movl %edi,%ecx		
 andl $0x007fffff,%edx
 movl _dc_iscale, %edi
 addl $-2, %esi
 jl 8f 
 .align 8
6:
 movl %edx,%eax
 sarl $16,%edx
 addl %edi,%eax
 andl $0x007fffff,%eax
 movb (%edx,%ebp),%cl
 movl %eax,%edx
 sarl $16,%eax
 movb (%ecx),%cl
 addl %edi,%edx
 movb %cl,(%ebx)
 movb (%eax,%ebp),%cl
 addl $640,%ebx
 andl $0x007fffff,%edx
 movb (%ecx),%cl
 addl $-2,%esi
 movb %cl,-320(%ebx)
 jge 6b
8:
 jnp 9b
 sarl $16,%edx
 movl %ebx,%esi
 movb (%edx,%ebp),%cl
 popl %ebx
 popl %edi
 movb (%ecx),%cl
 movb %cl,(%esi)
 popl %esi
 popl %ebp
 ret
//----------------------------------------------------------------------------
// $Id$
//----------------------------------------------------------------------------
//
// $Log$
// Revision 1.1  2000-07-29 13:20:39  fraggle
// Initial revision
//
// Revision 1.3  1998/03/04  12:33:29  killough
// Fix problem with last translucent pixel being drawn
//
// Revision 1.2  1998/02/23  04:18:24  killough
// Add translucency support, more tuning
//
// Revision 1.1  1998/02/17  06:37:37  killough
// Initial version
//
//
//----------------------------------------------------------------------------