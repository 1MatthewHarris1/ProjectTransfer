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

		

	#============================================================
	
	jr $ra

	.end	main
	.size   main, .-main
