;--------------------------------------------------
; out\ ports
;--------------------------------------------------
; $2x 	Printer DATA
; $3x	Floppy Control	D7 = S/D\, D6 = HD\/S, D5 = Side, D4 = FDCRES, D3 = DSEL4, D2 = DSEL3, D1 = DSEL1, D0 = DSEL0
; $4x	FDC 1797
; $5x	U52 74LS259
; 	$50	ATARI Out data
; 	$51	RS232 TX
; 	$52	ROM switch
; 	$53	printer strobe
; 	$54	reset index-pulse
; 	$55	RS232 DTR
; 	$56	set index pulse
; 	$57	0=CMD,  1=SIO
; $8x	CTC
;--------------------------------------------------
; in\ ports
;--------------------------------------------------
; $2x	Printer Control
; $4x	FDC 1797
; $5x	RS232	D7=RX,  D3=1,  D2=?,  D1=FlowControl
; $70	SIO	D7=RX,  D3=RDY/+5V, D1=CMD
;--------------------------------------------------
;
;	... DATA STRUCTURE FOR DISK I/O CONTROL BLOCK ...
;
DSKOP		equ	0		;DISK OPERATION CODE
DSKDRV		equ	1		;DRIVE# (WITH SIDE# IN BIT 7)
DSKTRK		equ	2		;TRACK#
DSKSEC		equ	3		;SECTOR#
DSKPTR		equ	4		;READ/WRITE POINTER
DSKAUX		equ	6		;AUXILLIARY PARAMETERS (2 BYTES)
DSKSTS		equ	8		;OPERATION COMPLETION STATUS
		
	
TRKREG		equ	FDCTRK
DATREG		equ	FDCDAT
CMDREG		equ	FDCCMD
SECREG		equ	FDCSEC
STSREG		equ	FDCSTAT

INDXSET		equ	SETIDX
INDXCLR		equ	RESIDX

CTC0		equ	TC0
CTC1		equ	TC1
CTC2		equ	TC2
CTC3		equ	TC3
CTCVEC		equ	TC0INTVEC

LATCH		equ	DISKCTRL

;
RIDCMD		equ	11000000B	;READ ID COMMAND
RDCMD		equ	10001000B	;READ COMMAND
WRTCMD		equ	10101000B	;WRITE COMMAND
FINCMD		equ	11010000B	;FORCE INTERRUPT COMMAND
;SKCMD		equ	00011000B	;SEEK COMMAND
SKCMD		equ	00010000B	;SEEK COMMAND
RSTCMD		equ	00000000B	;RESTORE COMMAND
STEPOUT		equ	01100000B	;STEP OUT COMMAND
STEPIN		equ	01000000B	;STEP IN COMMAND
;HLOAD		equ	00001000B	;HEAD LOAD BIT FOR TYPE 1 COMMANDS
HLOAD		equ	00000000B	;HEAD LOAD BIT FOR TYPE 1 COMMANDS

		include "SALLY-CONST.asm"

		org	00000h
		
		jp	start
		
		dw	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
		dw	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
		dw	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
		dw	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
		dw	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
		dw	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
		dw	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
		dw	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
		db	0	
		
		
;--------------------------------------------------
; conin T/C 0 (here counter) interrupt for start-bit
;--------------------------------------------------		
coninInt:	push	af
     		ld	a, 87h
     		out	(TC0), a			;set T/C 0 to 9600 Baud timer 
baud9600:     	ld	a, CONBAUD;01ah			;26 * 16 = 416; 4Mhz/416 = 9600 baud
     		out	(TC0), a
     		ld	a, coninIntB & 255		;set T/C 0 interrupt to next routine
     		ld	(TC0INTVEC), a
     		ld	a, 7fh
coninInt1:	ld	(inbyte + 1), a
     		pop	af
     		ei
     		reti

coninIntB:    	push	af
     		in	a, (SIOIN)			;read RS232-	RX
     		rla
inbyte:    	ld	a, 00h				;<- set by previous routine to 7fh
     		rra
     		jr	c, coninInt1			;loop 7 times
		
coninIntPtr:    ld	(KEYBUF), a			;store result in ff00
     		ld	a, coninIntC & 255		;timer 0 interrupt vector = 0ff2c
		ld	(TC0INTVEC), a
     		pop	af
     		ei
     		reti

coninIntC:     	push	af
     		ld	a, 0c7h				;timer 0 counter
     		out	(TC0), a
     		ld	a, 01h
     		out	(TC0), a
     		ld	a, (coninIntPtr + 1)
     		inc	a
     		and	0fh
     		ld	(coninIntPtr + 1), a		;increment buffer (16 bytes)
     		ld	a, coninInt & 255		;reset T/C 0 interrupt to start-bit routine
     		ld	(TC0INTVEC), a
     		pop	af
     		ei
     		reti


;--------------------------------------------------
; conout T/C 1 interrupt for start-bit
;--------------------------------------------------		
conoutIntA:     push	af
     		xor	a
     		out	(SIOOUT), a
     		ld	a, conoutIntB & 255
     		ld	(TC1INTVEC), a
     		pop	af
     		ei
     		reti

conoutIntB:	push	af
		ld	a, 00h
     		out	(SIOOUT), a
     		rra
     		ld	(conoutIntC + 2), a
     		ld	a, conoutIntC & 255
     		ld	(TC1INTVEC), a
     		pop	af
     		ei
     		reti

conoutIntC:	push	af
     		ld	a, 00h
     		out	(SIOOUT), a
     		rra
     		ld	(conoutIntD + 2), a
     		ld	a, conoutIntD & 255
     		ld	(TC1INTVEC), a
     		pop	af
     		ei
     		reti

conoutIntD:	push	af
     		ld	a, 00h
     		out	(SIOOUT), a
     		rra
     		ld	(conoutIntE + 2), a
     		ld	a, conoutIntE & 255
     		ld	(TC1INTVEC), a
     		pop	af
     		ei
     		reti

conoutIntE:	push	af
     		ld	a, 00h
     		out	(SIOOUT), a
     		rra
     		ld	(conoutIntF + 2), a
     		ld	a, conoutIntF & 255
     		ld	(TC1INTVEC), a
     		pop	af
     		ei
     		reti

conoutIntF:	push	af
     		ld	a, 00h
     		out	(SIOOUT), a
     		rra
     		ld	(conoutIntG + 2), a
     		ld	a, conoutIntG & 255
     		ld	(TC1INTVEC), a
     		pop	af
     		ei
     		reti

conoutIntG:	push	af
     		ld	a, 00h
     		out	(SIOOUT), a
     		rra
     		ld	(conoutIntH + 2), a
     		ld	a, conoutIntH & 255
     		ld	(TC1INTVEC), a
     		pop	af
     		ei
     		reti

conoutIntH:	push	af
     		ld	a, 00h
     		out	(SIOOUT), a
     		rra
     		ld	(conoutIntI + 2), a
     		ld	a, conoutIntI & 255
     		ld	(TC1INTVEC), a
     		pop	af
     		ei
     		reti

conoutIntI:	push	af
     		ld	a, 00h
     		out	(SIOOUT), a
     		ld	a, conoutIntJ & 255
     		ld	(TC1INTVEC), a
     		pop	af
     		ei
     		reti

conoutIntJ:	push	af
     		ld	a, 01h				;stop bit
     		out	(SIOOUT), a
     		ld	a, resetConout & 255
     		ld	(TC1INTVEC), a
     		pop	af
     		ei
     		reti

resetConout: 	push	af				;disable timer 1 interrupt
     		ld	a, 01h
     		out	(TC1), a
     		ld	a, 0ffh
     		ld	(TC1INTVEC), a
     		pop	af
     		ei
     		reti
		
;--------------------------------------------------
; Code executed after Reset
;--------------------------------------------------
start:		di      				; disable interrupt
		xor     a				; set a to zero
restime:	dec     a				; do 256 times nothing
		jr      nz, restime			; loop 

		
		ld      hl, portval			; init 11 ports with values at 0a3h
		ld      b, 0ah              
portinit:	ld      c, (hl)             
		inc     hl                 
		outi   			 	 
		jr      nz, portinit			; loop

		ld      sp, 0c100h			; stack-pointer

		ld	hl, 00000h			; source
		ld	(VARS + addr), hl
		ld	de, 08000h			; dest
		ld	bc, 02000h
		ldir
		
		ld	hl, code8000
		set	7, h
		jp	(hl)
code8000:	ld	a, 1
		out	(ROMSWITCH), a
		
		ld	hl, 08000h
		ld	de, 00000h
		ld	bc, 02000h
		ldir
		jp	code0000
		
code0000:	ld      a, 0ffh				; load interrupt-vector register
		ld      i, a				; with 0ffh
		im      2				; enable interrupt mode 2 (vectored)
		
			
     		call	init9600			;init 9600 baud send/receive
		
		ld	a, 041h
		out	(DISKCTRL), a
		
		ld	b, 5	
stepinloop:	call	stepin
		djnz	stepinloop
	
stepoutloop:	call	stepout
		in	a, (FDCSTAT)
		bit	2, a
		jr	z, stepoutloop

		call	waitmotor
		ld	a, 040h
		out	(DISKCTRL), a

;loop1:
;		xor	a
;		out	(FDCTRK), a			;track 0
;		ld	a, 1
;		out	(FDCSEC), a			;sector 1
;		call	readsec
;		
;loop:		in	a, (FDCSTAT)
;		call	printhex
;	
;		jp	loop1
		

		call	getchar
		
		ld	ix, 10h
		ld	a,0
		ld	(ix+DSKDRV), a
		ld	(TRACK), a

		ld	a, 0
		ld	(ix+DSKTRK), a
		ld	a, 1
		ld	(ix+DSKSEC), a
		ld	a, 0
		ld	(ix+DSKPTR), a
		ld	a, 80h
		ld	(ix+DSKPTR+1), a
		ld	a, 128
		ld	(ix+DSKAUX), a
		ld	a, 0
		ld	(ix+DSKAUX+1), a
		
secloop:	ld	a, '*'
		call	putchar
		call	SETSSO
		ld	a, (IX+DSKSTS)
		call	printhex
		jp	secloop
		
		call	getchar
	

		
		

		jp	printmenu

; S7 Motor On		1=on, 0 = off
; S6 Write Protect
; S5 Rec Type/Spin up	SpinUp: 1=completed, RecType:0=Data Mark, 1=Deleted Data Mark	
; S4 RecNotFound	
; S3 CRC Error
; S2 Lost Data/TRK00	TRK00 = 1
; S1 DRQ/IP
; S0 Busy		
stepin:		call	busy
		ld	a, 041h				;stepin, no update, spinup, 12ms		
stepin1:	out	(FDCCMD), a
		jp	wait

stepout:	call	busy
		ld	a, 061h				;stepin, no update, spinup, 12ms		
		jp	stepin1

readsec:	di
		ld	hl, 0a2edh			;LOAD HL WITH 'INI' OPCODE
		ld	(NMIVEC), hl
		ld	hl, NMIVEC+2
		ld	(hl), 0c9h			;STORE 'RET' OPCODE AFTER INI/OUTI
		ld	c, FDCDAT			;c=disk data port#
		ld	hl, 08000h
		ld	b, 128
		call	busy
		ld	a, 084h				;read sector, spin-up, settletime
		out	(FDCCMD), a
		
readsec1:	halt
		jr	nz, readsec1
		ei

busy:		in	a, (FDCSTAT)
		bit	0, a
		jr	nz, busy
		ret
		
		
waitmotor:	in	a, (FDCSTAT)
		bit	7, a
		jr	nz, waitmotor
		ret
		
wait:		ld	a, 10
wait1:		dec	a
		jr	nz, wait1
		ret
		
;
;	... SECTOR READ/WRITE FUNCTION ...
;
SETSSO:		LD	B,RDCMD
;		BIT	7,(IX+DSKDRV)	;TEST SIDE# BIT IN DRIVE PARAM
;		JR	Z,DISK
;		SET	1,B		;SET 'SSO' CONTROL BIT IN 1797 CMD
DISK:	
		LD	A,B
		LD	(CMDBYT),A	;STORE 1797 COMMAND PASSED IN B
		CALL	SELECT		;SELECT DRIVE/SIDE FOR DISK OPERATION
		BIT	7,A
		JR	Z,DISKX		;EXIT IF NOT READY
		
		LD	A,(TRACK)		
		CP	255
		JR	Z,DISK2
		CP	(IX+DSKTRK)	;TEST IF ALREADY AT DESIRED TRACK
		JR	Z,DISK3		;SKIP SEEK PART IF SO

DISK2:		CALL	SEEKTRK		;GO LOOKING FOR TRACK# IN IOCB
		JR	NZ,DISKX	;EXIT IF HEAD POSITIONING ERROR

DISK3:	
		LD	A,(CTCVEC+2)
		INC	A		;LOOP TILL TX CTC TURNS ITSELF OFF
		JR	NZ,DISK3	;(INTERRUPT VECTOR LSB SET=FFH)
	
		DI
		LD	A,00000011B
		OUT	(CTC0),A	;DISABLE RX INTERRUPT FROM CTC0
		LD	A,00100111B
		OUT	(CTC2),A	;DISABLE CTC2 AND ALSO INIT FOR
		LD	A,61		; CLOCK SOURCE FOR WATCHDOG TIMER
		OUT	(CTC2),A
		LD	HL,WATCHDOG	;STORE NEW CTC3 VECTOR FOR SAFETY
		LD	(CTCVEC+6),HL
	
		LD	A,(RWMAX)
		LD	(RWTRY),A	;SET READ/WRITE RETRY COUNT
DISK4:		LD	A,(IX+DSKSEC)	;OUTPUT SECTOR NUMBER FOR READ/WRITE
		OUT	(SECREG),A
		LD	A,(IX+DSKTRK)
		OUT	(TRKREG),A	;DITTO FOR TRACK NUMBER
		LD	A,(CMDBYT)
		OUT	(CMDREG),A	;START 1791 WORKING ON W/R COMMAND
		CALL	RWDISK		;DO HALT/NMI DISK DATA TRANSFER
		JR	Z,DISKX		;EXIT IF NO DISK ERRORS
				
		PUSH	AF
		CALL	RECOVER		;DO READ/WRITE ERROR RECOVERY ROUTINE
		POP	BC
		JR	NZ,DISK5	;SKIP RETRY IF IRRECOVERABLE ERROR
		LD	HL,RWTRY
		DEC	(HL)
		JR	NZ,DISK4	;ELSE DECREMENT RETRY COUNT TILL=0
DISK5:		LD	A,B		;THEN LOAD A WITH ERROR STATUS

DISKX:		
		LD	(IX+DSKSTS),A	;SAVE COMPLETION STATUS CARRIED IN ACC
		CALL	RENEW		;RESTART ATARI AND RS232 INTERRUPTS
		CALL	ACTIVON		;START DISK ACTIVITY TIMER
		RET
;
ACTIVON:
	DI
	LD	A,10100111B
	OUT	(CTC3),A	;PUT CTC3 IN TIMER MODE, PRESCALE 256
	XOR	A
	OUT	(CTC3),A	;COUNT MOD 256
	DEC	A
	LD	(DRVTMR),A	;STOP DRIVES AFTER 255 INTERRUPTS
	LD	HL,ACTIVTY
	LD	(CTCVEC+6),HL	;SET VECTOR TO ACTIVITY-CHECK ROUTINE
	EI
	RET
;
;
;
ACTIVTY:
	PUSH	AF
	LD	A,(DRVTMR)
	DEC	A
	LD	(DRVTMR),A
	JR	NZ,ACTV2	;EXIT IF 4 SECONDS NOT ELAPSED

	call	shutdown
	LD	A,00100001B
	OUT	(CTC3),A	;RESET INTERRUPT BUT KEEP COUNTER GOING
	LD	(DRVOFF),A	;SET 'DRVOFF' FLAG TO NON-ZERO VALUE

ACTV2:	POP	AF
	EI
	RETI
;
;
;
shutdown:
;	IN	A,(TRKREG)
;	OUT	(DATREG),A
;	LD	A,SKCMD
;	OUT	(CMDREG),A	;DUMMY SEEK TO UNLOAD THE HEADS
	LD	A,(OUTCPY)
	AND	11110000B	;DESELECT ALL 4 DRIVES
	OUT	(LATCH),A
	ret

;
;
;
RWDISK:				;A=1797 TYPE 2 COMMAND BYTE
	LD	HL,(NMIVEC)
	PUSH	HL
	LD	HL,(NMIVEC+2)
	PUSH	HL		;SAVE 4 BYTES AT NMI VECTOR
	LD	HL,0A2EDH	;LOAD HL WITH 'INI' OPCODE
	BIT	5,A		;TEST IF READ OR WRITE BEING DONE
	JR	Z,RW2		;JUMP IF COMMAND IS A READ
	INC	H		;ELSE TRANSFORM 'INI' INTO 'OUTI'
RW2:	LD	(NMIVEC),HL
	LD	HL,NMIVEC+2
	LD	(HL),0C9H	;STORE 'RET' OPCODE AFTER INI/OUTI
	DI
	LD	A,11000111B
	OUT	(CTC3),A	;ARM CTC3 FOR COUNTER MODE INTERRUPT
	XOR	A
	OUT	(CTC3),A	;COUNT 256 PULSES FROM CTC1
	EI
	LD	L,(IX+DSKPTR)	;HL=DISK READ/WRITE DATA POINTER
	LD	H,(IX+DSKPTR+1)
	LD	B,(IX+DSKAUX)	;B=SECTOR LENGTH COUNT (LSB)
	LD	C,DATREG	;C=DISK DATA PORT#
	LD	A,(IX+DSKAUX+1)
	SRL	A
	JR	Z,RW256		;JUMP IF BLOCKSIZE <= 256 BYTES
	SRL	A
	JR	Z,RW512		;JUMP IF BLOCKSIZE <= 512 BYTES
RW1024:	HALT
	JR	NZ,$-1
	HALT
	JR	NZ,$-1
RW512:	HALT
	JR	NZ,$-1
RW256:	HALT
	JR	NZ,$-1
RWBUSY:	IN	A,(STSREG)
	BIT	0,A
	JR	NZ,RWBUSY	;LOOP TILL 1797 BUSY BIT GOES AWAY
	ld	b,a
	call	STOPTMR
	ld	a,b
RWEXIT:	POP	HL
	LD	(NMIVEC+2),HL
	POP	HL
	LD	(NMIVEC),HL	;RESTORE CODE AT NMI
	AND	01111101B	;MASK FOR DISK ERRORS
	RET			;RETURN WITH DISK ERROR FLAGS SET


;
;	... SEEK TRACK# IN (IX+DSKTRK) FROM TRACK# IN (TRACK) ...
;
SEEKTRK:	ld	a, 'S'
		call	putchar
		LD	A,(TRACK)
		CP	255
		JR	NZ,SEEK1	;JUMP IF HEAD POSITION IS KNOWN
	
		CALL	RESTORE		;ELSE DO SLOW RESTORE TO RECALIBRATE
		JR	NZ,SEEKX	;EXIT WITH PERMANENT ERROR IF FAILURE
	
SEEK1:		LD	B,1
		CALL	SEEK		;FIRST SEEK (IX+DSKTRK) WITH NO RETRIES
		RET	Z		;EXIT IF WE GOT TO THE DESIRED TRACK
		JR	NC,SEEK2	;JUMP IF WE LANDED ON SOME OTHER TRACK
	
		CALL	RESTORE		;ELSE RECALIBRATE BEFORE TRYING AGAIN
		JR	NZ,SEEKX	;EXIT IF TRACK ZERO NOT FOUND
	
SEEK2:		LD	B,2
		CALL	SEEK		;NOW SEEK WITH TWO TRIES
		RET	Z		;EXIT IF SUCCESSFUL THIS TIME
		JR	C,SEEKX		;EXIT IF NO ID MARK COULD BE FOUND
	
		LD	HL,100
		CALL	WAIT		;DELAY TO LET THE STEPPER RELAX ITSELF
		LD	HL,RATES
		LD	A,(UNIT)
		ADD	A,L
		LD	L,A		;INDEX INTO TABLE TO DRIVE'S STEP RATE
		INC	(HL)		; AND MAKE STEP RATE ONE NOTCH SLOWER
		LD	A,(HL)
		AND	00000011B	;CHECK FOR ROLL AROUND IN LOWER 2 BITS
		JR	NZ,SEEK2	;REPEAT IF STEP RATE BITS WERE NOT=3
	
		DEC	(HL)		;ELSE RESTORE FROM ROLL AROUND
SEEKX:		LD	A,255
		LD	(TRACK),A	;FLAG TRACK POSITION AS UNKNOWN
		LD	A,00010000B	;SET SEEK ERROR BIT IN STATUS WORD
		OR	A
		RET			;RETURN WITH RNF ERROR STATUS IN A
;
	
RESTORE:
	LD	A,(OUTCPY)
	SET	4,A
	OUT	(LATCH),A	;TWANG THE DISK CONTROLLER RESET PIN
	LD	B,15
	DJNZ	$		;HOLD RESET 50 MICROSECONDS
	RES	4,A
	OUT	(LATCH),A
	LD	B,0
	DJNZ	$
	CALL	FORCE		;THEN FORCE CLEAR THE BEAST

	LD	A,RSTCMD+HLOAD+3
	CALL	TYP1CMD		;DO RESTORE AT SLOWEST STEP RATE
	XOR	00000100B
	AND	10000100B
	RET			;RETURN WITH ACC=0 IF HOME
;
;
;
SEEK:
	PUSH	BC		;SAVE LOOPCOUNT IN B
	LD	A,(TRACK)
	LD	B,A		;LOAD B WITH CURRENT TRACK POSITION
	LD	C,(IX+DSKTRK)	;LOAD C WITH DESTINATION TRACK#
	CALL	STEP		;HAVE A GO AT STEPPING
	CALL	VERIFY		;VERIFY HEAD POSITION WITH READ-ID
	POP	BC
	SCF
	RET	NZ		;EXIT WITH CARRY SET IF READ-ID FAILS
	
	IN	A,(SECREG)
	LD	(TRACK),A	;STORE ACTUAL TRACK# FROM ID MARK
	SUB	(IX+DSKTRK)	;COMPARE IF WE GOT THERE THIS TIME
	RET	Z		;EXIT WITH ACC=0 IF TRACK# VERIFIED

	DJNZ	SEEK		;DO PRESCRIBED NUMBER OF SEEK RETRIES

	OR	A
	RET			;RETURN WITH CARRY AND ZERO FLAGS CLEAR
;

;
;
;	STEP FROM TRACK# IN B TOWARDS TRACK# IN C
;
STEP:
	LD	A,B
	OUT	(TRKREG),A	;STARTING TRACK# TO TRACK REGISTER
	LD	A,C
	OUT	(DATREG),A	;DESTINATION TRACK# TO DATA REGISTER
	LD	HL,RATES
	LD	A,(UNIT)
	ADD	A,L
	LD	L,A		;INDEX INTO STEP RATE TABLE FOR DRIVE
	LD	A,(HL)
	AND	00000011B	;EXTRACT LOWER 2 BITS FOR STEP RATE
	OR	SKCMD+HLOAD	;MERGE WITH SEEK COMAND
	CALL	TYP1CMD		;DO SEEK WITH SPECIFIED STEP RATE
	LD	A,(HL)
	AND	11111100B	;EXTRACT UPPER 6 BITS FOR SETTLE TIME
	LD	H,0
	LD	L,A
	CALL	WAIT		;4..256 MILLISECOND HEAD SETTLING DELAY
	RET

;
;
;
;
VERIFY:
	LD	A,RIDCMD
	CALL	TYP2CMD		;READ NEXT ID MARK TO VERIFY SEEK
	AND	00011000B
	RET	Z		;EXIT IF ID MARK READ OK

	LD	A,(OUTCPY)
	XOR	10000000B	;COMPLIMENT DENSITY BIT OF DRIVE TYPE
	LD	(OUTCPY),A
	OUT	(LATCH),A
	LD	HL,50
	CALL	WAIT		;ALLOW 50 MS DELAY AFTER CLOCK SWITCH
	LD	A,RIDCMD
	CALL	TYP2CMD		;TRY AGAIN IN NEW DENSITY
	AND	00011000B
	RET			;A=0 IF AN ID MARK WAS FOUND


SELECT:
		ld	a,(ix+DSKDRV)
		res	7,A				;CLEAR SIDE SELECT BIT FROM DRIVE#
		cp	4		
		jr	NC,SELX				;EXIT IF INVALID DRIVE NUMBER
			
		ld	B,0		
		ld	C,A				;LOAD BC WITH NEW DRIVE# TO BE SELECTED
		ld	HL,UNIT		
		sub	(HL)				;COMPARE NEW AND OLD SELECT CODES AND
		push	AF				; SAVE RESULT STATUS ON STACK
		ld	D,B		
		ld	E,(HL)				;LOAD DE WITH LAST SELECTED DRIVE#
		ld	(HL),C				;THEN STORE NEW UNIT# FROM C
			
		ld	HL,DRVTAB		
		add	HL,DE				;INDEX INTO TABLE BY CURRENT DRIVE#
		ld	A,(TRACK)		
		ld	(HL),A				;REMEMBER CURENT TRACK NUMBER
		ld	E,4		
		add	HL,DE				;NOW INDEX TO CONTROL BYTE FOR UNIT
		ld	A,(OUTCPY)		
		ld	(HL),A				;REMEMBER CURRENT DENSITY/TYPE
			
		ld	HL,DRVTAB		
		add	HL,BC				;INDEX INTO TABLE BY NEW DRIVE#
		ld	A,(HL)		
		ld	(TRACK),A			;STORE CURRENT (ASSUMED) HEAD POSITION
		add	HL,DE		
		pop	DE				;POP UNIT# COMPARE RESULT INTO D
		ld	A,(HL)				;GET CURRENT SELECT/TYPE BITS FOR DRIVE
		or	A		
		jr	NZ,SEL5				;JUMP IF NOT AN INITIAL DRIVE SELECT
;	
;			ARRIVE HERE ON INITIAL DRIVE SELECT
;	
		ld	HL,SELTAB
		add	HL,BC				;ELSE INDEX INTO TABLE OF SELECT BITS
		ld	A,(HL)		
		out	(DISKCTRL),A			;OUTPUT NEW DRIVE SELECTS
		push	AF		
		call	SPIN				;NOW SPIN UP TO TEST READY
		pop	BC		
		cp	222				;ERROR IF SLOWER THAN 222 MS
		jr	NC,SELX		
			
		set	6,B		
		cp	181				;MINI FLOPPY IF BETWEEN 222 AND 181 MS
		jr	NC,SEL4		
			
		res	6,B		
		cp	153				;BIG FLOPPY IF BETWEEN 181 AND 153 MS
		jr	C,SELX		
			
SEL4:		ld	A,B				;GET CONTROL BITS INTO A
		ld	D,0				;SET D=0 TO DISABLE HEAD LOAD DELAY
;	
;			ARRIVE HERE WITH DENSITY/TYPE/SELECT BITS IN ACC
;		
SEL5:		res	5,A				;RESET SIDE SELECT BIT UNCONDITIONALLY
		bit	7,(IX+DSKDRV)		
		jr	Z,SEL5A				;JUMP IF SELECTING SIDE# ZERO
		set	5,A				;ELSE SET SIDE SELECT BIT IN ACC
		
SEL5A:		;call	printhex
		out	(DISKCTRL),A		
		ld	(OUTCPY),A			;OUTPUT AND SAVE NEW PATTERN
		inc	D		
		dec	D		
		call	NZ,HLDWAIT			;DO HEAD LOAD DELAY IF NEW DRIVE SELECT
;		call	FORCE
		IN	A,(TRKREG)
                OUT	(DATREG),A
                LD	A,SKCMD
                OUT	(CMDREG),A
		ld	a, 8
		dec	a
		jr	nz, ASMPC-1
		IN	A,(STSREG)
		call	printhex
		
		bit	5,A				;TEST 1797 HEAD-LOAD STATUS
		ret	NZ				;EXIT IF LOADED AND MOTORS ON
		
		ld	a, '-'
		call	putchar
		call	SPIN				;ELSE LET THINGS SPIN A BIT
		or	A				;TEST PERIOD AFTER RE-SPINUP
KLUDGE:		jr	Z,SELX				;ERROR IF DISK REFUSES TO TURN
			
		call	FORCE1				;ELSE RETURN TYPE 1 STATUS THIS TIME
		ret
;	
;			ARRIVE HERE IF DRIVE CANNOT BE SELECTED AT ALL
;	
SELX:		xor	A				;TURN OFF EVERYTHING
		out	(DISKCTRL),A
		ld	(OUTCPY),A
		ld	A,10000000B
		or	A				;RETURN WITH NOT-READY ERROR
		ret
;
;
;
RECOVER:
	push	af
	ld	a, 'R'
	call	putchar
	pop	af
	LD	B,A
	AND	11100111B	;MASK OFF ALL BUT RNF/CRC ERROR BITS
	JR	Z,RECOV1	;JUMP IF ONE OF THOSE TWO WERE SET

	PUSH	AF
	CALL	FORCE		;RESET ERROR FLAGS IN 179X STATUS REG
	POP	AF
	AND	11100001B	;CLEAR BITS ASSOCIATED WITH LOST DATA
	RET			;RETURN WITH ACC=0 IF RETRY TO BE DONE
;
RECOV1:	BIT	4,B
	JR	NZ,RECOV3	;JUMP IF RECORD-NOT-FOUND ERROR
;
;	ARRIVE HERE IF BAD CRC ERROR
;
RECOV2:	LD	A,(RWMAX)
	LD	HL,RWTRY
	SUB	(HL)		;COMPUTE HOW MANY RETRYS HAVE BEEN DONE
	RET	Z		;EXIT IF FIRST RETRY

	LD	A,(TRACK)	;ELSE PREPARE TO WIGGLE BACK AND FORTH
	LD	B,A		; TO AN ADJACENT TRACK TO RE-CALIBRATE
	OR	A		; AND REMOVE POSSIBLE MEDIA CONTAMINANT
	JR	NZ,RCOV2A
	LD	C, 1		;STEP TO TRACK#1 IF ON TRACK# 0
	JR	RCOV2B
;
RCOV2A:	DEC	A		;STEP TO NEXT OUTER TRACK
	LD	C,A
RCOV2B:	PUSH	BC
	CALL	STEP		;STEP HEAD TO ADJACENT TRACK
	POP	DE
	LD	B,E		;EXCHANGE CONTENTS OF B AND C
	LD	C,D
	CALL	STEP		;STEP BACK TO ORIGINAL TRACK
	call	VERIFY
	ret			;allow retry successful read id
;
;	ARRIVE HERE IF RECORD-NOT-FOUND ERROR
;
RECOV3:	call	VERIFY		;read an id mark to verify position
	JR	NZ,RCOV4A	;RECALIBRATE IF VERIFY FAILS

	IN	A,(SECREG)	;LOAD A WITH CURENT TRACK# UNDER HEAD
	CP	(IX+DSKTRK)
	JR	NZ,RCOV4B	;JUMP IF NOT ON CORRECT TRACK
;
;	ARRIVE HERE IF ON CORRECT TRACK
;
	ld	a,(RWMAX)
	ld	hl,RWTRY
	sub	(hl)		;test if this is first retry after rnf error
	ret	z

	ld	a,00010000b
	ret			;indicate permanent rnf error
;
;	ARRIVE HERE IF HEAD POSITION IS INCORRECT
;
RCOV4A:	LD	A,255		;SET A TO FORCE RESTORE BEFORE SEEK
RCOV4B:	LD	(TRACK),A
	CALL	SEEKTRK		;SEEK TRACK# SPECIFIED IN IOCB
	RET

;
;
;	EXECUTE TYPE 2 COMMAND (READ/WRITE/READ ID) WITH TIMEOUT
;
TYP2CMD:
	CALL	CMDOUT		;ISSUE COMMAND
	PUSH	BC
	LD	BC,6250		;500,000/80 FOR ONE HALF SECOND DELAY
	JR	TPCMD2
;
;
;	EXECUTE TYPE 1 COMMAND (SEEK/STEP/RESTORE)
;
TYP1CMD:
	CALL	CMDOUT
	PUSH	BC
	LD	BC,37500	;3,000,000/80 FOR 3 SECONDS DELAY
TPCMD2:	IN	A,(STSREG)
	BIT	0,A
	JR	Z,TPCMD3	;EXIT IF BUSY BIT GOES AWAY

	CALL	CMDT1		;DELAY 56 MICROSECONDS
	DEC	BC
	LD	A,B
	OR	C
	JR	NZ,TPCMD2	;LOOP TAKES 80 MICROSECONDS

	LD	A,00010000B
TPCMD3:	LD	B,A
	CALL	FORCE1
	LD	A,B
	POP	BC
	RET

;
;
;
CMDOUT:
		OUT	(FDCCMD),A			;OUTPUT DISK CONTROLLER COMMAND BYTE
CMDT1:		LD	A,14	
CMDT2:		DEC	A
		JR	NZ,CMDT2			;DELAY 56 MICROSECONDS
		RET
;
;
;
;
FORCE:
		LD	A,FINCMD			;LOAD FORCE-INTERRUPT-IMMEDIATE CMD
		CALL	CMDOUT				;CLEAR 179X AND LATCH READY/HLD/TK0 ETC
FORCE1:		IN	A,(FDCSTAT)			;READ STATUS REGISTER CONTENTS
		RET

SPIN:
		XOR	A
		OUT	(INDXSET),A
		INC	A
		OUT	(INDXCLR),A	;SET TO ENABLE NORMAL INDEX PULSES
		IN	A,(TRKREG)
		OUT	(DATREG),A
		LD	A,SKCMD+HLOAD
		OUT	(CMDREG),A	;DO DUMMY SEEK TO START THE MOTORS
		CALL	STARTMR		;THEN RE-PROGRAM CTC1 FOR TIMER
		CALL	FORCE1
		LD	C,A		;SAVE CURRENT TYPE 1 DISK STATUS
		LD	B,6		;SET FOR 6 DISK REVOLUTIONS
		LD	HL,0
		LD	(TICKS),HL	;RESET MILLISECOND COUNTER FOR IRQ
SPIN2:		LD	DE,(TICKS)
		CALL	EDGE		;WAIT FOR INDEX INPUT TO CHANGE
		JR	C,SPIN3		;ABORT IF TIMEOUT
		CALL	EDGE		;WAIT FOR CHANGE BACK AGAIN
		JR	C,SPIN3
		DJNZ	SPIN2		;LET 6 REVOLUTIONS PASS
	
		LD	HL,(TICKS)	;READ TIME AT END OF REVOLUTION
		OR	A
		SBC	HL,DE		;COMPUTE INDEX PERIOD IN MILLISECONDS
SPIN3:		CALL	STOPTMR		;KILL INTERRUPT FROM CTC3
		XOR	A
		OUT	(INDXCLR),A
		INC	A
		OUT	(INDXSET),A	;DISABLE INDEX PULSES AFTER THIS
		LD	A,L
		INC	H
		DEC	H
		JR	Z,SPIN4		;A HOLDS VALID PERIOD IF H=0
		LD	A,255
SPIN4:		LD	(PERIOD),A
		RET
;
;
;
EDGE:
		CALL	FORCE1		;GET 1797 TYPE 1 STATUS
		XOR	C
		AND	00000010B	;CHECK FOR CHANGE IN INDEX BIT
		JR	NZ,EDGE2	;EXIT IF BIT CHANGES
	
		LD	A,(TICKS+1)
		CP	2048/256	;ELSE CHECK TIME ACCUMULATED IN 'TICKS'
		JR	C,EDGE		;KEEP LOOPING TILL 2 SECONDS PASS
	
		SCF
		RET			;THEN RETURN WITH CARRY=1
;	
EDGE2:		LD	A,C
		CPL			;FLIP INDEX STATE HELD IN C
		LD	C,A
		RET			;RETURN WITH CARRY=0

HLDWAIT:	ld	a, 'H'
		call	putchar
		LD	HL,(HLDTIM)	;LOAD HL WITH HEAD LOAD DELAY
		LD	H,0		;RANGE 1..256 MILLISECONDS
	
WAIT:		XOR	A
WAIT2:		DEC	A
		JR	NZ,WAIT2	;DELAY 1 MILLISECOND
		DEC	HL
		LD	A,H
		OR	L
		JR	NZ,WAIT		;LOOP UNTIL HL=0
		RET
	
;
;
;
TMRIRQ:
		PUSH	HL
		EI
		LD	HL,(TICKS)
		INC	HL		;BUMP FREE RUNING MILLISECOND COUNTER
		LD	(TICKS),HL
		POP	HL
		RETI

;
;	... MILLISECOND TIMER INTERRUPT ROUTINES ...
;
;
STARTMR:
		DI
		LD	A,10000111B
		OUT	(CTC3),A
		LD	A,250
		OUT	(CTC3),A
		LD	HL,TMRIRQ
		LD	(TC3INTVEC),HL
		EI
		RET
;
;
;
STOPTMR:
		DI
		LD	A,00000001B
		OUT	(CTC3),A
		EI
		RET
;
;
;	... CTC INTERRUPT CONTROL ROUTINES FOR DISK HANDLER ...
;
;
;
WATCHDOG:
	LD	A,FINCMD
	OUT	(CMDREG),A	;ABORT DISK CONTROLLER OPERATION
	LD	A,00000001B
	OUT	(CTC3),A	;RESET INTERRUPT FROM CTC3
	LD	HL,RWEXIT
	EX	(SP),HL		;TOSS RETURN ADDRESS AND PLANT FAKE ONE
	LD	A,00010000B
	OR	A		;INDICATE RECORD-NOT-FOUND ERROR
	EI
	RETI			;INSURE CTC IRQ LOGIC GETS RESTORED


SELTAB:		db	00000001B
		db	00000010B
		db	00000100B
		db	00001000B
;	
;	... GLOBAL VARIABLES FOR PHYSICAL DISK HANDLER ...
;
DRVTAB:		db	255,255,255,255	;HEAD POSITIONS FOR 4 DRIVES
		db	0,0,0,0		;DENSITY/TYPE/SELECT BITS FOR 4 DRIVES
RATES:		db	16,16,16,16	;SETTLING DELAYS / STEP RATES TABLE
UNIT:		db	0		;CURRENTLY SELECTED DISK#
TRACK:		db	255		;TRACK POSITION OF SELECTED DRIVE
DRVOFF:		db	1		;DRIVES-OFF FLAG FROM DISK TIMER IRQ
OUTCPY:		db	00000000B	;COPY OF DISK CONTROL LATCH
PERIOD:		db	0		;DISK SPIN PERIOD
HLDTIM:		db	50		;HEAD LOAD DELAY
RWMAX:		db	10		;MAX NUMBER OF READ/WRITE RETRIES
		db	0,0,0,0		;ROOM FOR EXPANSION

TICKS:		dw	0
CMDBYT:		db	0
RWTRY:		db	0
DRVTMR:		db	0
	
putchar:	
    		push	bc
     		push	af
     		ld	c, a
     		call	conout
     		pop	af
     		pop	bc
     		ret
		
printstack:    	ex	(sp), hl			;exchange (load) hl with stackpointer
printstack1:	ld	a, (hl)				;load char
     		call	putchar				;print it
     		ld	a, (hl)				;load it again
     		inc	hl				;increment hl
     		or	a				;char <> 0?
     		jr	nz, printstack1			;loop
     		ex	(sp), hl			;write hl back to stack
     		ret
		


getchar:     	push	hl
     		push	bc
     		call	conin				;CONIN in jumptable
     		pop	bc
		pop	hl
;     		res	7, a				;reset parity bit
     		cp	20h				;char < 20h = control char?
     		ret					;yes, return

		
;--------------------------------------------------
; SALLY Monitor
;--------------------------------------------------		
init9600:	di
     		ld	hl, resetConout | 255		;resetConout			;point Timer 1 interrupt (send)
     		ld	(TC1INTVEC), hl			;to 0f5eeh
     		ld	a, 07h				;Timer 1 reset, no int
     		out	(TC1), a			;load time constant
     		ld	a, (baud9600 + 1)		;01ah = 9600 Baud constant from conin
     		out	(TC1), a	
     		ld	hl, KEYBUF			;16-byte input buffer	
     		ld	(coninIntPtr+1), hl	
     		ld	(coninPtr), hl	
RENEW:     	di	
     		ld	a, 01h	
     		out	(CMDSIO), a			;enable SIO-Trig
init9600a:	ld	b, 7eh	
init9600c:	in	a, (SIOIN)			;read SIO
     		rla					;D7 (RX) to carry
     		jr	nc, init9600a			;0? repeat
     		djnz	init9600c			;high for 126 loops?
     		ld	a, 0c7h				;T/C 0 interrupt on, counter mode, falling edge (start bit)
     		out	(TC0), a	
     		ld	a, 01h				;T/C 0, count just 1 
     		out	(TC0), a	
     		ld	hl, coninInt			;set T/C 0 int to 0f500h
     		ld	(TC0INTVEC), hl
     		ei
     		ret


coninPtr:	dw	0h
		
;--------------------------------------------------
; const, is input from SIO ready?
;--------------------------------------------------		
;f635:
const:     	ld	hl, coninPtr
     		ld	a, (coninIntPtr + 1)
     		cp	(hl)
     		ret	z

     		ld	a, 0ffh
     		ret

;--------------------------------------------------
; conin
;--------------------------------------------------		
conin:		call	const				;0f635h
     		jr	z, conin			;input available?
     		ld	hl, (coninPtr)			;read from buffer
     		ld	a, (hl)				
     		inc	l				;increment buffer pointer
     		res	4, l				;wrap at 16 bytes
     		ld	(coninPtr), hl
     		ret

;f650:
conout:		ld	a, (TC1INTVEC)
     		inc	a				;resetConout & 255
     		jr	nz, conout			; (-07h)		
     		ld	a, c
;     		and	7fh
;    		jp	po, conout1			;set parity
;   		or	80h
conout1:     	ld	(conoutIntB + 2), a		;set byte to output
     		ld	a, conoutIntA & 255		;set conout interrupt
     		ld	(TC1INTVEC), a
     		ld	a, 81h				;enable timer 1 int
     		out	(TC1), a		
     		ret


;--------------------------------------------------
; 11 times port:value
;--------------------------------------------------
portval:	db	050h, 001h			;Bit0	set ATARI DATA
		db	051h, 001h			;Bit1	set RS232 TX		051h fake!
		db	080h, 003h			;CTC	Channel 0 reset
		db 	080h, 010h			;CTC	Channel 0 interrupt vector
		db	081h, 007h			;CTC	Channel 1 reset + set time constant
		db	081h, 001h			;CTC	Channel 1 time contant
		db	082h, 003h			;CTC	Channel 2 reset
		db	083h, 003h			;CTC	Channel 3 reset
		db	057h, 001h			;Bit7	CMDSIO = SIO
		db	030h, 040h			;DSE	Floppy Control (74LS273)
		db	040h, 0d0h			;DWR/DRW	FDC read-write	d0 = force int (with no interrupt)
		
		include "sallytest.asm"