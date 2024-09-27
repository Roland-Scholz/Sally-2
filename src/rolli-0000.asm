
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

DISKID		equ	0f82bh
DISKTAB		equ	0f83dh
DRVINDEX	equ	0fc28h
SENDBUFF	equ	0f66ch
SENDACK		equ	0fc4ah
HASPARMS	equ	0fbd3h
DISKREAD	equ	0f98ch
XMITBUF		equ	0f684h
RXBLOCK		equ	0f6d9h
CMDWAIT		equ	0f7e2h

TRKBUF		equ	00800h
TRK14BUF	equ	TRKBUF + 18 * 256

DEBUG		equ	0f9d4h

SIONORMAL	equ	40
SIOFAST		equ	8

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
		jp	fastrecv
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
		
;		call	sercr
;		call	sercr
;		jp      0f762h				;jump to code in DRAM
;		jp	0f003h
		jp	main

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
;--------------------------------------------------
; firmware patch
;--------------------------------------------------
		ld	hl, 0ffffh
		ld	(track), hl

		ld	hl, DISKTAB+2
		ld	de, disktab+3
		ld	bc, 3*7
		ldir

		ld	hl, 8
		ld	(disktab),hl
		ld	a, '?'
		ld	(disktab+2), a
		ex	de, hl
		
		ld	a, getspeed & 255
		ld	(hl), a
		inc	hl
		ld	a, getspeed / 256
		ld	(hl), a
		
		ld	hl, disktab
		ld	(DISKID), hl
		ld	(DISKID+2), hl
		ld	(DISKID+4), hl
		ld	(DISKID+6), hl
		
		ld	a, 0c3h				;'JP' instruction
		ld	(XMITBUF), a
		ld	(RXBLOCK), a
		ld	(CMDWAIT), a
		ld	(DEBUG), a
		
		ld	hl, xmitbuf
		ld	(XMITBUF+1), hl
		ld	hl, rxblock
		ld	(RXBLOCK+1), hl
		ld	hl, cmdwait
		ld	(CMDWAIT+1), hl
		ld	hl, debug
		ld	(DEBUG+1), hl
		
		ld	a, SIONORMAL
		ld	(pokeydiv), a		

		
		call	sercr
		call	sercr

test:		jp	0f762h

;
;
;
;--------------------------------------------------
; debug
;--------------------------------------------------
debug:	
;		ld	a, 'r'
;		call	serout
;		ld	a, 't'
;		call	serout
;		ld	a, (ix + DSKTRK)
;		call	serhex
;		ld	a, 's'
;		call	serout
;		ld	a, (ix + DSKSEC)
;		call	serhex
;		ld	a, 'l'
;		call	serout
;		ld	a, (ix + DSKAUX+1)
;		call	serhex
;		ld	a, (ix + DSKAUX)
;		call	serhex
;		ld	a, 'a'
;		call	serout		
;		ld	l, (ix + DSKPTR)
;		ld	h, (ix + DSKPTR+1)
;		call	seraddr

		
		ld	a, (ix + DSKTRK)
		cp	014h				;track 20 (directory)
		jr	nz, readtrack3

		ld	hl, TRK14BUF
		ld	(bufaddr), hl
		ld	a, (track14)			;track14 read?
		inc	a	
		jr	nz, match			;yes
		jr	readtrack4
		
readtrack3:	ld	hl, TRKBUF
		ld	(bufaddr), hl

		ld	a, (track)
		cp	(ix + DSKTRK)
		jr	z, match

readtrack4:	push	ix
		push	ix				;copy dcb
		pop	hl
		ld	de, dcb
		ld	bc, 9
		ldir		
		
		ld	ix, dcb
		ld	(ix + DSKSEC), 1
		ld	b, 18
		
readtrack:	push	bc
		call	compbufadr
		ld	(dcb + DSKPTR), hl
				
		call	0f00fh
		
		pop	bc
		ld	a, (dcb + DSKSTS)		;error occured?
		or	a
		jr	z, readtrack6			;no	
		pop	ix				;yes, store in original dcb
		jr	match2
	
readtrack6:	ld	a, (dcb + DSKSEC)
		add	a, 5
		cp	19
		jr	c, readtrack2
		sub	a, 18
readtrack2:	ld	(dcb + DSKSEC), a
		djnz	readtrack
		
		ld	hl, track
		ld	a, (dcb + DSKTRK)
		cp	014h
		jr	nz, readtrack5
		inc	hl
readtrack5:	ld	(hl), a

		pop	ix

match:		call	compbufadr
		ld	d, (ix + DSKPTR+1)
		ld	e, (ix + DSKPTR)
		ld	bc, (LOGSIZ)		
		ldir
		
		xor	a
match2:		ld	(ix + DSKSTS), a
		call	0f03ch
		jp	0f9d7h



compbufadr:	ld	hl, (bufaddr)
		ld	b, (ix + DSKSEC)
		dec	b
		ld	c, 0
		ld	a, (dcb + DSKAUX)
		or	a
		jr	z, compbufadr1
		srl	b
		rr	c
compbufadr1:	add	hl, bc
		ret
		
track:		db	255
track14:	db	255
bufaddr:	dw	0

TSTRDY		EQU	0				;SELECT DRIVE AND TEST READY
GETSEC		EQU	1				;READ SECTOR
PUTSEC		EQU	2				;WRITE SECTOR
GETID		EQU	3				;READ ID MARK
DISKV		equ	0f00fh
;
;		
;
dcb:		db	0				;DISK OPERATION CODE
		db	0				;DRIVE# (WITH SIDE# IN BIT 7)
		db	0				;TRACK#
		db	0				;SECTOR#
		dw	0				;READ/WRITE POINTER
		dw	0				;AUXILLIARY PARAMETERS (2 BYTES)
		db	0				;OPERATION COMPLETION STATUS

		ld	ix, dcb
		ld	(ix + DSKOP), GETSEC
		ld	b, 18
testloop:	push	bc

;		ld	a, (ix + DSKSEC)
;		call	serhex
		
		call	DISKV
		pop	bc
		
;		ld	a, (dcb + DSKSTS)
;		call	serhex
;		call	serspace
		
		ld	a, (ix + DSKSEC)
		add	a, 4
		cp	19
		jr	c, testloop1
		sub	18
testloop1:	ld	(ix + DSKSEC), a
		djnz	testloop

		ld	a, '-'
		call	serout

		jr	$


		call	0f0abh
		push	af
		ld	a, '-'
		call	serout
		pop	af
		jp	0f0f6h



		call	STARTMR		
		call	FORCE
		
		ld	C,A		;SAVE CURRENT TYPE 1 DISK STATUS
		ld	B,6		;SET FOR 6 DISK REVOLUTIONS
		ld	HL,0
		ld	(TICKS),HL	;RESET MILLISECOND COUNTER FOR IRQ

SPIN2:		ld	DE,(TICKS)
		
		call	EDGE		;WAIT FOR INDEX INPUT TO CHANGE
		jr	c, SPIN3	;ABORT IF TIMEOUT
		call	EDGE		;WAIT FOR CHANGE BACK AGAIN
		jr	c, SPIN3
		djnz	SPIN2		;LET 6 REVOLUTIONS PASS
	
		ld	HL,(TICKS)	;READ TIME AT END OF REVOLUTION
		or	A
		sbc	HL,DE		;COMPUTE INDEX PERIOD IN MILLISECONDS

		call	STOPTMR
		
		ld	a, l
		ld	(PERIOD), a
;		call	serhex
;		push	af
;		ld	a, '+'
;		call	serout
;		pop	af
		jp	DEBUG+3


SPIN3:		ld	a, '*'
		call	serout
		
		jr	$
		
		
EDGE:
;		call	FORCE		;GET 1797 TYPE 1 STATUS
		in	a, (STSREG)
;		call	serhex
		xor	C
		and	00000010B	;CHECK FOR CHANGE IN INDEX BIT
		jr	NZ,EDGE2	;EXIT IF BIT CHANGES
		ld	A,(TICKS+1)
		cp	2048/256	;ELSE CHECK TIME ACCUMULATED IN 'TICKS'
		jr	C,EDGE		;KEEP LOOPING TILL 2 SECONDS PASS
	
		scf
		ret			;THEN RETURN WITH CARRY=1
;	
EDGE2:		ld	A,C
		cpl			;FLIP INDEX STATE HELD IN C
		ld	C,A
		ret			;RETURN WITH CARRY=0
		
		
		
FORCE:
;		ld	a, FINCMD	;LOAD FORCE-INTERRUPT-IMMEDIATE CMD
		in	a, (TRKREG)
		out	(DATREG), a
		ld	a, SKCMD+8
		out	(CMDREG), a
	
		ld	a, 14
force1:		dec	a
		jr	nz, force1
		IN	A,(STSREG)	;READ STATUS REGISTER CONTENTS
;		RES	7, A
		ret
	


;--------------------------------------------------
; get Pokeydivisor command '?'
;--------------------------------------------------
getspeed:
;		ld	a, 041h
;		out	(LATCH), a
;		call	FORCE
		
;		ld	a, '?'
;		call	serout
		
		call	DRVINDEX			;POINT IY TO DRIVE'S DATA AREA
		ret	C				;EXIT IF NOT A DRIVE IN OUR BOX
			
;		call	HASPARMS		
;		ret	Z				;EXIT IF DISK PARAMS NOT KNOWN
			
		call	SENDACK				;SEND 'ACK' FOR COMMAND FRAME

		ld	hl, IOBUFF+LEN-1
		ld	(hl), SIOFAST
		ld	de, 'C'		
		call	SENDBUFF			;SEND 'C' AND PARAMS DATA FRAME
		jp	togglebaud
		ret



;--------------------------------------------------
; cmdwait
;--------------------------------------------------
cmdwait:	ld	a, (CMDFLG)
                or	a				;SEE IF COMMAND FRAME HAS ARRIVED
                ret	z				;EXIT IF NOTHING HAS HAPPENED
					
;		call	sercmd				;5-byte command frame

		ld	a, (CMDFLG)
                cp	1		

		di					;ELSE RESET INTERRUPT AND START AGAIN
                ld	a, 00000011B		
                out	(CTC0),A		
                ei		
				
                jp	nz, cmdwait1
		
		jp	0f7f3h				;PROCESS COMMAND IF GOOD FRAME RECVD
				
cmdwait1:
		call	togglebaud	
		jp	0f809h


		
;--------------------------------------------------
; xmitbuf
;--------------------------------------------------
xmitbuf:	
		di	
		ld	a, (pokeydiv)			;is fast?
		cp	SIONORMAL
		jr	nz, xmitfast			;yes, jump
		ld	bc, 0f715h
		jp	XMITBUF+4
		
xmitfast:		
		ld	a, 00000111B			;CTC1 4uS pulses (4Mhz / 1*16)
		out	(CTC1), a
		ld	a, 1
		out	(CTC1), a
		
xmitfast1:	ld	a, (hl)				;7
		inc	hl				;6
		xor	d				;4
		ld	c, a				;4
		add	a, e				;4
		adc	0				;7
		ld	e, a				;4
		call	fastsend			;17 send byte in c
		ld	a, h				;4
		cp	iobuflenhi			;7
		jr	c, xmitfast1			;12/7 loop if buffer end not reached	
		
		ld	a, 00000011B	
		out	(CTC1), a	
		ei
		ret
		
fastsend:	xor	a
		out	(ATROUT), a
		
		push	ix				;15
		pop	ix				;14
		ld	a, c				;4
		ld	b, 8				;7
		inc	bc				;6
		nop					;4
		
fastsend1:	nop					;4
		cp	1				;7
		out	(ATROUT), a			;11
		rrca					;4
		push	ix				;15
		pop	ix				;14
		djnz	fastsend1			;13 / 8

		ld	a, r				;9
		ld	a, 1				;7
		out	(ATROUT), a			;11
		ret					;10
		
;--------------------------------------------------
; rxblock
;--------------------------------------------------		
rxblock:	ld	a, (pokeydiv)			;is fast?
		cp	SIONORMAL
		jp	z, rxblock1

		call	fastrecv			;yes, fast speed
		ld	a, 00000011B			;reset timer
		out	(CTC3), a		
		ld	c, d				;checksum in c
		ret

rxblock1:	ld	bc, 0				;no, normal speed
		jp	0f707h

;
; 32 bytes for disktab
;
disktab:	dw	0, 0, 0, 0, 0, 0, 0, 0
		dw	0, 0, 0, 0, 0, 0, 0, 0

pokeydiv:	db	SIONORMAL

;--------------------------------------------------
; togglebaud
;--------------------------------------------------		
togglebaud:	ld	a, (pokeydiv)
		cp	SIONORMAL
		ld	a, SIOFAST
		jr	z, togglebaud1
		ld	a, SIONORMAL
togglebaud1:	ld	(pokeydiv), a
		ret
		push	af
		ld	a, 'B'
		call	serout
		pop	af
		call	serhex
		jp	sercr



		
clear:		ld	hl, IOBUFF
clear1:		xor	a
		ld	(hl), a
		inc	hl
		ld	a, h
		cp 	iobuflenhi
		jr	nz, clear1
		ret

		


;--------------------------------------------------
; set 4ms watchdog
;--------------------------------------------------
irq4ms:		pop	af				;pop irq-addr
		or	a				;clear carry
irq4ms1:	ei
		reti	
		
;--------------------------------------------------
; SIO receive 57600 baud
;--------------------------------------------------
fastrecv:
		di
		ld	bc, irq4ms		
		ld	(CTCVEC+6),bc			;SET VECTOR TO irq4ms ROUTINE
		ld	bc, CTC3			;clear b, load c with CTC3
		ld	de, 10100111B			;clear d, load e with a7h
		out	(c), e				;12 PUT CTC3 IN TIMER MODE, PRESCALE 256
		out	(c), c
		ei

fastrecv1:	in	a, (ATARI)			;11
		rla					;4
		jp	c, fastrecv1			;10	NEW BYTE IS COMING IF START BIT LOW
							;14-25 / 7
		ld	a, d				;4
		add	a, b				;4
		adc	a, 0				;7 ACCUMULATE CHECKSUM ATARI STYLE
		ld	d, a				;4
							
		ld	b, 07fh				;7
		jp	fastrecv2a			;10 = 50
;	
; SERIAL->PARALLEL CONVERSION AT 17,36 MICROSECONDS PER BIT
;	
fastrecv2:	push	af				; 11
		pop	af				; 10
fastrecv2a:	ld	a, (hl)				; 7
		ld	a, (hl)				; 7
		
		in	a, (ATARI)			; 11 CYCLES
		rla					;  4 CYCLES
		rr	b				;  8 CYCLES
		jr	c, fastrecv2			; 12/7 = 70 / 65 cycles
		
		ld	(hl), b				;7 THEN STORE IN MEMORY BUFFER @HL
		inc	hl				;6
		ld	a, h				;4
		cp	iobuflenhi			;7
		ccf					;4
		ret	c				;5 RETURN WITH CARRY SET IF BUFFER FILLED

		out	(c), e				;12 PUT CTC3 IN TIMER MODE, PRESCALE 256
		out	(c), c				;12 COUNT MOD 256
				
		jp	fastrecv1			;10

	
serdumpcpl:	push	hl
                push	af
                push	bc
                push	de
		ld	d, 255
		jr	serdump1

;--------------------------------------------------
; RS232 sercmd
;--------------------------------------------------
sercmd:		push	hl
		ld	hl, CFRAME
		call	sercr
		jr	serdump2

;--------------------------------------------------
; RS232 dump 
;--------------------------------------------------
serdump:	push	hl
serdump2:	push	af
		push	bc
		push	de
		
		ld	d, 0
		
serdump1:	ld	a, (hl)
		xor 	d
		call	serhex
		call	serspace
		inc	hl
		ld	a, h
		cp	a, 0c3h
		jr	c, serdump1

		pop	de
		pop	bc
		pop	af
		pop	hl
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
; RS232 output HL in hex
;--------------------------------------------------
seraddr:	ld	a, h
		call	serhex
		ld	a, l
		call	serhex
		jr	serspace
;--------------------------------------------------
; RS232 output A in hex
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
serout:
		;jp	fastsend
		push	af
		push	bc
		ld	b, a
		xor	a
		di
		out	(SEROUT), a			;startbit
		call	time19600			;17

		ld	a, b
		ld	b, 8				;7
serout1:	out	(SEROUT), a			;11
		call	time19600			;17
		rrca					;4
		djnz	serout1				;8
		ei
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
		
;
;	... MILLISECOND TIMER INTERRUPT ROUTINES ...
;
;
STARTMR:
		di
		ld	A,10000111B
		out	(CTC3),A
		ld	A,250
		out	(CTC3),A
		ld	HL,TMRIRQ
		ld	(CTCVEC+6),HL
		ei
		ret
	
STOPTMR:
		di
		ld	A,00000001B
		out	(CTC3),A
		ei
		ret
;
;
;
TMRIRQ:
		push	HL
		ei
		ld	HL,(TICKS)
		inc	HL		;BUMP FREE RUNING MILLISECOND COUNTER
		ld	(TICKS),HL
		pop	HL
		reti
	
	

		
sallycode	equ	ASMPC
