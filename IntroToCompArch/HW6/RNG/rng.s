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
	.globl  main, rng, seed
	.set	nomips16
	#!!.ent	main
	#!!.type   main, @function # tells the symbol table that `main` is a function
	.ent rng
	.type	rng, @function
	.type	seed, @function

#!!main:
rng:
	.frame  $fp, 48, $ra        # vars= 16, regs= 2/0, args= 16, gp= 8
	
	# push the return address and frame pointer on the stack
	addiu   $sp, $sp, -48 # 48 = 4 * F
	sw      $ra, 44($sp)
	sw      $fp, 40($sp)
	move    $fp, $sp # frame pointer is ready for callee

	#===============================================================

	addi	$s7, $ra, 0

	addi	$s0, $a0, 0
	li	$s2, 8253729
	li	$s3, 2396403

	mult	$s1, $s2
	mflo	$t1
	add	$t1, $t1, $s3
	addi	$s1, $t1, 0
	div	$t1, $s0
	mfhi	$v0
	bge	$v0, $0, done
	li	$t1, -1
	mult	$v0, $t1
	mflo	$v0
	done:
	j	exit

seed:	# void seed(int seed) // Updates seed (stored in $s1) with argument ($a0)

	addi	$s1, $a0, 0

	jr	$ra

exit:
	addi	$ra, $s7, 0

	#============================================================

	# pop the return address and frame pointers off the stack
	move    $sp, $fp
	lw      $ra, 44($sp)
	lw      $fp, 40($sp)
	addiu   $sp, $sp, 48

	jr	$ra

	#!!.end	main
	#!!.size   main, .-main
	.end rng
	.size	rng, .-rng
