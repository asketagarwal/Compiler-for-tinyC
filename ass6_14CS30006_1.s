	.section	.rodata
.LC0:
	.string "Example of passing pointer to function\n"
.LC1:
	.string "Value passed : "
.LC2:
	.string "\n"
.LC3:
	.string "Value returned  is: "
.LC4:
	.string "\n"
.LC5:
	.string "Read an integer!!"
.LC6:
	.string "\n"
.LC7:
	.string "The integer  read is:"
.LC8:
	.string "\n"
  # function printi start
  # function prints start
  # function readi start
  # function f start
#--param_offset:24
#--loacl var offset: 4 -4
#--loacl var offset: 4 -8
#--loacl var offset: 4 -12
#--loacl var offset: 4 -16
#-16
##ptra
##ptrx
##ptrt0
##ptrt1
##ptrt2
##ptrretVal
	.globl	f
	.type	f, @function
f:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$48,	%rsp
  # t0= *a
	movq	16(%rbp),	%rax
	movl	(%rax),	 %ecx
	movl	%ecx,	-8(%rbp)
  # x = t0
	movl	%eax,	-4(%rbp)
  # t1 = 5
	movl	$5,	-12(%rbp)
  # x+t1
	movl	-4(%rbp),	%edx
	movl	-12(%rbp),	%eax
	addl	 %edx,	%eax
	movl	 %eax,	-16(%rbp)
  # x = t2
	movl	-16(%rbp),	%eax
	movl	%eax,	-4(%rbp)
  # return x
	movq	-4(%rbp),	%rax
	leave
	ret
  # function f end
	leave
	ret
	.size	f,	.-f
  # function main start
#--loacl var offset: 4 -4
#--loacl var offset: 4 -8
#--loacl var offset: 8 -16
#--loacl var offset: 4 -20
#--loacl var offset: 8 -28
#--loacl var offset: 4 -32
#--loacl var offset: 4 -36
#--loacl var offset: 4 -40
#--loacl var offset: 4 -44
#--loacl var offset: 4 -48
#--loacl var offset: 4 -52
#--loacl var offset: 4 -56
#--loacl var offset: 4 -60
#--loacl var offset: 4 -64
#--loacl var offset: 4 -68
#--loacl var offset: 4 -72
#--loacl var offset: 4 -76
#--loacl var offset: 4 -80
#--loacl var offset: 4 -84
#-84
##ptra
##ptrq
##ptre
##ptrt3
##ptrt4
##ptrprints
##ptr.LC0
##ptr.LC1
##ptrprinti
##ptr.LC2
##ptrf
##ptr.LC3
##ptr.LC4
##ptr.LC5
##ptr.LC6
##ptrreadi
##ptr.LC7
##ptr.LC8
##ptrt5
##ptrretVal
	.globl	main
	.type	main, @function
main:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$112,	%rsp
  # t3 = 3
	movl	%eax,	-20(%rbp)
  # q = t3
	movl	-20(%rbp),	%eax
	movl	%eax,	-8(%rbp)
  # t4= &q
	leaq	-8(%rbp),	%rax
	movq	%rax,	-28(%rbp)
  # e = t4
	movl	-28(%rbp),	%eax
	movl	%eax,	-16(%rbp)
  # param .LC0
	movq	$.LC0,	%rdi
  # call prints 1
	call	prints
  # param .LC1
	movq	$.LC1,	%rdi
  # call prints 1
	call	prints
  # param q
	movq	-8(%rbp),	%rdi
  # call printi 1
	call	printi
  # param .LC2
	movq	$.LC2,	%rdi
  # call prints 1
	call	prints
  # param e
  # call f 1
	movq	-16(%rbp),	%rax
	pushq	%rax
	call	f
	addq	$8,	%rsp
#--
  # a = f
	movl	%eax,	-4(%rbp)
  # param .LC3
	movq	$.LC3,	%rdi
  # call prints 1
	call	prints
  # param a
	movq	-4(%rbp),	%rdi
  # call printi 1
	call	printi
  # param .LC4
	movq	$.LC4,	%rdi
  # call prints 1
	call	prints
  # param .LC5
	movq	$.LC5,	%rdi
  # call prints 1
	call	prints
  # param .LC6
	movq	$.LC6,	%rdi
  # call prints 1
	call	prints
  # param e
	movq	-16(%rbp),	%rdi
  # call readi 1
	call	readi
  # q = readi
	movl	%eax,	-8(%rbp)
  # param .LC7
	movq	$.LC7,	%rdi
  # call prints 1
	call	prints
  # param q
	movq	-8(%rbp),	%rdi
  # call printi 1
	call	printi
  # param .LC8
	movq	$.LC8,	%rdi
  # call prints 1
	call	prints
  # t5 = 0
	movl	%eax,	-84(%rbp)
  # return t5
	movq	-84(%rbp),	%rax
	leave
	ret
  # function main end
	leave
	ret
	.size	main,	.-main
