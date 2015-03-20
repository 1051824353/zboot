	.code16
	.text

	.globl	bootsec
stack		= 0x7c00

DISK_INFO       = 0x800
BPB_BytsPerSec  = (DISK_INFO)
BPB_SecPerClus  = (DISK_INFO + 2)
BPB_RsvdSecCnt  = (DISK_INFO + 4)
BPB_FatSz32     = (DISK_INFO + 6)
BPB_NumFats     = (DISK_INFO + 10)
FAT_START	= (DISK_INFO + 12)
ROOT_START	= (DISK_INFO + 14)
ROOT_SEC_START	= (DISK_INFO + 22)
INIT_START	= (DISK_INFO + 26)
INIT_SEC_START	= (DISK_INFO + 34)
INIT_SIZE	= (DISK_INFO + 38)
INIT_SEC_SIZE   = (DISK_INFO + 42)

DiskAddPaceket  = 0x900
PacketSize      = (DiskAddPaceket)
Reserved        = (DiskAddPaceket + 1)
BlockCount      = (DiskAddPaceket + 2)
BufferAddr      = (DiskAddPaceket + 4)
BlockNum        = (DiskAddPaceket + 8)

DiskCacheAdd	= 0x00007c00




	/* gas/ld has issues with doing this as absolute addresses... */
	.section ".bootsec", "a", @nobits
	.globl	bootsec
bootsec:
	.space	512

	.text
	.globl	_start
_start:
	cli
	xorw	%ax, %ax
	movw	%ax, %ds
	movw	%ax, %ss
	movw	$stack, %sp
	movw	%sp, %si
	pushw	%es		/* es:di -> $PnP header */
	pushw	%di
	movw	%ax, %es
	sti
	cld
	
	 /* Copy down to 0:0x600 */
	movw    $_start, %di 
	movw    $(512/2), %cx 
	rep; movsw

	ljmpw	$0, $next
next:
	call    load_fat_info
	call	load_first_file
	movw    $DiskCacheAdd, %sp    /* driveno == bootsec-6 */
	cli
	jmpw    *%sp            /* %sp == bootsec */

load_first_file:
	movl    ROOT_SEC_START,%eax
	movw	$0x0001,%cx
	call    read_exsector
	movw    $DiskCacheAdd,%bx
	movw    0x34(%bx),%ax
	sal	$8,%ax
	movw    0x3a(%bx),%ax
	subl    $2,%eax
	xorl	%ecx,%ecx
	movb	BPB_SecPerClus,%cl
	mulb	%cl
	movw	BPB_BytsPerSec,%cx
	mulw	%cx
	movl	ROOT_START,%ecx
	addl	%ecx,%eax
	movl	%eax,INIT_START
	mov     $0x200,%ecx
        divl    %ecx
        movl    %eax,INIT_SEC_START
	movl    0x3c(%bx),%eax
	movl	%eax,INIT_SIZE
	mov     $0x200,%ecx
	divl    %ecx
	cmpl	$0,%eax
	je	L1
	addl	$1,%eax
	movl    %eax,INIT_SEC_SIZE
	jmp	L2
L1:
	movl	$1,INIT_SEC_SIZE
L2:
	movl    INIT_SEC_START,%eax
    movl    INIT_SEC_SIZE,%ecx
    call    read_exsector
	ret	

load_fat_info:
    movl    $0x0000003f,%eax
	movw	$0x0001,%cx
    call    read_exsector
    movw    $DiskCacheAdd,%bx
    movw    0xb(%bx),%ax
    movw    %ax,BPB_BytsPerSec
    movw    0xd(%bx),%ax
    movw    %ax,BPB_SecPerClus
    movw    0xe(%bx),%ax
    movw    %ax,BPB_RsvdSecCnt
    movl    0x24(%bx),%eax
    movl    %eax,BPB_FatSz32
    movw    0x10(%bx),%ax
    movw    %ax,BPB_NumFats
	movw    BPB_BytsPerSec,%cx
	movw    BPB_RsvdSecCnt,%ax	
	mulw    %cx
	movw 	%ax,FAT_START
	movw    BPB_FatSz32,%ax
	mull    %ecx
	movw    $0x2,%cx
	mull    %ecx /* 2 fat */
	movw    FAT_START,%dx
	addl    %edx,%eax
	addl    $0x7e00,%eax
	movl	%eax,ROOT_START
	xor	%edx,%edx
	mov	$0x200,%ecx
	divl	%ecx
	movl    %eax,ROOT_SEC_START
    ret

read_exsector:
	movb	$0x10, PacketSize
	movb	$0x00, Reserved
	movw	%cx, BlockCount	/*0x0001*/
	movl	$DiskCacheAdd,BufferAddr
	movl	%eax,BlockNum
	
	mov	$0x80,%dl
	push	%si
	mov	$DiskAddPaceket,%si
	movb 	$0x42,%ah
	movb	$0x00,%al
	int	$0x13
	pop	%si
	ret


