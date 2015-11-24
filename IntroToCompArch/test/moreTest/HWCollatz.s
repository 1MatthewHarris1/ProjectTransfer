.abicalls
	.option pic0
	
	.rdata

	.align  2

	inputMessage:
		.asciiz	"Please input a positive integer: "
	intFormat:
		.asciiz	"%d"
	return:
		.asciiz "\n"
	DBG:
		.asciiz "Here"
	.data
	input:
		.word	1

	.text
	.align  2
	.globl  main
	.set    nomips16
	.ent    main
	.type   main, @function # tells the symbol table that `main` is a function

c_print:
	#Prints a format based on $a1
	#$a1 = 0: prints input message
	#$a1 = 1: prints integer in $a0

	# The following two instructions are necessary becuase printf takes argument $a0 as the address of a format string
	# and it takes $a1, $a2, etc... as the variables to be inserted based on that format string. So I need to move $a1
	# to $a2 and $a0 to $a1 to make room for the for the string format in $a0
	addi	$a2, $a1, 0		# Move $a1 to $a2
	addi	$a1, $a0, 0		# Move $a0 to $a1
	li	$t0, 1			# Load 1 into $t0. This is used to compare $a2 (now the option) to see what we will print
	li	$t1, 2

	beq	$a2, $t0, print_o1	# If $a2 == 1 jump to print_o1, else it just goes to print_o0
	beq	$a2, $t1, print_o2	# If $a2 == 2 jump to print_o2, else it just goes to print_o0

	print_o0:
		la	$a0, inputMessage	# Load the address of the inputMessage string into $a0
		j	print_oEnd		# Jump to print_oEnd

	print_o1:
		la	$a0, intFormat		# Load the address of the intFormat string into $a0
		j	print_oEnd		# Jump to print_oEnd
	
	print_o2:
		la	$a0, return		# Load a return character into $a0
		j	print_oEnd		# Jump to print_oEnd

	print_oEnd:

		# The following instruction is used to store the memory address so it is preserved when we return from printf
		addi	$s1, $ra, 0		# Put the return address into $s1
		jal	printf			# Jump and Link to printf
		addi	$ra, $s1, 0		# Put the return address back into $ra

	jr	$ra				# Jump back to the next instruction after we called c_print

c_scan:

	la	$a0, intFormat		# Load the address of the intFormat string into $a0
	la	$a1, input		# Load the address of the word called "input" into $a1

	addi	$s1, $ra, 0		# Preserve the return address in $s1
	jal	scanf			# Jump and Link to scanf
	addi	$ra, $s1, 0		# Put the return address back into #ra

	lw	$v0, input		# Load the word called "input" (now it's the user input integer) into $v0

	jr	$ra			# Jump back to the next instruction after we called c_scan
	
main:
	.frame  $fp, 48, $ra        # vars= 16, regs= 2/0, args= 16, gp= 8
	
	# push the return address and frame pointer on the stack
	addiu   $sp, $sp, -48 # 48 = 4 * F
	sw      $ra, 44($sp)
	sw      $fp, 40($sp)
	move    $fp, $sp # frame pointer is ready for callee

	#==========================

	li	$a1, 0		# Load 0 into $a1 (option 0 makes c_print just print a message, not the int in $a1)
	jal	c_print		# Jump and Link to function c_print

	jal	c_scan		# Jump and Link to function c_scan (takes no arguments)

	addi	$a0, $v0, 0	# c_scan stores the input into $v0, so put $v0 into $a0

	li	$a1, 1		# Load 1 into $a1 (option 1 makes c_print print the integer in $a0)
	jal	c_print		# Jump and link to function c_print
	
	li	$a1, 2
	jal	c_print
	
	#==========================

	# pop the return address and frame pointers off the stack
	move    $sp, $fp
	lw      $ra, 44($sp)
	lw      $fp, 40($sp)
	addiu   $sp, $sp, 48
	jr      $ra



	.end    main
	.size   main, .-main
