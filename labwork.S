  # labwork.S
  # Written 2015-2017 by F Lundevall
  # Skeleton file for IS1200/IS1500 lab 1.
  # The macros PUSH and POP are in the public domain.
  # Please add your own code at the end of the file.

  #
  # Please keep the two macros PUSH and POP unchanged
  #
.macro	PUSH reg
	addi	$sp,$sp,-4
	sw	\reg,0($sp)
.endm

.macro	POP reg
	lw	\reg,0($sp)
	addi	$sp,$sp,4
.endm
  #
  # Please add your own code below this line

.data
.align 2
msg:	.space 8

.text

# you can write your code for subroutine "hexasc" below this line
# we expect a value in a0, only its 4 lsb is used. rest ignored
# values 1010 to 1111 will result in ASCII A to F
# result: 7 lsb in v0 is ASCII code. all bits before is 0. 
# idea: if 1010 <= a0 <= 1111, then set v0 to a0+31
.global hexasc
hexasc:
	andi	$a0, $a0, 0xF # we and immideate with binary 0000000000001111 (0xF), effectively setting all but the last for bits of a0 to 0
	
	blt     $a0, 0xA, num # if a0 < 10, we have a number, jump to num. Else we have letter and we continue
	nop
letter:
	# letter
	addi	$v0, $a0, 0x37	# set v0 to a0 + 55, (a0 + 0x37). This will map 10-15 to 0x41-0x46
	jr $ra
	nop

num:
	# number
	addi	$v0, $a0, 0x30	# set v0 t0 a0 + 48, (a0 + 0x30). This will map 0-9 to 0x30-0x39
	jr $ra
	nop


 
.global delay
delay: # anders rekursiva
	move $t0, $zero
	loop:
		addi $t0, $t0, 1	# increase the inner loop ctr by 1 at every iteration of the loop
		# blt $t0, 0xAD, loop	# change the constant to any reasonable value
		blt $t0, 6700, loop
		nop

		
	addi $a0, $a0, -1		# change the outer loop
	blt $a0, 1, end_delay 	# if delay < 1, end this function at this point
	nop
	j delay
	nop 
	
	end_delay:
		jr $ra
		nop
 

# $a0 contains the address of an area in memory
# $a1 16 lsb time-info, organized as four NBCD-coded digits of 4 bits each. 16 msb are ignored, can be anything
.global time2string
time2string:
	PUSH $ra
	PUSH $s0
	PUSH $s1
	
	move $s0, $a0 # dest adress now in s0
	move $s1, $a1 # input string now in s1
	
	
	# minute digit 1
	srl $a0, $s1, 12 # shift right logical of input so that the first  NBCD-coded digit is on the 4 lsb of a0
	jal hexasc # -> v0 = asci for the first digit
	nop
	sb $v0, ($s0)
	
	# minute digit 2
	srl $a0, $s1, 8 # shift right logical of input so that the first  NBCD-coded digit is on the 4 lsb of a0
	jal hexasc # -> v0 = asci for the first digit
	nop
	sb $v0, 1($s0)
	
	# colon
	li $t0, 0x3A  # temporary load 0x3A in t0
	sb $t0, 2($s0) 
	
		
	
	# second digit 1
	srl $a0, $s1, 4 # shift right logical of input so that the first  NBCD-coded digit is on the 4 lsb of a0
	jal hexasc # -> v0 = asci for the first digit
	nop
	sb $v0, 3($s0)
	
	# second digit 2
	move $a0, $s1
	jal hexasc # -> v0 = asci for the first digit
	nop
	sb $v0, 4($s0)
	
	
	# check seconds 00
	andi $t0, $s1, 0xFF # t0 = 0000 0000 0000 0000 0000 0000 0000 ssss ssss
	bnez $t0, nullbyte
	li $t0, 'X'
	sb $t0, 5($s0)
	sb $0, 6($s0)
	j end_time2string
	nop
	
	
nullbyte:
	# null byte
	sb $0, 5($s0) # we store 0x00 to the last byte
	
end_time2string:
	POP $s1
	POP $s0
	POP $ra
	
	jr $ra
	nop
	

# function that executes the ei instruction (and then returns to the caller). The ei instruction enables interrupts globally
.global enable_interrupt
enable_interrupt:
	ei
	nop

	jr $ra
	nop
	
	
	
	
	
stop: j stop 
