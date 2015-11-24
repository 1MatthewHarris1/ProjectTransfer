.abicalls
.option pic0
	
.rdata

	.align  2
	inputMessage:
		.asciz "Please input a positive integer: "

	.align  2
	intFormat:
		.asciz "%d"
	
	.align 2
	newLine:
		.asciz "\n"

	.align 2
	counter:
		.asciz "%d: "

	.align	2
	errorOutput:
		.asciz "\nYour input caused an error\n"

	.align 2
	inputIsEven:
		.asciz "\nYour input was even\n"

	.align 2
	inputIsOdd:
		.asciz "\nYour input was odd\n"

	.align  2
	DBG:
		.asciz "Here"

.data
	.align  2
	scanResult:
		.word 1

	.text
	.align  2
	.globl  main
	.set	nomips16
	.ent	main
	.type   main, @function # tells the symbol table that `main` is a function

	
main:
	.frame  $fp, 48, $ra        # vars= 16, regs= 2/0, args= 16, gp= 8
	
	# push the return address and frame pointer on the stack
	addiu   $sp, $sp, -48 # 48 = 4 * F
	sw      $ra, 44($sp)
	sw      $fp, 40($sp)
	move    $fp, $sp # frame pointer is ready for callee

	
	#===============================================================

	#Input a number
	li	$s2, 1

	la	$a0, inputMessage
	jal	printf

	la	$a0, intFormat
	la	$a1, scanResult
	jal	scanf
	lw	$s0, scanResult

	ble	$s0, $0, error
	
alpha:
	
	#Print the number
	la	$a0, counter
	addi	$a1, $s2, 0
	jal	printf
	addi	$s2, $s2, 1
	la	$a0, intFormat
	addi	$a1, $s0, 0
	jal	printf
	la	$a0, newLine
	jal	printf

	#Determine if it is even or odd
	li	$t0, 2
	addi	$s1, $s0, 0
	div	$s1, $t0
	mfhi	$t0

	bne	$t0, $0, odd

	addi	$s0, $s1, 0
	#la	$a0, inputIsEven
	#jal	printf

beta:

	#Exit if the number == 1
	li	$t0, 1
	beq	$s0, $t0, lastPrint
	j	alpha

odd:
	#la	$a0, inputIsOdd
	#jal	printf
	
	#Multiply by 3 and add 1
	li	$t0, 3
	mult	$s0, $t0
	mflo	$s0
	addi	$s0, $s0, 1
	mfhi	$s1
	bne	$s1, $0, error

	#Jump back to Beta
	j	beta

error:

	la	$a0, errorOutput
	jal	printf
	j	exit

lastPrint:

	la	$a0, counter
	addi	$a1, $s2, 0
	jal	printf
	addi	$s2, $s2, 1
	la	$a0, intFormat
	addi	$a1, $s0, 0
	jal	printf
	la	$a0, newLine
	jal	printf

exit:

	# pop the return address and frame pointers off the stack
	move    $sp, $fp
	lw      $ra, 44($sp)
	lw      $fp, 40($sp)
	addiu   $sp, $sp, 48

	#============================================================
	
	jr $ra

	.end	main
	.size   main, .-main
