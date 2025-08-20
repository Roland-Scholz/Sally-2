;
; VR177x debug 7 test/code
;
		LD	HL, NMIVEC
;		LD	(HL),0C9H	;STORE 'RET' OPCODE AFTER INI/OUTI
		ld	(hl), 0c3h	;store 'JP'
		inc	hl
		ld	(hl), LOW nmi
		inc	hl
		ld	(hl), HIGH nmi

		LD	A, 041h + 000h + 000h
		OUT	(LATCH), A
		
		ld	e, 0
		CALL	sercr
		
loop2:		LD	d, 1
loop:		LD	a, e
		CALL	serhex
		ld	a, d
		CALL	serhex
		
		CALL	serinfn
		
		ld	hl, 01000h
		ld	c, DATREG
		ld	b, 0
		LD	a, e
		OUT	(TRKREG), A
		LD	A, d
		OUT	(SECREG), A
		LD	A, RDCMD
		out	(CMDREG), A
;
; 297 T-States
;
;		ld	a, 100
;loop4:		dec	a					;4
;		jr	nz, loop4				;12
		
loop3:		halt
		jr	nz, loop3
		
loop1:		IN	A,(STSREG)
		BIT	0, A
		JR	NZ,loop1				;LOOP TILL 1797 BUSY BIT GOES AWAY

		CALL	serhex
		CALL	sercr		

		and	07fh
		jr	z, loop1a
	
		LD	A, 041h + 000h + 000h
		OUT	(LATCH), A
		
loop1a:		ld	hl, 01000h
		call	dumpsec
		
;		LD	A, 040h
;		OUT	(LATCH), A

		INC	d
		LD	a, d
		cp	19
		jr	nz, loop
		
		LD	A, STEPIN+NOWAITMTR+STEPRATE
		CALL	SalyCMDOUT
		INC	e
		JR	loop2
		
nmi:		ini
		ret

;--------------------------------------------------
; test code
;--------------------------------------------------
;		LD	A, 0c3h					;'JP' instruction
;		LD	(SEL4), A
;		LD	HL, SalySEL4
;		LD	(SEL4+1), HL
;
;		LD	A, 255
;		LD	(CTCVEC+2), A
;
;		LD	B, 3
;		LD	A, ' '
;stars:		CALL	seroutfn
;		DJNZ	stars
;
;		LD	IX, testdcb
;		CALL	sercr
;		CALL	DISKV
;		CALL	dumpdcb
;
;		LD	A, GETID
;		LD	(testdcb), A
;		CALL	DISKV
;		CALL	dumpdcb
;		CALL	dumprid
;
;		LD	HL, 512
;		LD	(testdcb+DSKAUX), HL
;		LD	A, GETSEC
;		LD	(testdcb), A
;
;		LD	A, '*'
;		CALL	seroutfn
;
;		LD	C, 1
;secloop1:	LD	B, 18
;		LD	HL, sectab
;secloop:	LD	A, (HL)
;		LD	(testdcb + DSKSEC), A
;		INC	HL
;		PUSH	BC
;		PUSH	HL
;		CALL	DISKV
;		POP	HL
;		POP	BC
;		DJNZ	secloop
;		DEC	C
;		JR	NZ, secloop1
;
;		LD	A, '-'
;		CALL	seroutfn
;
;loop:		JR	loop
;
;

;
;dumprid:
;		LD	HL, (testdcb + DSKPTR)
;		CALL	sercr
;		LD	B, 6
;dumprid1:	LD	A, (HL)
;		INC	HL
;		CALL	serhex
;		CALL	serspace
;		DJNZ	dumprid1
;		RET
;
;dummy:		XOR	A
;		CALL	serhex
;		RET
;
;testdcb:	DB	TSTRDY					;DISK OPERATION CODE
;		DB	1					;DRIVE# (WITH SIDE# IN BIT 7)
;		DB	0					;TRACK#
;		DB	1					;SECTOR#
;		DW	08000h					;READ/WRITE POINTER
;		DW	0					;AUXILLIARY PARAMETERS (2 BYTES)
;		DB	0					;OPERATION COMPLETION STATUS
;
;sectab:
;		DB	1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18
;		DB	1, 3, 5, 7, 9, 11, 13, 15, 17, 2, 4, 6, 8, 10, 12, 14, 16, 18
;		DB	1, 4, 7, 10, 13, 16, 2, 5, 8, 11, 14, 17, 3, 6, 9, 12, 15, 18
;		DB	1, 5, 9, 13, 17, 3, 7, 11, 15, 2, 6, 10, 14, 18, 4, 8, 12, 16
;--------------------------------------------------
; end of test code
;--------------------------------------------------
