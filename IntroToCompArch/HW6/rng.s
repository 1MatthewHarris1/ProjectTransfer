.abicalls
.option pic0
	
.rdata

	.align	2
	inSeed:
		.asciz	"Please enter a seed value (positive integer): "

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
	.globl  rng, seed
	.set	nomips16
	#.ent	main
	#.type   main, @function # tells the symbol table that `main` is a function

	
rng:
	.frame  $fp, 48, $ra        # vars= 16, regs= 2/0, args= 16, gp= 8
	
	# push the return address and frame pointer on the stack
	addiu   $sp, $sp, -48 # 48 = 4 * F
	sw      $ra, 44($sp)
	sw      $fp, 40($sp)
	move    $fp, $sp # frame pointer is ready for callee

	#===============================================================
	li	$s0, 8253729
	li	$s1, 2396403
	li	$s2, 10

	jal	seed
	mult	$v0, $s1
	mflo	$t0
	add	$s3, $t0, $s1
	div	$s3, $s2
	mfhi	$a1
	la	$a0, intFormat
	jal	printf
	
	addi	$t1, $ra, 0
	jal	exit

	#============================================================

seed:
	addi	$t1, $ra, 0

	la	$a0, inSeed
	jal	printf

	la	$a0, intFormat
	la	$a1, scanResult
	jal	scanf

	lw	$v0, scanResult

	addi	$ra, $t1, 0
	jr	$ra

exit:
	addi	$ra, $t1, 0
	jr	$ra

	#.end	main
	#.size   main, .-main
