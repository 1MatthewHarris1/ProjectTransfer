	.text

	.globl main


main:
	addi	$t0, $0, 1	#$t0 = 1
	addi	$t1, $0, 2	#$t1 = 2
	addi	$t2, $0, 3	#$t2 = 3
	
	addi	$v0, $0, 4
	la	$a0, uInput
	syscall

	addi	$v0, $0, 5
	syscall
	addi	$s0, $a0, 0

	blt	$s0, $0, error

alpha:
	div	$s0, $t1	#divide uInput by 2
	mfhi	$t4
	bne	$t4, $0, odd	#if(($s0 % 2) != 0) go to label odd
	mflo	$s0

beta:
	addi	$v0, $0, 1
	addi	$a0, $s0, 0
	syscall
	bne	$s0, $t0, alpha	#if $s0 != 1 go to label alpha
	j	exit
	
odd:
	mult	$s0, $t2
	mflo	$s0
	mfhi	$t4
	addi	$s0, $s0, 1
	bne	$t4, $0, error
		
	j	beta
	
error:
	la	$a0, errorOutput
	addi	$v0, $0, 4
	syscall

exit:
	addi	$v0, $0, 10
	syscall

	.data

uInput:		.ascii "Please input a positive integer: /000"

errorOutput:	.ascii "Your input caused an error/000"

















