main:                                   # Addr 0x0
	addi	sp, sp, -16					# imm[11:0]     rs1	000 rd 		 0010011	# 111111110000 00010 000 00010  0010011		# Allocate 16 bytes to the STACK			# x2 = sp = -16
	sw	ra, 12(sp)						# imm[11:5] rs2 rs1 010 imm[4:0] 0100011	# 0000000 00001 00010 010 01100 0100011		# Saves return address on STACK into memory address (sp + 12)
	sw	s0, 8(sp)						# imm[11:5] rs2 rs1 010 imm[4:0] 0100011	# 0001000 01000 00010 010 01000 0100011		# Saves Saved Register on STACK into memory address (sp + 8)
	addi	s0, sp, 16					# imm[11:0]     rs1	000 rd 		 0010011	# 000000010000 00010 000 01000  0010011		# Adds value in stack pointer (sp) to 16 and stores the sum in s0						# s0 = sp + 16
	mv	a0, zero						# 000000000000   rs1	000 rd 	 0010011	# 000000000000 00000 000 01010  0010011		# Adds x0 + 0 to a0							# a0 = 0
	sw	a0, -12(s0)						# imm[11:5] rs2 rs1 010 imm[4:0] 0100011	# 1111111 01010 01000 010 10100 0100011		# Stores word from a0 into memory address(s0 - 12)
	sw	a0, -16(s0)						# imm[11:5] rs2 rs1 010 imm[4:0] 0100011	# 1111111 01010 01000 010 10000 0100011		# Stores word from a0 into memory address(s0 - 16)
	j	.LBB0_1							# imm[20|10:1|11|19:12] rd       1101111	# 0 0000010000 000000000 00000 1101111		# Return from subroutine (return to normal code)
.LBB0_1:                                # Addr 0x20=32=0000010000 0									# For loop statement
	lw	a0, -16(s0)						# imm[11:0]		rs1 010 rd       0000011	# 111111110000 01000 010 01010 0000011		# Load old value from STACK. Load double word from memory address (s0 - 16) into a0		# for(i = 0; rs1 < rs2; i++)
	addi	a1, zero, 255				# imm[11:0]     rs1	000 rd 		 0010011	# 000011111111 00000 000 01011 0010011		# a1 = x0 + 255
	blt	a1, a0, .LBB0_4			 		# imm[12|10:5]  rs2 rs1 100 imm[4:1|11] 1100011		#0000100 01010 01011 100 00000 1100011		# Branch if rs1 < rs2 Exit: exits the loop if condition is met
	j	.LBB0_2							# imm[20|10:1|11|19:12] rd       1101111	# 00000011000 000000000 00000 1101111	      Location 4
.LBB0_2:                                # Addr 0x30									  Loop body
	lui	a0, %hi(1024)					# imm[31:12]			rd		 0110111	# 00000000000000000000 01010 0110111		Location 48
	addi	a0, a0, %lo(1024)			# imm[11:0]     rs1	000 rd 		 0010011	# 010000000000 01010 000 01010 0010011
	lw	a1, -16(s0)						# imm[11:0]		rs1 010 rd       0000011	# 111111110000 01000 010 01011 0000011
	slli	a1, a1, 2					# 0000000 shamt rs1 001 rd		 0010011	# 0000000 00010 01011 001 01011
	add	a0, a0, a1						# 0000000 rs2   rs1 000 rd		 0110011	# 0000000 01011 01010 000 01010 0110011
	flw	ft0, 0(a0)						# imm[11:0] 	rs1 010 rd		 0000111	# 000000000000 01010 010 00000 0000111
	lui	a0, %hi(2048)					# imm[31:12]			rd		 0110111	# 00000000000000000000 01010 0110111
	addi	a0, a0, %lo(2048)			# imm[11:0]     rs1	000 rd 		 0010011	# 100000000000 01010 000 01010 0010011
	add	a0, a0, a1						# 0000000 rs2   rs1 000 rd		 0110011	# 0000000 01011 01010 000 01010 0110011
	flw	ft1, 0(a0)						# imm[11:0] 	rs1 010 rd		 0000111	# 000000000000 01010 010 00001 0000111
	fadd.s	ft0, ft0, ft1				# 0000000 rs2   rs1 rm 	rd		 1010011	# 0000000 00001 00000 111 00000 1010011
	lui	a0, %hi(3072)					# imm[31:12]			rd		 0110111	# 00000000000000000000 01010 0110111
	addi	a0, a0, %lo(3072)			# imm[11:0]     rs1	000 rd 		 0010011	# 110000000000 01010 000 01010 0010011
	add	a0, a0, a1						# 0000000 rs2   rs1 000 rd		 0110011	# 0000000 01011 01010 000 01010 0110011
	fsw	ft0, 0(a0)						# imm[11:5] rs2 rs1 010 imm[4:0] 0100111	# 0000000 00000 01010 010 00000 0100111
	j	.LBB0_3							# imm[20|10:1|11|19:12] rd       1101111	# 00000111000000000000    00000 1101111
.LBB0_3:                                # Addr 0x70
	lw	a0, -16(s0)						# imm[11:0]		rs1 010 rd       0000011	# 111111110000 01000 010 01010 0000011
	addi	a0, a0, 1					# imm[11:0]     rs1	000 rd 		 0010011	# 000000000001 01010 000 01010 0010011
	sw	a0, -16(s0)						# imm[11:5] rs2 rs1 010 imm[4:0] 0100011	# 11111111 01010 01000 010 0000 0100011
	j	.LBB0_1							# imm[20|10:1|11|19:12] rd       1101111	# 00000001000 000000000 00000 1101111		# iterate the for loop
.LBB0_4:								# Addr 0x80							        # After the loop
	lw	a0, -12(s0)						# imm[11:0]		rs1 010 rd       0000011	# 111111110100 01000 010 01010 0000011
	lw	s0, 8(sp)						# imm[11:0]		rs1 010 rd       0000011	# 000000001000 00010 010 01000 0000011
	lw	ra, 12(sp)						# imm[11:0]		rs1 010 rd       0000011	# 000000001100 00010 010 00001 0000011
	addi	sp, sp, 16					# imm[11:0]     rs1	000 rd 		 0010011	# 000000010000 00010 000 00010 0010011
	ret									# imm[11:0] 	rs1 000 rd		 1100111	# 000000000000 00001 000 00000 1100111




	main:                                   # Addr 0x0
		addi	sp, sp, -16
		sw	ra, 12(sp)
		sw	s0, 8(sp)
		addi	s0, sp, 16
		mv	a0, zero
		sw	a0, -12(s0)
		sw	a0, -16(s0)
		j	.LBB0_1
	.LBB0_1:                                # Addr 0x20
		lw	a0, -16(s0)
		addi	a1, zero, 255
		blt	a1, a0, .LBB0_4
		j	.LBB0_2
	.LBB0_2:                                # Addr 0x30
		lui	a0, %hi(1024)
		addi	a0, a0, %lo(1024)
		lw	a1, -16(s0)
		slli	a1, a1, 2
		add	a0, a0, a1
		flw	ft0, 0(a0)
		lui	a0, %hi(2048)
		addi	a0, a0, %lo(2048)
		add	a0, a0, a1
		flw	ft1, 0(a0)
		fadd.s	ft0, ft0, ft1
		lui	a0, %hi(3072)
		addi	a0, a0, %lo(3072)
		add	a0, a0, a1
		fsw	ft0, 0(a0)
		j	.LBB0_3
	.LBB0_3:                                # Addr 0x70
		lw	a0, -16(s0)
		addi	a0, a0, 1
		sw	a0, -16(s0)
		j	.LBB0_1
	.LBB0_4:								# Addr 0x80
		lw	a0, -12(s0)
		lw	s0, 8(sp)
		lw	ra, 12(sp)
		addi	sp, sp, 16
		ret
