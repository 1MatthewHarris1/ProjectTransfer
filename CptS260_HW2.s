# Matthew Harris
# CptS 260: Introduction to Computer Architecture
# Dr. Lewis
# Homework 2

    .abicalls
    .option pic0
    
    .rdata # read-only data
    
    .align  2
resultFormat:
    .asciz  "(%d/%u)\n"
    
    .text
    .align  2
    .globl  main
    .set    nomips16
    .ent    main
    .type   main, @function # tells the symbol table that `main` is a function
    
rat_print:
    # ($a0, $a1) -> () (no output registers)
    # prints "(i/j)" on stdout, where i = $a0 and j = $a1
    addiu $sp, $sp, -4
    sw    $ra, ($sp)
    
    move $a2, $a1    
    move $a1, $a0
    la   $a0, resultFormat
    jal  printf
    
    lw    $ra, ($sp)
    addiu $sp, $sp, +4

    jr    $ra

# put your additional rat_* functions here

rat_mul:
  mult	$a0, $a2
  mflo  $v0
  mult  $a1, $a3
  mflo  $v1
  jr    $ra

rat_add:
  mult  $a0, $a3
  mflo  $t0
  mult  $a1, $a3
  mflo  $t1
  mult  $a1, $a2
  mflo  $t2
 
  add   $v0, $t0, $t2
  add   $v1, $t1, $0

  jr    $ra

rat_sub:

  sub	$a2, $0, $a2; 
  add   $s0, $ra, $0
  jal   rat_add
  add   $ra, $s0, $0

  jr    $ra

rat_div:
#ignore the comments. That's just me being crazy
  add   $t0, $a2, $0
  add   $a2, $a3, $0
  add   $a3, $t0, $0
 
  add	$s0, $ra, $0 
  #$ra = PC
#  ble	$a3, $0, rat_flip			
  jal   rat_mul
  add   $ra, $s0, $0

 # add	$s1, $ra, $0
 # jal	rat_simplify
 # add	$ra, $s1, $0

  jr    $ra
rat_flip:

  li	$t0, -1
  mult	$a2, $t0
  mflo	$a2
  mult	$a3, $t0
  mflo	$a2

  jr	$ra

rat_gcd: #As found in the examples posted online (modified slightly):

			        # This is Euclid's algorithm.
    div     $0, $a0, $a1 	# results are in `lo` and `hi` registers
    mfhi    $t2    		# copy `c`, the remainder, from the `hi` register to $s2
    beq     $t2, $0, rat_jump	# jump to the return address in $ra if no remainder is found
    move    $a0, $a1 # a = b
    move    $a1, $t2 # b = c	# (TODO) figure out where the result is being stored ($a0 or $a1) and put it in $v0
    add		$s2, $ra, $0
    jal rat_print
    add		$ra, $s2, $0
    j       rat_gcd

rat_jump:
	jr	$ra
rat_simplify:

	add	$s0, $ra, $0
	add	$a0, $v0, $0
	add	$a1, $v1, $0
	jal	rat_gcd
	add	$ra, $s0, $0	
	div	$v0, $t0
	mflo	$v0
	div	$v1, $t0
	mflo	$v1

	jr $ra

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
    # F = 2 + 8 + 1 + 1 = 12 words, hence 48 bytes

    .frame  $fp, 48, $ra        # vars= 16, regs= 2/0, args= 16, gp= 8
    
    # push the return address and frame pointer on the stack
    addiu   $sp, $sp, -48 # 48 = 4 * F
    sw      $ra, 44($sp)
    sw      $fp, 40($sp)
    move    $fp, $sp # frame pointer is ready for callee
    
    # ------ Test code begins here.
    
    # Put some test code here for your rat_* routines. (The instructor
    # will have a different test "main", but you'll want to test your
    # code for yourself.)
    
    # Here is an example of a call to rat_print()

    li      $a0,1
    li      $a1,2
    jal     rat_print

    li      $a0,2
    li      $a1,2
    li      $a2,-1
    li      $a3,2
    jal     rat_div
    move	$a0, $v0
    move	$a1, $v1
    
    jal rat_print

    # result:
    # $v0: numerator
    # $v1: denominator

    # ------ Test code ends here.

    # pop the return address and frame pointers off the stack
    move    $sp, $fp
    lw      $ra, 44($sp)
    lw      $fp, 40($sp)
    addiu   $sp, $sp, 48
    jr      $ra

    .end    main
    .size   main, .-main
