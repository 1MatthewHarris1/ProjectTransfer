	.abicalls
	.option pic0
	
	.rdata # read-only data
	
	.align  2
resultFormat:
	.asciz  "%d\n"
	
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
	li      $a0, 123456
	jal     seed

	# Here's how to call rng(k)
	li      $a0, 1000000  # = k
	jal     rng

	# reminder of how to call printf().
	la      $a0, resultFormat
	move    $a1, $v0
	jal     printf

	
	
	#----(implement "guess" above)----

	# pop the return address and frame pointers off the stack
	move    $sp, $fp
	lw      $ra, 44($sp)
	lw      $fp, 40($sp)
	addiu   $sp, $sp, 48
	jr      $ra

	.end    main
	.size   main, .-main
