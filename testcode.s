	.text
	.align	2
	.globl	f
	.type	f, @function
f:
addiu	$sp,$sp,-40
sw	$fp,36($sp)
move	$fp,$sp
lw	$8,12($fp)
move	$8,$4
sw	$8,12($fp)
lw	$8,16($fp)
move	$8,$5
sw	$8,16($fp)
lw	$8,12($fp)
sw	$8,12($fp)
lw	$8,24($fp)
li	$8,3
sw	$8,24($fp)
lw	$8,24($fp)
sw	$8,12($fp)
lw	$8,16($fp)
sw	$8,16($fp)
lw	$8,32($fp)
li	$8,4
sw	$8,32($fp)
lw	$8,32($fp)
sw	$8,16($fp)
lw	$8,12($fp)
sw	$8,12($fp)
lw	$8,16($fp)
sw	$8,16($fp)
lw	$8,12($fp)
lw	$9,16($fp)
mult	$8,$9
mflo	$8
sw	$8,36($fp)
lw	$2,36($fp)
move	$sp,$fp
lw	$fp,36($sp)
addiu	$sp,$sp,40
j	$31
nop
