.globl main
.data
  input_str: .asciiz "Positive integer: \n"
  output_str1: .asciiz "The value of 'factorial("
  output_str2: .asciiz ")' is: "
  newline: .asciiz "\n"
.text
main:
	la $a0, input_str 
	jal print_str #printf("Positive integer: ");
	
	#int number is in $t0, read_int syscall to store into it
	li $v0, 5
	syscall
	move $s0, $v0
	
	# add argument to the stack, but move it 12 so we can
	# store return address and get the return value later
	addi $sp, $sp, -12
	sw $s0, ($sp)
	sw $ra, 8($sp)
	# call factorial(number)
	jal factorial

	# get that return value
	lw $t0, 4($sp)

	# print the result
        la $a0, output_str1
	jal print_str #"The value of factorial("

	la $a0, ($s0)
	jal print_int #"%d"   for input
	
	la $a0, output_str2
	jal print_str #") is: "
	
	la $a0, ($t0)
	jal print_int #"%d"  for result
	
	la $a0, newline 
	jal print_str #"\n"
	
	# exit
	li $v0, 10
	# return 0
	li $a0, 0
	syscall
print_str:
	# assume string argument is in $a0
	li $v0, 4
	syscall
	li $v0, 0
	jr $ra
print_int:
	# assume integer argument is in $a0
	li $v0, 1
	syscall
	li $v0, 0
	jr $ra
factorial:
	# stack management for this recursive function:
	#	0($sp) will be the argument
	#	4($sp) will be the return value
	#	8($sp) will be the return address
	
	# check base case
	lw $t0, ($sp)
	# which is if x == 0, then branch
	beq $t0, 0, factBaseCase
	
	# otherwise subtract 1 to represent the next factorial value to multiply
	addi $t0, $t0, -1

	# adjust the stack to make room for the argument, return value, and return address
	addi $sp, $sp, -12
	# then store the return values
	sw $t0, ($sp)
	sw $ra, 8($sp)
	jal factorial

	# recall that 4($sp) should have our returned result after factorial is called
	lw $t1, 4($sp)
	# 8($sp) should hold the current return address in the recursive call
	lw $ra, 8($sp)
	# thus, 12($sp) should hold our previous returned result
	lw $t2, 12($sp)

	# and then move the stack again
	addi $sp, $sp, 12
	# we multiply the previous product and the next number
	mult $t1, $t2
	# lets just get the LO register as we can only really get factorials up to 12! in 32 bits
	mflo $t3
	# and store it as the next result (next meaning higher number)
	sw $t3, 4($sp)
	
	jr $ra

factBaseCase:
	# return 1 in the stack
	li      $t0, 1
	sw      $t0, 4($sp)
	jr      $ra