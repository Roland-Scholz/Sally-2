
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

		include "equs.mac"
		include "global.mac"

;--------------------------------------------------
; Code executed after Reset
;--------------------------------------------------
		ORG	00000h
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
							 
		ld      sp, 0c100h			;set stack to 0c100h
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

code0000:	call	0f5fch				;coninit


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
