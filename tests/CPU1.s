main:                                   # Addr 0x100
	addi	sp, sp, -16
	sw	ra, 12(sp)
	sw	s0, 8(sp)
	addi	s0, sp, 16
	mv	a0, zero
	sw	a0, -12(s0)
	sw	a0, -16(s0)
	j	.LBB0_1
.LBB0_1:                                # Addr 0x120 	0 0100100000 000000000 00000 1101111
	lw	a0, -16(s0)
	addi	a1, zero, 255
	blt	a1, a0, .LBB0_4                 # 0001100 01010 01011 100 00000 1100011 # imm[12|10:5]  rs2 rs1 100 imm[4:1|11] 1100011
	j	.LBB0_2
.LBB0_2:                                # Addr 0x130	0 0100110000 000000000 00000 1101111
	lui	a0, %hi(1024)									00000000000000000000 01010 0110111
	addi	a0, a0, %lo(1024)							010000000000 01010 000 01010 0010011
	lw	a1, -16(s0)
	slli	a1, a1, 2
	add	a0, a0, a1
	flw	ft0, 0(a0)
	lui	a0, %hi(2048)									00000000000000000000 01010 0110111
	addi	a0, a0, %lo(2048)
	add	a0, a0, a1										100000000000 01010 000 01010 0010011
	flw	ft1, 0(a0)
	fsub.s	ft0, ft0, ft1
	lui	a0, %hi(4096)									 00000000000000000001 01010 0110111
	addi	a0, a0, %lo(4096)							 000000000000 01010 000 01010 0010011
	add	a0, a0, a1
	fsw	ft0, 0(a0)
	j	.LBB0_3
.LBB0_3:                                # Addr 0x170	0 0101110000 000000000 00000 1101111
	lw	a0, -16(s0)
	addi	a0, a0, 1
	sw	a0, -16(s0)
	j	.LBB0_1
.LBB0_4:								# Addr 0x180	0 0110000000 000000000 00000 1101111
	lw	a0, -12(s0)
	lw	s0, 8(sp)
	lw	ra, 12(sp)
	addi	sp, sp, 16
	ret
