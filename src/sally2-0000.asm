NOHIGHSPEEDSIO	equ	0

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

TSTRDY		equ	0				;SELECT DRIVE AND TEST READY
GETSEC		equ	1				;READ SECTOR
PUTSEC		equ	2				;WRITE SECTOR
GETID		equ	3				;READ ID MARK

DISKV		equ	0f00fh

CFRAME		equ	IOBUFF+LEN-5
IOBUFLENHI	equ	(IOBUFF+LEN) / 256


;--------------------------------------------------
; Firmware entry points
;--------------------------------------------------
;	diskio.mac
ACTIVON		equ	0f03ch
SHUTDOWN	equ	0f068h
DISK3		equ	0f0bfh
SEL4		equ	0f157h

;	bitbang.mac
SENDBUFF	equ	0f66ch
XMITBUF		equ	0f684h
RECVBUFF	equ	0f6b9h
RXBLOCK		equ	0f6d9h
STARTBIT	equ	0f719h

;	atari.mac
EMULATOR	equ	0f762h
LOGON		equ	0f77eh
CMDWAIT		equ	0f7e2h
CMDL4		equ	0f7f3h
CMDL5		equ	0f809h
DISKID		equ	0f82bh
DISKTAB		equ	0f83dh
DISKWRT1	equ	0f918h					;hook for setDirectSector write
DISKWRITE	equ	0f948h
DISKRD1		equ	0f993h					;hook for read direct sector
DISKREAD	equ	0f9d4h
SETSTAT		equ	0f9e3h
SECTRAN		equ	0fa10h
HASPARMS	equ	0fbd3h
DRVINDEX	equ	0fc28h
SENDACK		equ	0fc4ah
SENDCHAR	equ	0fc56h

;	format.max
SKEWSD		equ	0fe56h
SKEWDD		equ	0fe68h
SKEW13		equ	0fec8h
SKEW17		equ	0fee2h



;--------------------------------------------------
; Track-Buffer 26*256 bytes
;--------------------------------------------------
TRKBUF		equ	00800h

SIONORMAL	equ	40
SIOFAST		equ	8

		include "equs.mac"
		include "global.mac"

;--------------------------------------------------
; Code executed after Reset
;--------------------------------------------------
		ORG	00000h
;
reset:		di      				;disable interrupt
		xor     a				;set a to zero
;		ld	b, a
restime:	dec     a				;do 256 times nothing
		jr      nz, restime			;loop 
;		djnz	restime
		
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

;--------------------------------------------------
; firmware patch
;--------------------------------------------------
		
		ld	hl, 0ffffh			;reset drive / track buffer
		ld	(drive), hl

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
		inc	hl
		
		ld	hl, disktab
		ld	(DISKID), hl
		ld	(DISKID+2), hl
		ld	(DISKID+4), hl
		ld	(DISKID+6), hl
		
		ld	a, SIONORMAL
		ld	(pokeydiv), a		
		xor	a
		ld	(hispeed), a
		ld	a, 2
		ld	(direct), a
		ld	a, 0c3h				;'JP' instruction
		ld	(XMITBUF), a
		ld	(RXBLOCK), a
		ld	(CMDWAIT), a
		ld	(LOGON), a
		ld	(DISKWRT1), a
		ld	(DISKRD1), a
		
		ld	hl, xmitbuf
		ld	(XMITBUF+1), hl
		ld	hl, rxblock
		ld	(RXBLOCK+1), hl
		ld	hl, cmdwait
		ld	(CMDWAIT+1), hl
		ld	hl, diskread
		ld	(DISKREAD+1), hl
		ld	hl, diskwrite
		ld	(DISKWRITE+1), hl
		ld	hl, logon
		ld	(LOGON+1), hl
		ld	hl, diskwrt1
		ld	(DISKWRT1+1), hl
		ld	hl, diskrd1
		ld	(DISKRD1+1), hl
		
;--------------------------------------------------
; test code
;--------------------------------------------------
;		ld	a, 0c3h				;'JP' instruction
;		ld	(SEL4), a
;		ld	hl, sel4
;		ld	(SEL4+1), hl
;
;		ld	a, 255
;		ld	(CTCVEC+2), a
;		
;		ld	b, 3
;		ld	a, ' '
;stars:		call	serout
;;		djnz	stars	
;
;		ld	ix, testdcb
;		call	sercr
;		call	DISKV
;		call	dumpdcb
;		
;		ld	a, GETID
;		ld	(testdcb), a
;		call	DISKV
;		call	dumpdcb
;		call	dumprid
;	
;		ld	hl, 512
;		ld	(testdcb+DSKAUX), hl 
;		ld	a, GETSEC
;		ld	(testdcb), a
;		
;		ld	a, '*'
;		call	serout
;
;		ld	c, 1
;secloop1:	ld	b, 18
;		ld	hl, sectab
;secloop:	ld	a, (hl)
;		ld	(testdcb + DSKSEC),a
;		inc	hl
;		push	bc
;		push	hl
;		call	DISKV
;		pop	hl
;		pop	bc
;		djnz	secloop
;		dec	c
;		jr	nz, secloop1
;		
;		ld	a, '-'
;		call	serout
;		
;loop:		jr	loop
;;		
;;		

;		
;dumprid:	
;		ld	hl, (testdcb + DSKPTR)
;		call	sercr
;		ld	b, 6	
;dumprid1:	ld	a, (hl)
;		inc	hl
;		call	serhex
;		call	serspace
;		djnz	dumprid1
;		ret
;
;dummy:		xor	a
;		call	serhex
;		ret
;		
;testdcb:	db	TSTRDY				;DISK OPERATION CODE
;		db	1				;DRIVE# (WITH SIDE# IN BIT 7)
;		db	0				;TRACK#
;		db	1				;SECTOR#
;		dw	08000h				;READ/WRITE POINTER
;		dw	0				;AUXILLIARY PARAMETERS (2 BYTES)
;		db	0				;OPERATION COMPLETION STATUS
;		
;sectab:		
;		db	1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18
;		db	1, 3, 5, 7, 9, 11, 13, 15, 17, 2, 4, 6, 8, 10, 12, 14, 16, 18
;		db	1, 4, 7, 10, 13, 16, 2, 5, 8, 11, 14, 17, 3, 6, 9, 12, 15, 18
;		db	1, 5, 9, 13, 17, 3, 7, 11, 15, 2, 6, 10, 14, 18, 4, 8, 12, 16
;--------------------------------------------------
; end of test code
;--------------------------------------------------		
		jp	EMULATOR
	
logon:		LD	(RWMAX),A			;DO LESS RETRIES IN ATARI MODE

		ld	a, 0c3h				;'JP' instruction
		ld	(SEL4), a
		ld	hl, sel4
		ld	(SEL4+1), hl
		ld	hl, shutdown
		ld	(SHUTDOWN+1), hl
		jp	LOGON+3


shutdown:	call	0f228h
		ld	a, 00000111B			;CTC1 4uS pulses (4Mhz / 1*16)
		out	(CTC1), a
		ld	a, 1
		out	(CTC1), a
		ret
;
;
;
sel4:		bit	6, b				;8" found?
		jr	z, sel4ex			;yes, do nothing

		push	bc				;save registers
		push	de
		push	hl
		push	ix
		
		ld	a, b				;switch HD on
		res	6, a
		out	(LATCH), a

		push	ix				;load hl with ix 
		pop	hl				
		ld	de, dcb
		ld	bc, 9
		ldir					;copy dcb

		ld	ix, dcb
		ld	hl, id
		ld	(dcb + DSKPTR), hl
		ld	a, RIDCMD
		ld	(CMDBYT), a
		ld	a, 018h				;substitute JR	Z,xx by JR xx
		ld	(DISK3 + 031h), a
		ld	b, 6
sel4a:		push	bc
		call	DISK3+6				;DISK3: READ 6 BYTE ID RECORD
		pop	bc
		djnz	sel4a
		
		ld	a, 028h				;reset JR Z,xx
		ld	(DISK3 + 031h), a
		
		pop	ix
		pop	hl
		pop	de
		pop	bc
		
		ld	a, (dcb + DSKSTS)		;check disk status
		or	a
		jr	nz, sel4ex			;not zero, no HD
		res	6, b				;
sel4ex:	
		ld	a, b
		ld	d, 0
		jp	SEL4+3

;--------------------------------------------------
; hook for read directSector if aux1/2 = 0
;--------------------------------------------------
diskrd1:
		ld	a, 2
		ld	(direct), a			;clear direct
		
		ld	hl, (CFRAME + 2)		;load DAUX1/2, test if zero
		ld	a, l
		or	h
		jr	z, diskrd1a			;if zero, use direct sector number	
		call	SECTRAN				;else use original code, call SECTRAN
		jp	DISKRD1+3			;and procees
		
diskrd1a:	ld	ix, DKIOCB
		ld	(ix + DSKOP), TSTRDY
;		call	dumpdcb	
		push	hl
		call	DISKV
		pop	hl
		
		call	sec2track			;compute sector, track, side from direct sector		
		ld	(ix + DSKOP), GETSEC
;		call	dumpdcb
		push	hl
		call	diskread			;CALL DISK I/O HANDLER
		pop	hl
		
		ld	a, (DKIOCB+DSKSTS)
		call	SETSTAT				;call SETSTAT
		ld	d, 0				;no invert
		jp	SENDBUFF			;jump SENDBUFF

;--------------------------------------------------
;compute sector, track, side from direct sector		
;--------------------------------------------------
sec2track:	xor	a				;clear carry and a
		ld	b, 0ffh				;also b
		ld	hl, (dsector)			;compute track and side numer
		ld	de, 18				;18 secs per track
sec2track1:	inc	b				;b holds track-number
		sbc	hl, de
		jr	nc, sec2track1			;subtract 18 as long carry clear
		ld	a, l	
		add	19				;add 19 to get sector number + 1
		ld	(DKIOCB+DSKSEC), a
		srl	b				;divide track by two, (we have two sides)
		ld	a, b
		ld	(DKIOCB+DSKTRK), a
		rra					;shift-in side-number from previously lowest-bit	
		and	080h				;mask out bit 0-6
		ld	hl, DKIOCB+DSKDRV
		res	7, (hl)
		or	(hl)
		ld	(hl), a
		
		call	serhex
		call	serspace
		ld	a, 'S'
		call	serout
		ld	a, (DKIOCB+DSKSEC)
		call	serhex
		call	serspace
		ld	a, 'T'
		call	serout
		ld	a, (DKIOCB+DSKTRK)
		call	serhex
		call	sercr
		
		ld	hl, 512
		ld	(DKIOCB+DSKAUX), hl
		ld	hl, IOBUFF
		ld	(DKIOCB+DSKPTR), hl
		ld	ix, DKIOCB
		ret
		
;--------------------------------------------------
; hook for setDirectSector if aux1/2 = 0
;--------------------------------------------------
diskwrt1:	push	hl
		ld	(LOGSIZ),HL			;SAVE DATA BLOCK LENGTH	
		
		ld	hl, (CFRAME + 2)		;load DAUX1/2
		ld	a, l
		or	h
		jr	z, diskwrt1a			;if zero, do special stuff
		
		pop	hl				;otherwise continue
		jp	DISKWRT1+3			;normal
		
diskwrt1a:	call	SENDACK

		ld	hl, direct			;direct
;		ld	a, (hl)
;		call	serhex		
		dec	(hl)
		jr	z, diskwrt1d			;first sector
		jp	m, diskwrt1b			;second sector
		
		ld	b, 64
		djnz	$				;wait some time

		ld	hl, (IOBUFF)			;save 2-byte sector number (0-xxxx)
		ld	(dsector), hl

diskwrt1c:	pop	hl
		ld	a, 'C'
		jp	SENDCHAR
	
diskwrt1d:	ld	hl, IOBUFF
		ld	de, IOBUFF+LEN+2
		ld	bc, 0100h			;rec first half
		ldir
		jr	diskwrt1c
		
diskwrt1b:	ld	(hl), 2				;direct = 2
		
		ld	hl, IOBUFF
		ld	de, IOBUFF+0100h
		ld	bc, 0100h
		ldir
		ld	hl, IOBUFF+LEN+2
		ld	de, IOBUFF
		ld	bc, 0100h
		ldir

		call	sec2track

		ld	(ix + DSKOP), PUTSEC
		call	diskwrite

		jr	diskwrt1c
;--------------------------------------------------
; diskwrite: write through sector
;--------------------------------------------------
diskwrite:	
;		ld	a, 'W';
;		call	debug
		
;		jp	DISKV
		
		call	checktrack		
		jr	nz, diskwrite1
		
		call	compbufadr
		ex	de, hl
		ld	h, (ix + DSKPTR+1)
		ld	l, (ix + DSKPTR)
		ld	bc, (LOGSIZ)		
		ldir		
		
diskwrite1:	jp	DISKV

;--------------------------------------------------
; Debug routine
;--------------------------------------------------
;debug:		call	serout
;		ld	a, (ix + DSKDRV)
;		call	serhex
;		ld	a, 't'
;		call	serout
;		ld	a, (ix + DSKTRK)
;		call	serhex		
;		ld	a, 's'
;		call	serout
;		ld	a, (ix + DSKSEC)
;		call	serhex
;		ld	a, (ix + DSKPTR+1)
;		call	serhex
;		ld	a, (ix + DSKPTR)
;		call	serhex
;		ld	a, (ix + DSKAUX+1)
;		call	serhex
;		ld	a, (ix + DSKAUX)
;		call	serhex
;		
;		call	serspace
;		ld	a, (CFRAME+3)
;		call	serhex
;		ld	a, (CFRAME+2)
;		call	serhex
;		call	serspace
;		ld	a, (IY+NSECS+1)
;		call	serhex
;		ld	a, (iy+NTRKS)
;		call	serhex
;		
;		call	sercr
;		ret
;
dumpdcb:	push	af
		push	bc
		push	hl
		
		push	ix
		pop	hl
;		call	sercr
		ld	b, 9	
dumpdcb1:	ld	a, (hl)
		inc	hl
		call	serhex
		call	serspace
		djnz	dumpdcb1
		call	sercr
		
		pop	hl
		pop	bc
		pop	af
		ret
;--------------------------------------------------
; diskread: cache a track
;--------------------------------------------------
diskread:	
;		ld	a, 'R'
;		call	debug
		
		call	checktrack		
		jp	z, match
		
		ld	(drive), de			;save new drive and track		
		push	ix				;save ix
		
		push	ix				;load hl with ix 
		pop	hl
		ld	de, dcb
		ld	bc, 9
		ldir					;copy dcb

		ld	ix, dcb				;load ix with new dcb
		
		ld	a, (dcb + DSKAUX+1)		;get sector length high-byte
		cp	2
		jr	nz, readtrack2			;no MS-DOS disk


		ld	hl, TRKBUF
		ld	bc, 18 * 256 + 1		;b=18, c = 1
		
readtrack3:	ld	(dcb + DSKPTR), hl
		ld	(ix + DSKSEC), c
		
;		call	dumpdcb
		
		push	hl
		push	bc
		call	DISKV
		pop	bc
		pop	hl
;		call	dumpsec
		ld	a, (dcb + DSKSTS)		;error occured?
		or	a
		jr	nz, readtrack4			;yes
		inc	h
		inc	h
		inc	c
		djnz	readtrack3
		jr	readtrack5
	
readtrack2:	ld	b, 0				;compute skew-list from media type
		ld	c, (iy + MEDIA)		
		ld	hl, skewtab
		add	hl, bc
		ld	a, (hl)
		inc	hl
		ld	h, (hl)
		ld	l, a
		ld	b, (iy + NSECS+1)

readtrack1:	ld	(secptr), hl

readtrack:	ld	hl, (secptr)
		ld	a, (hl)
			ld	(dcb + DSKSEC), a
		inc	hl
		ld	(secptr), hl
		
		push	bc
		call	compbufadr
		ld	(dcb + DSKPTR), hl
		call	DISKV
		pop	bc

		ld	a, (dcb + DSKSTS)		;error occured?		
		or	a
		jr	z, readtrack6			;no	
readtrack4:	;call	dumpdcb
		pop	ix				;yes, store in original dcb
		jr	match2
readtrack6:	djnz	readtrack
readtrack5:	pop	ix
		
match:		call	compbufadr
		ld	d, (ix + DSKPTR+1)
		ld	e, (ix + DSKPTR)
		ld	bc, (LOGSIZ)		
		ld	a, (dcb + DSKAUX+1)		;get sector length high-byte
		cp	2
		jr	nz, match1
		ld	bc, 512
match1:		ldir
		
		xor	a
match2:		ld	(ix + DSKSTS), a

		ld	a, FINCMD			;keeps motor spinning
		call	CMDOUT

		jp	ACTIVON


;--------------------------------------------------
; dump 512 bytes at hl
;--------------------------------------------------
dumpsec:	push	bc
		push	hl
		call	seraddr
		call	sercr
		ld	bc, 512
match3a:	ld	a, (hl)
		call	serhex
		ld	a, b
		and	15
		jr	nz, match3
		call	sercr
match3:		inc	hl
		dec	c
		jr	nz, match3a
		djnz	match3a
		pop	hl
		pop	bc
		jp	sercr

;--------------------------------------------------
; hl = TRKBUF + DSKSEC * (128/256/512)
;--------------------------------------------------
compbufadr:	ld	hl, TRKBUF
		ld	b, (ix + DSKSEC)
		dec	b
		ld	c, 0
		ld	a, (dcb + DSKAUX + 1)		;load seclen highbyte
		or	a
		jr	z, compbufadr2			;if zero, assume 128 bytes length
		cp	1
		jr	z, compbufadr1			;if 1, assume 256 bytes 
		sla	b				;512 bytes
		jr	compbufadr1
compbufadr2:	srl	b				;128 bytes
		rr	c
compbufadr1:	add	hl, bc
		ret
	
checktrack:	ld	hl, (drive)			;load 
		ld	d, (ix + DSKTRK)		;high
		ld	e, (ix + DSKDRV)		;low
		or	a				;clear carry
		sbc	hl, de
		ret
		
;--------------------------------------------------
; get Pokeydivisor command '?'
;--------------------------------------------------
getspeed:	
;		ld	a, '?'
;		call	serout
		
		call	DRVINDEX			;POINT IY TO DRIVE'S DATA AREA
		ret	C				;EXIT IF NOT A DRIVE IN OUR BOX
			
;		call	HASPARMS		
;		ret	Z				;EXIT IF DISK PARAMS NOT KNOWN
			
		call	SENDACK				;SEND 'ACK' FOR COMMAND FRAME

		ld	hl, IOBUFF+LEN-1
		
		IF NOHIGHSPEEDSIO <> 1
		ld	a, SIOFAST
		ld	(hl), a
		ld	(hispeed), a
		ELSE
		ld	(hl), SIONORMAL
		ENDIF
				
		ld	de, 'C'		
		jp	SENDBUFF			;SEND 'C' AND PARAMS DATA FRAME


;--------------------------------------------------
; cmdwait
;--------------------------------------------------
cmdwait:	ld	a, (CMDFLG)
                or	a				;SEE IF COMMAND FRAME HAS ARRIVED
                ret	z				;EXIT IF NOTHING HAS HAPPENED
					
		call	sercmd				;5-byte command frame
		
		ld	a, (CMDFLG)
                cp	1		

		di					;ELSE RESET INTERRUPT AND START AGAIN
                ld	a, 00000011B		
                out	(CTC0),A		
		ei		
				
                jp	z, CMDL4			;good cmd-frame
	
		call	togglebaud		
		jp	CMDL5


		
;--------------------------------------------------
; xmitbuf
;--------------------------------------------------
xmitbuf:	
		di	
		ld	a, (pokeydiv)			;is fast?
		cp	SIONORMAL
		jr	nz, xmitfast			;yes, jump
		ld	bc, STARTBIT
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
		cp	IOBUFLENHI			;7
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

rxblock1:
		ld	bc, 0				;no, normal speed
		jp	RXBLOCK+3

;--------------------------------------------------
; togglebaud
;--------------------------------------------------		
togglebaud:	;ld	a, (hispeed)
		;or	a
		;ret	z
		
		ld	a, (pokeydiv)
		cp	SIONORMAL
		ld	a, SIOFAST
		jr	z, togglebaud1
		ld	a, SIONORMAL
togglebaud1:	ld	(pokeydiv), a
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
fastrecv2:	push	af				;11
		pop	af				;10
fastrecv2a:	ld	a, (hl)				;7
		ld	a, (hl)				;7
							 
		in	a, (ATARI)			;11 CYCLES
		rla					; 4 CYCLES
		rr	b				; 8 CYCLES
		jr	c, fastrecv2			;12/7 = 70 / 65 cycles
		
		ld	(hl), b				;7 THEN STORE IN MEMORY BUFFER @HL
		inc	hl				;6
		ld	a, h				;4
		cp	IOBUFLENHI			;7
		ccf					;4
		ret	c				;5 RETURN WITH CARRY SET IF BUFFER FILLED
		
		out	(c), e				;12 PUT CTC3 IN TIMER MODE, PRESCALE 256
		out	(c), c				;12 COUNT MOD 256
				
		jp	fastrecv1			;10

	
;serdumpcpl:	push	hl
;               push	af
;               push	bc
;               push	de
;		ld	d, 255
;		jr	serdump1

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
		
CMDOUT:
		OUT	(CMDREG),A			;OUTPUT DISK CONTROLLER COMMAND BYTE
CMDT1:		LD	A,14	
CMDT2:		DEC	A
		JR	NZ,CMDT2			;DELAY 56 MICROSECONDS
CMDT3:		in	a, (STSREG)
		bit	0, a
		jr	nz, CMDT1
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

;--------------------------------------------------
; variables and data structure
;--------------------------------------------------
;
; 32 bytes for disktab
; 7 commands copied in from DISKTAB, 3x7=21
; 2 commands added '?' and 0xDD (setDirectSector)
;
disktab:	dw	0, 0, 0, 0, 0, 0, 0, 0
		dw	0, 0, 0, 0, 0, 0, 0, 0

;
; direct MSDOS sector for each drive
;
dsector:	db	0, 0
direct:		db	0
		
pokeydiv:	db	SIONORMAL
hispeed:	db	0

drive:		db	255
track:		db	255
secptr:		dw	0		

skewtab:	dw	SKEWSD
		dw	SKEW13
		dw	SKEWDD
		dw	SKEW17

dcb:		db	0				;DISK OPERATION CODE
		db	0				;DRIVE# (WITH SIDE# IN BIT 7)
		db	0				;TRACK#
		db	0				;SECTOR#
		dw	0				;READ/WRITE POINTER
		dw	0				;AUXILLIARY PARAMETERS (2 BYTES)
		db	0				;OPERATION COMPLETION STATUS

id:		dw	0, 0, 0	
		
sallycode	equ	ASMPC				;append Sally code here
	