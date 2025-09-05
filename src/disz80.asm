;==============================================================================
; DIS-Z80 DISASSEMBLER
;==============================================================================
; This code was found in a ZX Spectrum tool called UTILITY3 v1.3
; DIS-Z80 was published in the SUBSET column of Personal Computer World 1987.
; The routine disassembles a single Z80 instruction at address DE.
; Originally developed for CP/M on an Amstrad CPC128.
; Just over 1K (1090 bytes) of incredibly compact code by John Kerr.
;==============================================================================

;	SECTION	code_clib
;	PUBLIC	disz80
;	PUBLIC	_disz80
;	EXTERN	fputc_cons

;------------------------------------------------------------------------------
; Main disassembly routine entry points
;------------------------------------------------------------------------------
;.disz80
;._disz80
;	pop	hl
;	pop	bc
;	pop	de
;	push	de
;	push	bc
;	push	hl
;	push	ix				; save caller's ix
;	
;	ld	a, d				; Supplying -1 as address means disassemble following opcode
;	and	e
;	inc	a
;	jr	nz, dizloop
;	ld	hl, 2
;	add	hl, sp
;	ld	e, (hl)
;	inc	hl
;	ld	d, (hl)
;	
;.dizloop
;	push	bc
;	call	DISASM
;	pop	bc
;	ld	a, 13
;	call	CHROP
;	ld	a, 10
;	call	CHROP
;	dec	bc
;	ld	a, b
;	or	c
;	jr	nz, dizloop
;	ld	h, d				; Address of next instruction
;	ld	l, e
;	pop	ix				; restore caller's ix
;	ret

;==============================================================================
; MAIN DISASSEMBLY ROUTINE
;==============================================================================
DISASM:		CALL	ADRSP			; Display address
		LD	BC, $0900		; Initialize buffer counter
		LD	HL, $2020		; Space characters

BUFFER:		PUSH	HL			; Create buffer on stack
		DJNZ	BUFFER
		LD	H, B
		LD	L, C
		ADD	HL, SP

		PUSH	BC
		EX	(SP), IX		; Set up IX pointer
		PUSH	BC
		PUSH	BC
		ADD	IX, SP

		PUSH	HL
		LD	HL, GROUP3		; Point to prefix table

;------------------------------------------------------------------------------
; Check for instruction prefixes (DD, FD, ED, CB)
;------------------------------------------------------------------------------
TRYNDX:		CALL	FETCH			; Get next byte

		LD	B, C
		CP	$ED			; Extended instruction?
		JR	Z, CONFLG

		INC	B
		CP	$DD			; IX prefix?
		JR	Z, CONFLG

		INC	B
		CP	$FD			; IY prefix?
		JR	NZ, NOTNDX

CONFLG:		LD	(IX+1), B		; Store prefix type
		INC	B
		DJNZ	TRYNDX

		JR	NXBYTE

;------------------------------------------------------------------------------
; Handle non-prefix instructions
;------------------------------------------------------------------------------
NOTNDX:		LD	C, A			; Save opcode
		LD	A, (IX+1)		; Check prefix
		OR	A
		JR	Z, NODISP

		LD	A, C
		CP	$CB			; Bit operations?
		JR	Z, GETDIS

		AND	$44
		CP	4
		JR	Z, GETDIS

		LD	A, C
		AND	$C0
		CP	$40
		JR	NZ, NODISP

GETDIS:		CALL	FETCH			; Get displacement byte
		LD	(IX+2), A

NODISP:		LD	HL, GROUP1		; Point to main opcode table
		LD	A, C
		CP	$CB
		JR	NZ, NEWMSK

		LD	HL, GROUP2		; Use CB prefix table

NXBYTE:		CALL	FETCH
		LD	C, A

;------------------------------------------------------------------------------
; Table lookup for instruction decoding
;------------------------------------------------------------------------------
NEWMSK:		LD	A, (HL)			; Get mask
		OR	A
		JR	Z, TABEND

		AND	C			; Apply mask to opcode
		INC	HL

NEWMOD:		LD	B, (HL)			; Get count
		INC	HL
		INC	B
		JR	Z, NEWMSK

TRYMAT:		CP	(HL)			; Compare with table entry
		INC	HL
		JR	Z, GETNDX

		BIT	7, (HL)			; Check end marker
		INC	HL
		JR	Z, TRYMAT

		JR	NEWMOD

GETNDX:		LD	A, (HL)			; Get instruction index
		AND	$7F
		DEC	B

;------------------------------------------------------------------------------
; Extract mnemonic and format output
;------------------------------------------------------------------------------
TABEND:		POP	HL
		PUSH	DE
		PUSH	HL

		EX	DE, HL
		LD	HL, MONICS		; Point to mnemonic table
		CALL	XTRACT			; Extract mnemonic

		POP	HL
		LD	DE, 5			; Move to operand position
		ADD	HL, DE
		POP	DE

		LD	A, B
		AND	$F0			; Check for first operand
		JR	Z, SECOND

		RRA				; Extract operand type
		RRA
		RRA
		RRA
		PUSH	BC

		LD	B, A
		LD	A, C
		CALL	OPRND1			; Process first operand

		POP	BC
		LD	A, B
		AND	$0F
		JR	Z, OPDONE

		LD	(HL), 44		; Add comma separator
		INC	HL

SECOND:		LD	A, B
		AND	$0F			; Process second operand

		LD	B, A
		LD	A, C
		CALL	NZ, OPRND2

;------------------------------------------------------------------------------
; Output formatting and display
;------------------------------------------------------------------------------
OPDONE:		LD	A, 3			; Calculate spacing
		SUB	(IX+0)

		POP	HL
		POP	HL
		POP	IX

		JR	C, OUTEXT

		INC	A			; Add spacing
		LD	B, A
		ADD	A, B
		ADD	A, B
		LD	B, A

SPACES:		LD	A, $20			; Output spaces
		CALL	CHROP
		DJNZ	SPACES
		
OUTEXT:		LD	B, 18			; Output buffer length

PUTOUT:		DEC	SP			; Output character buffer
		POP	HL
		LD	A, H
		CALL	CHROP
		DJNZ	PUTOUT

		RET

;==============================================================================
; INSTRUCTION TABLES
;==============================================================================

;------------------------------------------------------------------------------
; CB prefix instruction table (bit operations)
;------------------------------------------------------------------------------
GROUP2:		defb	$C0, $36, $40
		defb	$04, $80, $2D, $C0, $BE
		defb	$FF, $F8, $06, $00, $33
		defb	$08, $38, $10, $35, $18
		defb	$3A, $20, $3F, $28, $40
		defb	$30, $00, $38, $C1

;------------------------------------------------------------------------------
; Main instruction table
;------------------------------------------------------------------------------
GROUP1:		defb	$FF, $00, $00
		defb	$24, $07, $32, $0F, $37
		defb	$17, $31, $1F, $36, $27
		defb	$0D, $2F, $0B, $37, $3D
		defb	$3F, $06, $76, $14, $C9
		defb	$30, $D9, $12, $F3, $0F
		defb	$FB, $91, $72, $C6, $02
		defb	$CE, $01, $DE, $BC, $02
		defb	$D6, $42, $E6, $03, $EE
		defb	$43, $F6, $25, $FE, $8C
		defb	$04, $08, $93, $01, $10
		defb	$10, $18, $9D, $AF, $22
		defb	$A2, $FA, $2A, $A2, $A7
		defb	$32, $A2, $7A, $3A, $A2
		defb	$03, $C3, $1C, $CD, $85
		defb	$97, $D3, $AA, $79, $DB
		defb	$9B, $5F, $E3, $93, $0E
		defb	$E9, $9C, $05, $EB, $93
		defb	$DF, $F9, $A2, $FF, $C0
		defb	$B6, $40, $A2, $FF, $F8
		defb	$76, $80, $02, $88, $01
		defb	$98, $BC, $06, $90, $42
		defb	$A0, $03, $A8, $43, $B0
		defb	$25, $B8, $8C, $FF, $C7
		defb	$0B, $04, $16, $05, $8E
		defb	$B2, $06, $A2, $20, $C0
		defb	$B0, $23, $C2, $1C, $C4
		defb	$85, $10, $C7, $BB, $FF
		defb	$CF, $D3, $01, $A2, $0D
		defb	$03, $16, $0B, $8E, $FD
		defb	$09, $82, $60, $C1, $2B
		defb	$C5, $AC, $FF, $E7, $21
		defb	$20, $9D, $FF, $EF, $E7
		defb	$02, $A2, $7E, $0A, $A2

;------------------------------------------------------------------------------
; Extended instruction table (ED prefix)
;------------------------------------------------------------------------------
GROUP3:		defb	$FF, $00, $44
		defb	$23, $45, $2F, $4D, $2E
		defb	$4E, $00, $67, $39, $6F
		defb	$34, $70, $00, $71, $00
		defb	$A0, $21, $A1, $0A, $A2
		defb	$1A, $A3, $29, $A8, $1F
		defb	$A9, $08, $AA, $18, $AB
		defb	$28, $B0, $20, $B1, $09
		defb	$B2, $19, $B3, $27, $B8
		defb	$1E, $B9, $07, $BA, $17
		defb	$BB, $A6, $FF, $C7, $B8
		defb	$40, $9B, $8B, $41, $AA
		defb	$FF, $CF, $FD, $42, $3C
		defb	$4A, $81, $AD, $43, $A2
		defb	$DA, $4B, $A2, $FF, $E7
		defb	$40, $46, $95, $FF, $F7
		defb	$C7, $47, $A2, $7C, $57
		defb	$A2, $FF, $00

;==============================================================================
; MNEMONIC TABLE
;==============================================================================
MONICS:		defb	$BF
		defb	'A', 'D', 'C'+$80		; ADC 
		defb	'A', 'D', 'D'+$80		; ADD 
		defb	'A', 'N', 'D'+$80		; AND 
		defb	'B', 'I', 'T'+$80		; BIT 
		defb	'C', 'A', 'L', 'L'+$80		; CALL 
		defb	'C', 'C', 'F'+$80		; CCF
		defb	'C', 'P', 'D', 'R'+$80		; CPDR
		defb	'C', 'P', 'D'+$80		; CPD
		defb	'C', 'P', 'I', 'R'+$80		; CPIR
		defb	'C', 'P', 'I'+$80		; CPI
		defb	'C', 'P', 'L'+$80		; CPL
		defb	'C', 'P'+$80			; CP 
		defb	'D', 'A', 'A'+$80		; DAA
		defb	'D', 'E', 'C'+$80		; DEC 
		defb	'D', 'I'+$80			; DI
		defb	'D', 'J', 'N', 'Z'+$80		; DJNZ 
		defb	'E', 'I'+$80			; EI
		defb	'E', 'X', 'X'+$80		; EXX
		defb	'E', 'X'+$80			; EX 
		defb	'H', 'A', 'L', 'T'+$80		; HALT
		defb	'I', 'M'+$80			; IM 
		defb	'I', 'N', 'C'+$80		; INC 
		defb	'I', 'N', 'D', 'R'+$80		; INDR
		defb	'I', 'N', 'D'+$80		; IND
		defb	'I', 'N', 'I', 'R'+$80		; INIR
		defb	'I', 'N', 'I'+$80		; INI
		defb	'I', 'N'+$80			; IN 
		defb	'J', 'P'+$80			; JP 
		defb	'J', 'R'+$80			; JR 
		defb	'L', 'D', 'D', 'R'+$80		; LDDR
		defb	'L', 'D', 'D'+$80		; LDD
		defb	'L', 'D', 'I', 'R'+$80		; LDIR
		defb	'L', 'D', 'I'+$80		; LDI
		defb	'L', 'D'+$80			; LD 
		defb	'N', 'E', 'G'+$80		; NEG
		defb	'N', 'O', 'P'+$80		; NOP
		defb	'O', 'R'+$80			; OR 
		defb	'O', 'T', 'D', 'R'+$80		; OTDR
		defb	'O', 'T', 'I', 'R'+$80		; OTIR
		defb	'O', 'U', 'T', 'D'+$80		; OUTD
		defb	'O', 'U', 'T', 'I'+$80		; OUTI
		defb	'O', 'U', 'T'+$80		; OUT 
		defb	'P', 'O', 'P'+$80		; POP 
		defb	'P', 'U', 'S', 'H'+$80		; PUSH 
		defb	'R', 'E', 'S'+$80		; RES 
		defb	'R', 'E', 'T', 'I'+$80		; RETI
		defb	'R', 'E', 'T', 'N'+$80		; RETN
		defb	'R', 'E', 'T'+$80		; RET
		defb	'R', 'L', 'A'+$80		; RLA
		defb	'R', 'L', 'C', 'A'+$80		; RLCA
		defb	'R', 'L', 'C'+$80		; RLC 
		defb	'R', 'L', 'D'+$80		; RLD
		defb	'R', 'L'+$80			; RL 
		defb	'R', 'R', 'A'+$80		; RRA
		defb	'R', 'R', 'C', 'A'+$80		; RRCA
		defb	'R', 'R', 'C'+$80		; RRC 
		defb	'R', 'R', 'D'+$80		; RRD
		defb	'R', 'R'+$80			; RR 
		defb	'R', 'S', 'T'+$80		; RST 
		defb	'S', 'B', 'C'+$80		; SBC 
		defb	'S', 'C', 'F'+$80		; SCF
		defb	'S', 'E', 'T'+$80		; SET 
		defb	'S', 'L', 'A'+$80		; SLA 
		defb	'S', 'R', 'A'+$80		; SRA 
		defb	'S', 'R', 'L'+$80		; SRL 
		defb	'S', 'U', 'B'+$80		; SUB 
		defb	'X', 'O', 'R'+$80		; XOR 

;==============================================================================
; OPERAND PROCESSING ROUTINES
;==============================================================================

;------------------------------------------------------------------------------
; Process first operand
;------------------------------------------------------------------------------
OPRND1:		DJNZ	CONDIT

RSTADR:		AND	$38			; RST address calculation
		JR	DA

;------------------------------------------------------------------------------
; Process second operand
;------------------------------------------------------------------------------
OPRND2:		DJNZ	DAT8

RELADR:		CALL	FETCH			; Get relative address
		LD	C, A
		RLA				; Sign extend
		SBC	A, A
		LD	B, A
		EX	DE, HL
		PUSH	HL
		ADD	HL, BC			; Calculate target address
		JR	DHL

;------------------------------------------------------------------------------
; Condition code processing
;------------------------------------------------------------------------------
CONDIT:		RRA
		RRA
		RRA
		DJNZ	BITNUM

		BIT	4, A
		JR	NZ, ABS

		AND	3
	
ABS:		AND	7
		ADD	A, $14
		JR	PS1

;------------------------------------------------------------------------------
; 8-bit data processing
;------------------------------------------------------------------------------
DAT8:		DJNZ	DAT16

D8:		CALL	FETCH
		JR	DA

;------------------------------------------------------------------------------
; Bit number processing
;------------------------------------------------------------------------------
BITNUM:		DJNZ	INTMOD
		AND	7

DA:		LD	C, A
		SUB	A
		JR	DAC

;------------------------------------------------------------------------------
; 16-bit data processing
;------------------------------------------------------------------------------
DAT16:		DJNZ	EXAF
	
D16:		CALL	FETCH			; Get low byte
		LD	C, A
		CALL	FETCH			; Get high byte

DAC:		EX	DE, HL
		PUSH	HL
		LD	H, A
		LD	L, C

DHL:		LD	C, $F8			; Format as hexadecimal
		PUSH	HL
		CALL	CONVHL
		POP	HL
		LD	BC, $000A		; Check if < 10
		OR	A
		SBC	HL, BC
		POP	HL
		EX	DE, HL
		RET	C

		LD	(HL), 'H'		; Add 'H' suffix for hex
		INC	HL
		RET

;------------------------------------------------------------------------------
; Interrupt mode processing
;------------------------------------------------------------------------------
INTMOD:		DJNZ	STKTOP
		AND	3
		ADD	A, $1C
	
PS1:		JR	PS3

;------------------------------------------------------------------------------
; Stack top operations
;------------------------------------------------------------------------------
STKTOP:		LD	C, $13
		DEC	B
		JR	Z, PS2

;------------------------------------------------------------------------------
; 16-bit register pair processing
;------------------------------------------------------------------------------
REG16P:		DJNZ	COMMON
		RRA
		AND	3
		CP	3
		JR	NZ, RX

		DEC	A
		JR	RNX

;------------------------------------------------------------------------------
; Exchange operations
;------------------------------------------------------------------------------
EXAF:		LD	C, $0A
		DEC	B
		JR	Z, PS2

EXDE:		INC	C
		DEC	B
		JR	Z, PS2

;------------------------------------------------------------------------------
; 8-bit register processing
;------------------------------------------------------------------------------
REG8S:		DJNZ	ACCUM

R8:		AND	7
		CP	6			; Check for (HL)
		JR	NZ, PS3

		LD	(HL), '('		; Format indexed addressing
		INC	HL
		CALL	REGX
		LD	A, (IX+2)		; Check displacement
		OR	A
		JR	Z, RP

		LD	(HL), 43		; '+' sign
		RLCA
		RRCA
		JR	NC, POS

		LD	(HL), 45		; '-' sign
		NEG

POS:		INC	HL
		EX	DE, HL
		PUSH	HL
		LD	H, B
		LD	L, A
		LD	C, $FB
		CALL	CONVHL
		POP	HL
		EX	DE, HL
		JR	RP

;------------------------------------------------------------------------------
; Accumulator operations
;------------------------------------------------------------------------------
ACCUM:		RRA
		RRA
		RRA

COMMON:		LD	C, 7
		DEC	B
		JR	Z, PS2

;------------------------------------------------------------------------------
; Port operations
;------------------------------------------------------------------------------
PORTC:		DEC	C
		DJNZ	IDAT8

PS2:		LD	A, C
PS3:		JR	PS4

;------------------------------------------------------------------------------
; Indirect 8-bit data
;------------------------------------------------------------------------------
IDAT8:		DJNZ	IDAT16
		LD	(HL), '('
		INC	HL
		CALL	D8
		JR	RP

;------------------------------------------------------------------------------
; Indirect 16-bit data
;------------------------------------------------------------------------------
IDAT16:		DJNZ	REG8
		LD	(HL), '('
		INC	HL
		CALL	D16
		JR	RP

;------------------------------------------------------------------------------
; Register processing
;------------------------------------------------------------------------------
REG8:		DEC	B
		JR	Z, R8

IPAREF:		DJNZ	REG16
		AND	9
		JR	PS4

REG16:		RRA
		DJNZ	IREG16

R16:		AND	3
RX:		CP	2
		JR	Z, REGX

RNX:		ADD	A, $0C
		JR	PS4

;------------------------------------------------------------------------------
; Indirect register processing
;------------------------------------------------------------------------------
IREG16:		DJNZ	REGX
		LD	(HL), '('
		INC	HL
		CALL	R16

RP:		LD	(HL), ')'		; Close parenthesis
		INC	HL
		RET

;------------------------------------------------------------------------------
; Index register processing
;------------------------------------------------------------------------------
REGX:		LD	A, (IX+1)		; Get index register type
		ADD	A, $10

PS4:		EX	DE, HL
		PUSH	HL
		LD	HL, RGSTRS		; Point to register strings
		CALL	XTRACT
		POP	HL
		EX	DE, HL
		RET

;==============================================================================
; REGISTER NAME TABLE
;==============================================================================
RGSTRS:		defb	'B'+$80
		defb	'C'+$80
		defb	'D'+$80
		defb	'E'+$80
		defb	'H'+$80
		defb	'L'+$80
		defb	'(', 'C', ')'+$80
		defb	'A'+$80
		defb	'I'+$80
		defb	'R'+$80
		defb	'A', 'F', ',', 'A', 'F', "'"+$80
		defb	'D', 'E', ',', 'H', 'L'+$80
		defb	'B', 'C'+$80
		defb	'D', 'E'+$80
		defb	'A', 'F'+$80
		defb	'S', 'P'+$80
		defb	'H', 'L'+$80
		defb	'I', 'X'+$80
		defb	'I', 'Y'+$80
		defb	'(', 'S', 'P', ')'+$80
		defb	'N', 'Z'+$80
		defb	'Z'+$80
		defb	'N', 'C'+$80
		defb	'C'+$80
		defb	'P', 'O'+$80
		defb	'P', 'E'+$80
		defb	'P'+$80
		defb	'M'+$80
		defb	'0'+$80
		defb	'?'+$80
		defb	'1'+$80
		defb	'2'+$80

;==============================================================================
; UTILITY ROUTINES
;==============================================================================

;------------------------------------------------------------------------------
; Convert HL to decimal/hex string
;------------------------------------------------------------------------------
CONVHL:		SUB	A

CVHL1:		PUSH	AF
		SUB	A
		LD	B, 16

CVHL2:		ADD	A, C
		JR	C, CVHL3
		SUB	C

CVHL3:		ADC	HL, HL
		RLA
		DJNZ	CVHL2

		JR	NZ, CVHL1

		CP	10
		INC	B
		JR	NC, CVHL1

CVHL4:		CP	10
		SBC	A, $69
		DAA
		LD	(DE), A
		INC	DE
		POP	AF
		JR	NZ, CVHL4

		RET

;------------------------------------------------------------------------------
; Extract string from table
;------------------------------------------------------------------------------
XTRACT:		OR	A
		JR	Z, COPY

SKIP:		BIT	7, (HL)			; Check end marker
		INC	HL
		JR	Z, SKIP

		DEC	A
		JR	NZ, SKIP

COPY:		LD	A, (HL)			; Copy string
		RLCA
		SRL	A
		LD	(DE), A

		INC	DE
		INC	HL
		JR	NC, COPY

		RET

;------------------------------------------------------------------------------
; Fetch byte and format address
;------------------------------------------------------------------------------
FETCH:		LD	A, (DE)			; Get byte from memory
		INC	DE
		INC	(IX+0)			; Increment instruction length
		PUSH	AF
		CALL	BYTSP			; Display byte
		POP	AF
		RET

ADRSP:		LD	A, D			; Display address
		CALL	BYTOP
		LD	A, E

BYTSP:		CALL	BYTOP			; Display byte with space
		LD	A, $20
		JR	CHROP

BYTOP:		PUSH	AF			; Display byte in hex
		RRA
		RRA
		RRA
		RRA
		CALL	HEXOP
		POP	AF

HEXOP:		AND	$0F			; Convert nibble to hex
		CP	10
		SBC	A, $69
		DAA

;==============================================================================
; CHARACTER OUTPUT ROUTINE
;==============================================================================
CHROP:		jp	seroutfn

;Alternative CHROP implementation (commented out):
;CHROP:		PUSH	HL
;		PUSH	DE
;		PUSH	BC
;
;		ld	c, a
;		call	conout
;		
;		POP	BC
;		POP	DE
;		POP	HL
;		RET