main:                                   # 0x0
	addi	sp, sp, -32					# imm[11:0]     rs1	000 rd 		 # 11111110000000010000000100010011 	# 0xFE010113
	sw	ra, 28(sp)
	sw	s0, 24(sp)
	addi	s0, sp, 32					# imm[11:0]     rs1	000 rd 		 0010011 # 00000010000000010000010000010011 # 0x2010413
	mv	a0, zero
	sw	a0, -12(s0)
	sh	a0, -14(s0)
	j	.LBB0_1							# imm[20|10:1|11|19:12] rd       1101111 # 00000010000000000000000001101111 # 0x200006F
.LBB0_1:                                # 0x20
	lhu	a0, -14(s0)
	addi	a1, zero, 99				# imm[11:0]     rs1	000 rd 		 0010011 # 00000110001100000000010110010011 # 0x6300593
	blt	a1, a0, .LBB0_16			 	# imm[12|10:5]  rs2 rs1 100 imm[4:1|11] 1100011 # 00010110101001011100001001100011 # 0x16A5C263
	j	.LBB0_2							# imm[20|10:1|11|19:12] rd       1101111 # 00000011000000000000000001101111 # 0x300006F
.LBB0_2:                                # 0x30
	mv	a0, zero
	sh	a0, -16(s0)
	j	.LBB0_3							# imm[20|10:1|11|19:12] rd       1101111 # 00000011110000000000000001101111 # 0x3C0006F
.LBB0_3:                                # 0x3C
	lhu	a0, -16(s0)
	addi	a1, zero, 99				# imm[11:0]     rs1	000 rd 		 0010011 # 00000110001100000000010110010011 # 0x6300593
	blt	a1, a0, .LBB0_14			 	# imm[12|10:5]  rs2 rs1 100 imm[4:1|11] 1100011 # 00010100101001011100100001100011 # 0x14A5C863
	j	.LBB0_4							# imm[20|10:1|11|19:12] rd       1101111 # 00000100110000000000000001101111 # 0x4C0006F
.LBB0_4:                                # 0x4C
	mv	a0, zero
	sh	a0, -22(s0)
	sh	a0, -18(s0)
	j	.LBB0_5							# imm[20|10:1|11|19:12] rd       1101111 # 00000101110000000000000001101111 # 0x5C0006F
.LBB0_5:                                # 0x5C
	lhu	a0, -18(s0)
	addi	a1, zero, 2					# imm[11:0]     rs1	000 rd 		 0010011 # 00000000001000000000010110010011 # 0x200593
	blt	a1, a0, .LBB0_12			 	# imm[12|10:5]  rs2 rs1 100 imm[4:1|11] 1100011 # 00010000101001011100100001100011 # 0x10A5C863
	j	.LBB0_6							# imm[20|10:1|11|19:12] rd       1101111 # 00000110110000000000000001101111 # 0x6C0006F
.LBB0_6:                                # 0x6C
	mv	a0, zero
	sh	a0, -20(s0)
	j	.LBB0_7							# imm[20|10:1|11|19:12] rd       1101111 # 00000111100000000000000001101111 # 0x780006F
.LBB0_7:                                # 0x78
	lhu	a0, -20(s0)
	addi	a1, zero, 2					# imm[11:0]     rs1	000 rd 		 0010011 # 00000000001000000000010110010011 # 0x200593
	blt	a1, a0, .LBB0_10			 	# imm[12|10:5]  rs2 rs1 100 imm[4:1|11] 1100011 # 00001110101001011100111001100011 # 0xEA5CE63
	j	.LBB0_8							# imm[20|10:1|11|19:12] rd       1101111 # 00001000100000000000000001101111 # 0x880006F
.LBB0_8:                                # 0x88
	lui	a0, %hi(1024)					# imm[31:12]			rd		 0110111 # 00000000000000000000010100110111 # 0x537
	addi	a0, a0, %lo(1024)			# imm[11:0]     rs1	000 rd 		 0010011 # 01000000000001010000010100010011 # 0x40050513
	lhu	a1, -14(s0)
	lhu	a2, -18(s0)
	add	a1, a1, a2
	addi	a3, zero, 102				# imm[11:0]     rs1	000 rd 		 0010011 # 00000110011000000000011010010011 # 0x6600693
	mul	a1, a1, a3
	lhu	a3, -16(s0)
	lhu	a4, -20(s0)
	add	a3, a3, a4
	add	a1, a1, a3
	add	a0, a0, a1
	lbu	a0, 0(a0)
	lui	a1, %hi(11428)					# imm[31:12]			rd		 0110111 # 00000000000000000010010110110111 # 0x25B7
	addi	a1, a1, %lo(11428)			# imm[11:0]     rs1	000 rd 		 0010011 # 11001010010001011000010110010011 # 0xCA458593
	addi	a3, zero, 3					# imm[11:0]     rs1	000 rd 		 0010011 # 00000000001100000000011010010011 # 0x300693
	mul	a2, a2, a3
	add	a2, a2, a4
	add	a1, a1, a2
	lbu	a1, 0(a1)
	mul	a0, a0, a1
	lh	a1, -22(s0)
	add	a0, a1, a0
	sh	a0, -22(s0)
	j	.LBB0_9							# imm[20|10:1|11|19:12] rd       1101111 # 00001110110000000000000001101111 # 0xEC0006F
.LBB0_9:                                # 0xEC
	lh	a0, -20(s0)
	addi	a0, a0, 1					# imm[11:0]     rs1	000 rd 		 0010011 # 00000000000101010000010100010011 # 0x150513
	sh	a0, -20(s0)
	j	.LBB0_7							# imm[20|10:1|11|19:12] rd       1101111 # 00001111110000000000000001101111 # 0xFC0006F
.LBB0_10:                               # 0xFC
	j	.LBB0_11						# imm[20|10:1|11|19:12] rd       1101111 # 00010000000000000000000001101111 # 0x1000006F
.LBB0_11:                               # 0x100
	lh	a0, -18(s0)
	addi	a0, a0, 1					# imm[11:0]     rs1	000 rd 		 0010011 # 00000000000101010000010100010011 # 0x150513
	sh	a0, -18(s0)
	j	.LBB0_5							# imm[20|10:1|11|19:12] rd       1101111 # 00010001000000000000000001101111 # 0x1100006F
.LBB0_12:                               # 0x110
	lh	a0, -22(s0)
	lui	a1, %hi(11437)					# imm[31:12]			rd		 0110111 # 00000000000000000010010110110111 # 0x25B7
	addi	a1, a1, %lo(11437)			# imm[11:0]     rs1	000 rd 		 0010011 # 11001010110101011000010110010011 # 0xCAD58593
	lhu	a2, -14(s0)
	addi	a3, zero, 100				# imm[11:0]     rs1	000 rd 		 0010011 # 00000110010000000000011010010011 # 0x06400693
	mul	a2, a2, a3
	lhu	a3, -16(s0)
	add	a2, a2, a3
	slli	a2, a2, 1
	add	a1, a1, a2
	sh	a0, 0(a1)
	j	.LBB0_13						# imm[20|10:1|11|19:12] rd       1101111 # 00010100000000000000000001101111 # 0x1400006F
.LBB0_13:                               # 0x140
	lh	a0, -16(s0)
	addi	a0, a0, 1					# imm[11:0]     rs1	000 rd 		 0010011 # 00000000000101010000010100010011 # 0x00150513
	sh	a0, -16(s0)
	j	.LBB0_3							# imm[20|10:1|11|19:12] rd       1101111 # 00010101000000000000000001101111 # 0x1540006F
.LBB0_14:                               # 0x150
	j	.LBB0_15						# imm[20|10:1|11|19:12] rd       1101111 # 00010101010000000000000001101111 # 0x1540006F
.LBB0_15:                               # 0x154
	lh	a0, -14(s0)
	addi	a0, a0, 1					# imm[11:0]     rs1	000 rd 		 0010011 # 00000000000101010000010100010011 # 0x00150513
	sh	a0, -14(s0)
	j	.LBB0_1							# imm[20|10:1|11|19:12] rd       1101111 # 00010110010000000000000001101111	# 0x1640006F
.LBB0_16:								# 0x164
	lw	a0, -12(s0)
	lw	s0, 24(sp)
	lw	ra, 28(sp)
	addi	sp, sp, 32					# imm[11:0]     rs1	000 rd 		 0010011 # 00010111100000010000000100010011	# 0x17810113
	ret
.Lfunc_end0:							# 0x178

0xfe010113
0x00112e23
0x00812c23
0x02010413
0x00000513
0xfea42a23
0xfea41923
0x0200006F
0xff245503
0x06300593
0x16a5c263
0x0300006F
0x00000513
0xfea41823
0x03c0006f
0xff045503
0x06300593
0x14a5c863
0x04C0006F
0x00000513
0xfea41523
0xfea41723
0x05C0006F
0xfee45503
0x00200593
0x10a5c863
0x06C0006F
0x00000513
0xfea41623
0x00780006
0xfec45503
0x00200593
0x0EA5CE63
0x0880006F
0x00000537
0x40050513
0xff245583
0xfee45603
0x00c585b3
0x06600693
0x02d585b3
0xff045683
0xfec45703
0x00e686b3
0x00d585b3
0x00b50533
0x00054503
0x000025B7
0xCA458593
0x00300693
0x02d60633
0x00e60633
0x00c585b3
0x0005c583
0x02b50533
0xfea41583
0x00a58533
0xfea41523
0x0EC0006F
0xfec41503
0x00150513
0xfea41623
0x0FC0006F
0x1000006F
0xfee41503
0x00150513
0xfea41723
0x1100006F
0xfea41503
0x000025B7
0xCAD58593
0xff245603
0x06400693
0x02d60633
0xff045683
0x00d60633
0x00161613
0x00c585b3
0x00a59023
0x1400006F
0xff041503
0x00150513
0xfea41823
0x1540006F
0x1540006F
0xff241503
0x00150513
0xfea41923
0x1640006F
0xff442503
0x01812403
0x01c12083
0x17810113
0x00008067
