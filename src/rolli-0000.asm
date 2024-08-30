
RIDCMD		equ	11000000B	;READ ID COMMAND
RDCMD		equ	10000000B	;READ COMMAND
WRTCMD		equ	10100000B	;WRITE COMMAND
FINCMD		equ	11010000B	;FORCE INTERRUPT COMMAND
SKCMD		equ	00010000B	;SEEK COMMAND
RSTCMD		equ	00000000B	;RESTORE COMMAND
STEPOUT		equ	01100000B	;STEP OUT COMMAND
STEPIN		equ	01000000B	;STEP IN COMMAND
HLOAD		equ	00000000B	;HEAD LOAD BIT FOR TYPE 1 COMMANDS

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

putaddr		equ	0f493h
puthex		equ	0f498h
coninit		equ	0f5fch
conin		equ	0f640h
conout		equ	0f650h
SELECT		equ	0f110h
RESTORE		equ	0f23bh
SEEKTRK		equ	0f1fch
RWDISK		equ	0f2b6h
RENEW		equ	0f018h
WATCHDOG	equ	0f3a8h
RECOVER		equ	0f30fh
ACTIVON		equ	0f03ch

dskhandler	equ	0f00fh
CFRAME		equ	IOBUFF+LEN-5
iobuflenhi	equ	(IOBUFF+LEN) / 256


		include "equs.mac"
		include "global.mac"

;--------------------------------------------------
; Code executed after Reset
;--------------------------------------------------
		ORG	00000h
		jp	reset
		jp	serin
		jp	serout
		jp	serhex
		jp	sercmd
		jp	serdump
		jp	serrecv
;
reset:		di      				;disable interrupt
		xor     a				;set a to zero
restime:	dec     a				;do 256 times nothing
		jr      nz, restime			;loop 
							 
		ld      hl, portval			;init 11 ports with values at 0a3h
		ld      b, 0bh              	
portinit:	ld      c, (hl)             	
		inc     hl                 	
		outi   			 		 
		jr      nz, portinit			; loop
		
		
		ld      hl,0f000h          		;test ram @F000-FFFF
		ld      a, 01h             		;write 1,2,4,8,16,32,64,128 
testram2:	ld      b, 10h			
testram:	ld      (hl),a
		rlca    
		inc     l
		jr      nz, testram
		inc     h
		djnz    testram
		
		ld      c, 10h
testram1:	dec     hl
		rrca    
		cp      (hl)
ramerr:		jr      nz, ramerr			;if error, loop forever
		djnz    testram1                         
		dec     c                                
		jr      nz, testram1                     
		add     a, a                             
		jr      nz, testram2		         
							 
		ld      hl, sallycode			;copy BIOS 0f000h
		ld      de, 0f000h			;length always 0f00h
		ld      bc, 00f00h        		  
		ldir                    		  
		ld      hl, sallycode+0efch		;copy initial values to 0ff20h
		ld      de, 0ff20h			;length $2F
		ld      bc, 002fh        		  
		ldir                    		  
							 
		xor     a				;fill up to $FFFF with zeros
ramfill:	ld      (de),a             
		inc     e                  
		jr      nz, ramfill           
					
		ld      sp, 0ff10h			;stack-pointer to 0ff10h
							 
		ld      a, 0ffh				;load interrupt-vector register
		ld      i, a				;with 0ffh
		im      2				;enable interrupt mode 2 (vectored)	
							 
;--------------------------------------------------      
; step 5 times in and then out to trk00                  
; set bit 6 for each online floppy in ff5eh              
; percom block (16 bytes, byte 8 bit 6)                  
;--------------------------------------------------      
		ld      a, 4fh				;select drive 1-4, Motor off, side 0, B/S=1, DD
		out     (LATCH),a         		  
							 
		ld      d, a				;d = 4fh
		ld      b, 5				;step 5 times
stepin:		                                         
		ld      a, 49h				;4b = 0100 1011 = step-in 
		call    CMDOUT				;write A to FDC command and wait
		djnz    stepin          		   
							 
		ld      b, 64h				;seek track 00 for all 4 drives
outloop1:	ld      a, d				;select all drives
		out     (LATCH),a         		  
		ex      (sp),hl				;???
		ex      (sp),hl				;???
		ld      a, 69h				;step out
		call    CMDOUT				;write A to FDC command and wait
							 
		ld      e, 01h          		   
outloop:	ld      a, e            		   
		or      40h     		         
		out     (LATCH), a			;select one drive
		ex      (sp), hl        		   
		ex      (sp), hl        		   
;		call    0f391h				;stop command, get status
		in	a, (STSREG)
		bit     2, a            		   
		jr      z, excldrv			;bit not set, not at track 00
		ld      a, e				;drive at track 00
		cpl     				;exclude drive from seeking
		and     d               		  
		ld      d, a            		   
excldrv:		                                 
		sla     e               		  
		bit     4, e				 
		jr      z, outloop			;status checked for all 4 drives?
		djnz    outloop1			;step out again
							
;		call    0f068h				;deselect floppies and seek current track?
		ld	a, 50h				;reset FDC and deselct
		out	(LATCH), a
		ld	a, 40h				;reset FDC and deselct
		out	(LATCH), a
		
		ld      hl, 0ff5eh			;set bit for each connected floppy?
		ld      bc, 0010h       	         
		ld      a, 04h          	         
nextdrv:	rr      d               	         
		jr      c, noset        	         
		set     6, (hl)         	         
noset:		add     hl, bc          	         
		dec     a               	         
		jr      nz, nextdrv     	         


		ld	hl, 00000h			; source
		ld	de, 08000h			; dest
		ld	bc, 02000h
		ldir		
		ld	hl, code8000
		set	7, h
		jp	(hl)
code8000:	ld	a, 1
		out	(BANKSW), a
		ld	hl, 08000h
		ld	de, 00000h
		ld	bc, 02000h
		ldir
		jp	code0000

code0000:					 
		ld      sp, 0c100h			;set stack to 0c100h
		call	sercr
		call	sercr
		jp      0f762h				;jump to code in DRAM
;		jp	0f003h
;		jp	main

CMDOUT:
		OUT	(CMDREG),A			;OUTPUT DISK CONTROLLER COMMAND BYTE
CMDT1:		LD	A,14	
CMDT2:		DEC	A
		JR	NZ,CMDT2			;DELAY 56 MICROSECONDS
CMDT3:		in	a, (STSREG)
		bit	0, a
		jr	nz, CMDT3
		RET

;--------------------------------------------------
; 11 times port:value
;--------------------------------------------------
portval:	db	050h, 001h			;Bit0	set ATARI DATA
		db	051h, 001h			;Bit1	set RS232 TX
		db	080h, 003h			;CTC	Channel 0 reset
		db 	080h, 010h			;CTC	Channel 0 interrupt vector
		db	081h, 007h			;CTC	Channel 1 reset + set time constant
		db	081h, 001h			;CTC	Channel 1 time contant
		db	082h, 003h			;CTC	Channel 2 reset
		db	083h, 003h			;CTC	Channel 3 reset
		db	057h, 001h			;Bit7	ATARI RXD
		db	030h, 050h			;DRIVE CONTROL reset FDC
		db	030h, 040h			;DRIVE CONTROL 8Mhz
	
main:
		;call	0f5fch				;coninit

serloop1:	ld	hl, IOBUFF+LEN-5
serloop:	call	serrecv
		jr	z, serloop
		call	sercmd
		call	0fc54h

		ld	hl, IOBUFF
		call	serrecv
		call	serdump
		
		jp	main
;
;
; 19200 = 208 cycles
;
serrecv:	
;	LD	DE,2730		;SET ABORT COUNTER FOR 32 MILLISECONDS
	LD	BC,0		;CLEAR B/C FOR CHECKSUM DERRIVATION
	JR	RXB35		;GO START LOOPING FOR START BIT
;
RXB1:	LD	A,C		;4
	ADD	A,B		;4
	ADC	A,0		;7 ACCUMULATE CHECKSUM ATARI STYLE
	LD	C,A		;4
	EX	(SP),HL		;19
	EX	(SP),HL		;19
	EX	(SP),HL		;19
	EX	(SP),HL		;19
	LD	B,8		;7	=102
;
;	SERIAL->PARALLEL CONVERSION AT 52 MICROSECONDS PER BIT
;
RXB2:	LD	A,11		;  7 CYCLES
	LD	A,11		;  7 CYCLES
	NOP			;  4 CYCLES
RXB3:	DEC	A		; 44 CYCLES  (11*4)
	JP	NZ,RXB3		;110 CYCLES  (11*10)
	IN	A,(ATARI)	; 11 CYCLES
	RLA			;  4 CYCLES
	RR	D		;  8 CYCLES
	DJNZ	RXB2		; 13 CYCLES  (8 ON FINAL BIT)

	LD	B,D		;4 SAVE COPY OF LAST DATA BYTE IN B
	LD	(HL),D		;7 THEN STORE IN MEMORY BUFFER @HL
	INC	HL		;6
	LD	A,H		;4
	CP	iobuflenhi	;7
	CCF			;4
	RET	C		;5 RETURN WITH CARRY SET IF BUFFER FILLED

	ld	a, 6
rxb3a:	dec	a
	jp	nz, rxb3a
	
RXB35:	;LD	A,01000111B
	;OUT	(CTC0),A	;PUT CTC0 IN COUNTER MODE
	;XOR	A
	;OUT	(CTC0),A	;COUNT DATA HIGH->LOW EDGES MOD 256

	LD	DE,417		;10 5 MILLISECONDS @ 12 MICROSECONDS/LOOP
RXB4:	IN	A,(ATARI)
	RLA
	JR	NC,RXB1		;NEW BYTE IS COMING IF START BIT LOW
	DEC	DE
	LD	A,D
	OR	E
	JR	NZ,RXB4		;ELSE LOOP TILL TIMER RUNS OUT

	RET			;RETURN WITH CARRY CLEAR IF TIMED OUT
;--------------------------------------------------
; RS232 dump
;--------------------------------------------------
serdump:	ld	de, 0c100h
		ex	de, hl
		call	sercr
		call	sercmd
		call	sercr
serdump2:	ld	b, 16
serdump1:	ld	a, (hl)
		call	serhex
		call	serspace
		inc	hl
		dec	de
		ld	a, d
		or	a, e
		jr	z, serdump3
		djnz	serdump1
		call	sercr
		jr	serdump2
serdump3:	call	sercr
		ld	a, (hl)
		call	serhex
		call	sercr
		ret
		

;--------------------------------------------------
; RS232 sercmd
;--------------------------------------------------
sercmd:		push	af
		push	bc
		push	hl
		ld	hl, CFRAME
		ld	b, 4
sercmd1:	ld	a, (hl)
		call	serhex
		call	serspace
		inc	hl
		djnz	sercmd1
		call	sercr
		pop	hl
		pop	bc
		pop	af
		ret
		
;--------------------------------------------------
; RS232 <space>
;--------------------------------------------------
serspace:	push	af
		ld	a, ' '
		call	serout
		pop	af
		ret

;--------------------------------------------------
; RS232 <CR>
;--------------------------------------------------
sercr:		push	af
		ld	a, '\r'
		call	serout
		pop	af
		ret
		
;--------------------------------------------------
; RS232 output byte in hex
;--------------------------------------------------
serhex:		push	af
		push	af
		rrca
		rrca
		rrca
		rrca
		call	sernib
		pop	af
		call	sernib
		pop	af
		ret
		
sernib:		and	0fh
		add	'0'
		cp	'9' + 1
		jr	c, sernib1
		add	7
sernib1:			
;--------------------------------------------------
; RS232 out	208 T-States
;--------------------------------------------------
serout:		push	af
		push	bc
		ld	b, a
		xor	a
		out	(SEROUT), a			;startbit
		call	time19600			;17

		ld	a, b
		ld	b, 8				;7
serout1:	out	(SEROUT), a			;11
		call	time19600			;17
		rrca					;4
		djnz	serout1				;8

		ld	a, 1				;7
		out	(SEROUT), a			;11
		call	time19600			;17
		
		pop	bc
		pop	af
		ret
		
		
;--------------------------------------------------
; RS232 in	208 T-States
;--------------------------------------------------
serin:		push	bc
serin2:		in	a, (SERIN)
		rlca
		jr	c, serin2
		
		ex	(sp), hl			;19, 4.75uS
		ex	(sp), hl			;19  9uS
		
		ld	b, 80h
serin1:		call	time19600
		in	a, (SERIN)
		rlca
		rr	b
		jr	nc, serin1

		ld	a, b
		pop	bc
		ret

time19600:	ld	c, 9				;4
time19600a:	dec	c				;4
		jr	nz, time19600a			;12/7
		ret					;10

;		DI
;		LD	A,10000111B
;		OUT	(CTC3),A
;		LD	A,250
;		OUT	(CTC3),A
;		LD	HL,TMRIRQ
;		LD	(CTCVEC+6),HL
;		EI
	
dskloop:
;		ld	hl, (TICKS)
;		call	putaddr
;		jp	dskloop
		
		ld	ix, dcb
		call	dskhandler
		call	dump
		
		call	conin
		
		jp	dskloop
		
		
dump:		ld	hl, dcb
		ld	b, 9
loop:		ld	a, (hl)
		call	puthex
		inc	hl
		djnz	loop
		ret
		
;		ld	ix, dcb
;		call	dskhandler
;		ld	hl, dcb
;		ld	b, 9
;loop1:		ld	a, (hl)
;		call	puthex
;		inc	hl
;		djnz	loop1
;		
;		jp	dskloop

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
		


		
dcb:		db	0				;DISK OPERATION CODE
		db	0				;DRIVE# (WITH SIDE# IN BIT 7)
		db	0				;TRACK#
		db	1				;SECTOR#
		dw	8000h				;READ/WRITE POINTER
		dw	128				;AUXILLIARY PARAMETERS (2 BYTES)
		db	0				;OPERATION COMPLETION STATUS
		
sallycode	equ	ASMPC
