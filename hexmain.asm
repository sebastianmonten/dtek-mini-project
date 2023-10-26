  # hexmain.asm
  # Written 2015-09-04 by F Lundevall
  # Copyright abandonded - this file is in the public domain.

	.text
main:
	li	$a0,17		# change this to test different values

	jal	hexasc		# call hexasc
	nop			# delay slot filler (just in case)	

	move	$a0,$v0		# copy return value to argument register

	li	$v0,11		# syscall with v0 = 11 will print out
	syscall			# one byte from a0 to the Run I/O window
	
stop:	j	stop		# stop after one run
	nop			# delay slot filler (just in case)

  # You can write your own code for hexasc here
  #

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

num:
	# number
	addi	$v0, $a0, 0x30	# set v0 t0 a0 + 48, (a0 + 0x30). This will map 0-9 to 0x30-0x39
	jr $ra
