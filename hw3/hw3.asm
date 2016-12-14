.globl main
.data
  # we want to align everything to 8 bits (2^3 bits)
  array: .asciiz "Joe"
	.align 5
	.asciiz "Jenny"
	.align 5
	.asciiz "Jill"
	.align 5
	.asciiz "John"
	.align 5
	.asciiz "Jeff"
	.align 5
	.asciiz "Joyce"
	.align 5
	.asciiz "Jerry"
	.align 5
	.asciiz "Janice"
	.align 5
	.asciiz "Jake"
	.align 5
	.asciiz "Jonna"
	.align 5
	.asciiz "Jack"
	.align 5
	.asciiz "Jocelyn"
	.align 5
	.asciiz "Jessie"
	.align 5
	.asciiz "Jess"
	.align 5
	.asciiz "Janet"
	.align 5
	.asciiz "Jane"
	.align 5

  size: .word 16
  initial: .asciiz "Initial array is:\n"
  sorted: .asciiz "Insertion sort is finished!\n"
  space: .asciiz " "
  left: .asciiz "["
  right: .asciiz " ]\n"
.text
main:
    # save return address
    addi $sp, $sp, -4
    sw $ra, 0($sp)

    # printf(Initial array is:\n")
    la $a0, initial
    jal print_str

    # print_array(data,size)
    la $a0, array
    lw $a1, size
    jal print_array

    # insertSort(data,size)
    la $a0, array
    lw $a1, size
    jal insertion_sort
    
    # printf(Insertion sort is finished!\n")
    la $a0, sorted
    jal print_str
    
    # print_array(data,size)
    la $a0, array
    lw $a1, size
    jal print_array

    # restore ra and the stack
    lw $ra, 0($sp)
    addi $sp, $sp, 4

    # exit(0)
    li $v0, 10
    li $a0, 0
    syscall

insertion_sort:

    # save return
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    
    # assume address is in $a0 and length in $a1
    move $s0, $a0
    move $s1, $a1
    
    # int i, j
    li $s2, 1
    li $s3, 0
    
    # for lt arguments
    addi $sp, $sp, -16
    
    is_loop1:
        # i < length
    	bge $s2, $s1, is_done
    	# 32 word array, get i increment
    	mul $t4, $s2, 32
    	# address of a[i]
    	add $t4, $s0, $t4
    	
    	# char *value = a[i]
    	lw $t6, 0($t4)
    	lw $t7, 4($t4) # second word
    	
    	# for lt args
    	sw $t6, 0($sp)
    	sw $t7, 4($sp)
    	
    	# j = i - 1
    	subi $s3, $s2, 1
    	is_loop2:
    	    # 32 word array, get j increment
    	    mul $t8, $s3, 32
    	    # address of a[j]
    	    add $t8, $s0, $t8
            # j >= 0
     	    blt $s3, $zero, end_loop2

    	    # make a[j] second arg on stack
            lw $t6, 0($t8)
    	    lw $t7, 4($t8)
            sw $t6, 8($sp)
            sw $t7, 12($sp)
            
            jal str_lt
            beq $zero, $v1, end_loop2
    	    
    	    # a[j+1] = a[j]
    	    lw $t9, 0($t8)
    	    sw $t9, 32($t8)
    	    lw $t9, 4($t8) # second word
    	    sw $t9, 36($t8)
    	    
    	    # j--
    	    subi $s3, $s3, 1
    	    b is_loop2
    	end_loop2:
        # a[j+1] = value
        lw $t6, 0($sp)
    	lw $t7, 4($sp)
    	sw $t6, 32($t8)
    	sw $t7, 36($t8)
    	# i++
    	addi $s2,$s2,1
    	b is_loop1
    is_done:
        # restore ra and the stack
        addi $sp, $sp, 16
        lw $ra, 0($sp)
        addi $sp, $sp, 4
   	jr $ra
    
str_lt:
    # char *x
    la $t0, 0($sp)
    lw $t2,0($t0)
    # char *y
    la $t1, 8($sp)
    lw $t3,0($t1)
    # for
    lt_loop:
    	# load byte to go character by character
        lb $t2,0($t0)
    	lb $t3,0($t1)
    	
    	# *x!='\0' && *y!='\0'
    	beq $zero, $t2, lt_done		
	beq $zero, $t3, lt_done
	# if (*x < *y) return 1;
	blt $t2, $t3, return_one
	#if (*y < *x) return 0;		
	blt $t3, $t2, return_zero

	# x++, y++
	addi $t0, $t0, 1
	addi $t1, $t1, 1
	b lt_loop
	
    lt_done:
        # if ( *y == '\0' ) return 0;
	beq $zero, $t3, return_zero
    # else return 1  (fall through)
    return_one:
    	li $v1, 1
    	jr $ra
    return_zero:
	li $v1, 0
    	jr $ra
print_array:
    # assume address is in $a0 and size in $a1
    move $t0, $a0
    move $t1, $a1
    #lw $t1, ($a1)
    # int i = 0
    li $t2, 0
    
    # printf("[")
    la $a0, left
    li $v0, 4
    syscall
    
    # while(i < size)
    pa_loop:
    bge $t2, $t1, done
    
    # address offset in $t3
    mul $t3, $t2, 32
    # pointer to a[i]
    add $t4, $t0, $t3
    
    # printf(" 
    la $a0, space
    syscall
    # %s", a[i]
    la $a0, ($t4)
    syscall
    # i++
    addi $t2,$t2,1
    b pa_loop
    done:
    
    # printf(" ]")
    la $a0, right
    syscall
    
    li $v0, 0
    jr $ra
    
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
