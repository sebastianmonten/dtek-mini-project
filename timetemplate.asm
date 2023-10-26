  # timetemplate.asm
  # Written 2015 by F Lundevall
  # Copyright abandonded - this file is in the public domain.

.macro	PUSH (%reg)
	addi	$sp,$sp,-4
	sw	%reg,0($sp)
.end_macro

.macro	POP (%reg)
	lw	%reg,0($sp)
	addi	$sp,$sp,4
.end_macro

	.data
	.align 2
mytime:	.word 0x5957
timstr:	.ascii "text more text lots of text\0"
	.text
main:
	# print timstr
	la	$a0,timstr
	li	$v0,4
	syscall
	nop
	# wait a little
	li	$a0, 1000 ################################################# <- delay of 1000 ms
	jal	delay
	nop
	# call tick
	la	$a0,mytime
	jal	tick
	nop
	# call your function time2string
	la	$a0,timstr
	la	$t0,mytime
	lw	$a1,0($t0)
	jal	time2string
	nop
	# print a newline
	li	$a0,10
	li	$v0,11
	syscall
	nop
	# go back and do it all again
	j	main # jump to main
	nop
# tick: update time pointed to by $a0
tick:	lw	$t0,0($a0)	# get time
	addiu	$t0,$t0,1	# increase
	andi	$t1,$t0,0xf	# check lowest digit
	sltiu	$t2,$t1,0xa	# if digit < a, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0x6	# adjust lowest digit
	andi	$t1,$t0,0xf0	# check next digit
	sltiu	$t2,$t1,0x60	# if digit < 6, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0xa0	# adjust digit
	andi	$t1,$t0,0xf00	# check minute digit
	sltiu	$t2,$t1,0xa00	# if digit < a, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0x600	# adjust digit
	andi	$t1,$t0,0xf000	# check last digit
	sltiu	$t2,$t1,0x6000	# if digit < 6, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0xa000	# adjust last digit
tiend:	sw	$t0,0($a0)	# save updated result
	jr	$ra		# return
	nop

# you can write your code for subroutine "hexasc" below this line
# we expect a value in a0, only its 4 lsb is used. rest ignored
# values 1010 to 1111 will result in ASCII A to F
# result: 7 lsb in v0 is ASCII code. all bits before is 0. 
# idea: if 1010 <= a0 <= 1111, then set v0 to a0+31
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


delay_original:
 jr $ra
 nop
 

delay: # anders rekursiva
	move $t0, $zero
	loop:
		addi $t0, $t0, 1	# increase the inner loop ctr by 1 at every iteration of the loop
		blt $t0, 0xAD, loop	# change the constant to any reasonable value
		nop

		
	addi $a0, $a0, -1		# change the outer loop
	blt $a0 1 end_delay 	# if delay < 1, end this function at this point
	nop
	j delay
	nop 
	
	end_delay:
		jr $ra
		nop



delay2: # a0 is delay time in ms
	move $t0, $a0 # t0 = a0, ms from C code
	
	# while loop
wloop:
	blt $t0, 1, wdone # if t0 < 1, then go to wdone
	nop
	addi $t0, $t0, -1 # else, t0--
	
	# foor loop
	addi $t1, $0, 0 #t1 = 0, i from C code
	li $t2, 0xAD  #t2 = 845 = 0x34D
floop:
	bgt $t1, $t2, fdone # if t1 > 845, then go to fdone
	nop
	add $t1, $t1, 1 # else, t1++
	j floop # next iteration of for loop
	nop
fdone:
	j wloop # next iteration of while loop
	nop
	
wdone:
	jr $ra
	nop
 

# $a0 contains the address of an area in memory
# $a1 16 lsb time-info, organized as four NBCD-coded digits of 4 bits each. 16 msb are ignored, can be anything
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
	
	# null byte
	sb $0, 5($s0) # we store 0x00 to the last byte
	
	
	POP $s1
	POP $s0
	POP $ra
	
	jr $ra
	nop
	
	
	
	
	
	
	
	
