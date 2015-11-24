	.abicalls
	.option pic0
	
	.rdata # read-only data
	
	.align  2
resultFormat:
	.asciz  "%d\n"

	.align	2
inSeed:
	.asciz	"Please enter a number to seed rng: "

	.align	2
intFormat:
	.asciz	"%d"

	.align	2
newLine:
	.asciz	"\n"

	.align	2
inputMessage:
	.asciz	"Your guess? (0 - 1000): "

	.align	2
yourAnswerWas:
	.asciz	"Your answer was "

	.align	2
tooLow:
	.asciz	"too low "

	.align	2
tooHigh:
	.asciz	"too high "

	.align	2
tryAgain:
	.asciz	"try again\n"

	.align	2
winMessage:
	.asciz	"correct! You WIN =D"

	.data

	.align	2
scanResult:
	.word	1
	
	.text
	.align  2
	.globl  main
	.set    nomips16
	.ent    main
	.type   main, @function # tells the symbol table that `main` is a function
	
main:
	# Need to allocate F words in the frame, where
	# F = A + L + P + (1 for $ra if needed) + (1 for $fp if in use) + padding
	# In this case,
	#  A = 2 (maximum) arguments for called functions (scanf() and printf())
	#  L = 0 words for local data
	#  P = 8 for reserved registers ($s0 - $s7)
	#  +1: $ra needed (this is a non-leaf function)
	#  +1: $fp is in use
	# padding is 0, since F is a multiple of 2 words (8 bytes)
	# Hence,
	# F = 2 + 8 + 1 + 1 = 12, hence 48 bytes

	.frame  $fp, 48, $ra        # vars= 16, regs= 2/0, args= 16, gp= 8
	
	# push the return address and frame pointer on the stack
	addiu   $sp, $sp, -48 # 48 = 4 * F
	sw      $ra, 44($sp)
	sw      $fp, 40($sp)
	move    $fp, $sp # frame pointer is ready for callee
	
	#----(implement "guess" below)----
	
	# Here's how we set the seed. In the assignment, prompt the user
	# for the seed value.

	la	$a0, inSeed
	jal	printf
	la	$a0, intFormat
	la	$a1, scanResult
	jal	scanf
	lw	$a0, scanResult
	addi	$s5, $a0, 0
	jal     seed

	Alpha:

	la	$a0, inputMessage
	jal	printf
	la	$a0, intFormat
	la	$a1, scanResult
	jal	scanf
	lw	$s6, scanResult

	addi	$a0, $s5, 0
	jal	seed
	li      $a0, 1000  # = k
	jal     rng
	addi	$s4, $v0, 0

	la	$a0, yourAnswerWas
	jal	printf

	bgt	$s6, $s4, high
	blt	$s6, $s4, low

	la	$a0, winMessage
	jal	printf
	la	$a0, newLine
	jal	printf
	j	exit

	high:
		la	$a0, tooHigh
		j	Beta
	low:
		la	$a0, tooLow
		j	Beta

	Beta:
		jal	printf
		la	$a0, tryAgain
		jal	printf
		j	Alpha
	exit:
	
	#----(implement "guess" above)----

	# pop the return address and frame pointers off the stack
	move    $sp, $fp
	lw      $ra, 44($sp)
	lw      $fp, 40($sp)
	addiu   $sp, $sp, 48
	jr      $ra

	.end    main
	.size   main, .-main
