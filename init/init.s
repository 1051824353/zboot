	.code16

stack		= 0x7c00
	.text
	.globl	_start
_start:
	cli
	xorw	%ax, %ax
	xorw	%bx, %bx
	xorw	%cx, %cx
	xorl	%ebp,%ebp
	xorl	%edi,%edi
	movw	%ax, %ds
	movw	%ax, %ss
	movw	$stack, %sp
	movw	%sp, %si
	pushw	%es		/* es:di -> $PnP header */
	pushw	%di
	movw	%ax, %es
	sti
	cld
	
	call	cstart	
