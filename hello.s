	.file	"hello.c"
	.text
	.globl	msg
	.data
	.align 8
	.type	msg, @object
	.size	msg, 14
msg:
	.ascii	"hello, world!\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$14, %edx
	leaq	msg(%rip), %rsi
	movl	$1, %edi
	call	write@PLT
	movl	$77, %edi
	call	_exit@PLT
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0"
	.section	.note.GNU-stack,"",@progbitvs
