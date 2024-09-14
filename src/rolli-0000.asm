
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
	
main:		ld	hl, DISKTAB+2
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
		
		ld	hl, xmitbuf
		ld	(XMITBUF+1), hl
		ld	hl, rxblock
		ld	(RXBLOCK+1), hl
		ld	hl, cmdwait
		ld	(CMDWAIT+1), hl
		
		ld	a, SIONORMAL
		ld	(pokeydiv), a
	
		ld	hl, disktab
		ld	de, 8*3+2
		call	serdump
		
		
		ld	c, 'A'
testfast:	call	fastsend
		inc	c
		ld	a, c
		cp	'Z' + 1
		jr	c, testfast
		
		jp	0f762h


;--------------------------------------------------
; get Pokeydivisor command '?'
;--------------------------------------------------
getspeed:	ld	a, '?'
		call	serout
		
		call	DRVINDEX			;POINT IY TO DRIVE'S DATA AREA
		ret	C				;EXIT IF NOT A DRIVE IN OUR BOX
			
;		call	HASPARMS		
;		ret	Z				;EXIT IF DISK PARAMS NOT KNOWN
			
		call	SENDACK				;SEND 'ACK' FOR COMMAND FRAME

		ld	hl, IOBUFF+LEN-1
		ld	(hl), SIOFAST
		ld	de, 'C'		
		jp	SENDBUFF			;SEND 'C' AND PARAMS DATA FRAME


cmdwait:	ld	a, (CMDFLG)
                or	a				;SEE IF COMMAND FRAME HAS ARRIVED
                ret	z				;EXIT IF NOTHING HAS HAPPENED
					
		call	sercmd				;5-byte command frame
		call	serhex				;status 1 = OK
		
                cp	1		
                jp	nz, cmdwait1
		
		jp	0f7f3h				;PROCESS COMMAND IF GOOD FRAME RECVD
				
cmdwait1:       di					;ELSE RESET INTERRUPT AND START AGAIN
                ld	a, 00000011B		
                out	(CTC0),A		
                ei		

		call	togglebaud
		
		jp	0f809h


		
xmitbuf:	ld	a, (pokeydiv)			;is fast?
		cp	SIONORMAL
		jr	nz, xmitbuf1			;yes, jump
		di					;no
		ld	bc, 0f715h
		jp	XMITBUF+4
		
xmitbuf1:	ld	a, (hl)
		inc	hl
		xor	d
		ld	c, a
		add	a, e
		adc	0
		ld	e, a
		call	fastsend			;send byte in c
		ld	a, h
		cp	iobuflenhi
		jr	c, xmitbuf1			;loop if buffer end not reached
		ret
		
		
rxblock:	ld	a, (pokeydiv)			;is fast?
		cp	SIONORMAL
		jp	nz, rxblock1
		ld	bc, 0
		jp	0f707h

rxblock1:	call	fastrecv
		ld	c, d
		ret
;
; 32 bytes for disktab
;
disktab:	dw	0, 0, 0, 0, 0, 0, 0, 0
		dw	0, 0, 0, 0, 0, 0, 0, 0

pokeydiv:	db	SIONORMAL

togglebaud:	ld	a, (pokeydiv)
		cp	SIONORMAL
		ld	a, SIOFAST
		jr	z, togglebaud1
		ld	a, SIONORMAL
togglebaud1:	ld	(pokeydiv), a
		call	serhex
		jp	sercr


		call	clear
		ld	hl, IOBUFF
		call	seraddr


serloop:	ld	hl, IOBUFF
		call	fastrecv

		ld	a, 00000011B			;reset timer
		out	(CTC3), a
		ld	c, d
		
		jp	c, main
		
		ld	de, IOBUFF
		sbc	hl, de
		jr	z, serloop
serloop2:	ex	de, hl
;		ld	(hl), c
		call	serdump
		
		jp	main
		
clear:		ld	hl, IOBUFF
clear1:		xor	a
		ld	(hl), a
		inc	hl
		ld	a, h
		cp 	iobuflenhi
		jr	nz, clear1
		ret

		
fastsend:	xor	a
		out	(ATROUT), a
		call	time				;11
		ld	a, c				;4
		ld	b, 8				;7
		nop					;4
		nop					;4
		nop					;4
		
fastsend1:	out	(ATROUT), a			;11
		rrca					;4
		call	time				;17
		djnz	fastsend1			;12
		
		ld	a, 1				
		out	(ATROUT), a

		ex	(sp), hl			;19
		ex	(sp), hl			;19
		ex	(sp), hl			;19
		ex	(sp), hl			;19
		
time:		nop					;4
		nop					;4
		nop					;4
		nop					;4
		ret					;10 = 26

;--------------------------------------------------
; set 4ms watchdog
;--------------------------------------------------
irq4ms:		pop	af				;pop irq-addr
;		ld	a, 00000011B			;reset timer
;		out	(CTC3), a
		or	a				;clear carry
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
		jr	c, fastrecv2			; 12/7
		
		ld	(hl), b				;7 THEN STORE IN MEMORY BUFFER @HL
		inc	hl				;6
		ld	a, h				;4
		cp	iobuflenhi			;7
		ccf					;4
		ret	c				;5 RETURN WITH CARRY SET IF BUFFER FILLED

		out	(c), e				;12 PUT CTC3 IN TIMER MODE, PRESCALE 256
		out	(c), c				;12 COUNT MOD 256
				
		jp	fastrecv1			;10

	
;--------------------------------------------------
; RS232 dump
;--------------------------------------------------
serdump:	;call	seraddr
;		ex	de, hl
;		call	seraddr
;		ex	de, hl
;		ld	a, c
;		call	serhex
;		call	sercr
		
;		ld	hl, IOBUFF
;		ld	de, 256
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
serdump3:;	call	sercr
	;	ld	a, (hl)
	;	call	serhex
		call	sercr
		ret
		

;--------------------------------------------------
; RS232 sercmd
;--------------------------------------------------
sercmd:		push	af
		push	bc
		push	hl
		ld	hl, CFRAME
		ld	b, 5
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
