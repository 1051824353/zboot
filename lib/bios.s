	.code16
.globl inittty

inittty:
	pushl	%eax
	movb    $0x00, %ah
	movb    $0x13, %al

	int $0x10       /* May destroy %bp */
	popl	%eax
	retl
	

.globl  putc
putc:
	pushl	%ebp
	movl	%esp,%ebp
	pushl	%eax
	pushl	%ebx

	movl    8(%ebp), %eax

	cmp		$0x0a,	%ax
	je		newline
	cmp     $0x0d,  %ax
	je      newline
	jne		print
putc1:
	popl	%ebx
	popl	%eax
	popl	%ebp	
	retl

print:
	movb    $0x0e, %ah 
	movb    (0x462), %bh 
	movb    $0x07, %bl 
	int $0x10       /* May destroy %bp */
	jmp putc1

newline:
	movb    $0x03, %ah 
	movb    (0x462), %bh 
	int $0x10  
	
	movb    $0x02, %ah 
	movb    (0x462), %bh 
	add		$0x1,	%dh
	movb	$0x0,	%dl
	int $0x10  
	
	jmp putc1


.globl	getc

getc:
	pushl 	%ebp
	movl	%esp,%ebp

	movb	$0x10,%ah
	int	$0x16

	popl	%ebp
	retl


.globl checkkb

checkkb:
	pushl 	%ebp
	movl	%esp,%ebp

	movb	$0x12,%ah
	int	$0x16	

	popl	%ebp
	retl

.globl pread
pread:
	pushl   %ebp
	movl    %esp,%ebp
	pushl	%eax
	pushl	%edx
	pushl	%ecx
	push    %si

	movl    12(%ebp), %ecx

	pushl	$0x0000
	pushl	16(%ebp)
	pushl	8(%ebp)
	push	%cx
	push	$0x0010

	mov		$0x80,%dl
	mov		%sp,%si
	movb 	$0x42,%ah
	movb	$0x00,%al
	int	$0x13
	
	addl    $16, %esp	
	pop	%si
	popl	%ecx
	popl	%edx
	popl	%eax
	popl    %ebp
	retl

