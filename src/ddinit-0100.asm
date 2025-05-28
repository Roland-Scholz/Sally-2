LATCH		equ	30h
CMDREG		equ	40h
STSREG		equ	40h
TRKREG		equ	41h
SECREG		equ	42h
DATREG		equ	43h

READSEC		equ	88h
FORCEINT	equ	0d0h
RESTORE		equ	01h
STEPIN		equ	04bh

NULL		equ	000h
CR			equ	00dh
LF			equ	00ah


     		ORG	00100h
;
; Disassembly of the file "C:\github\Sally-1\ATR8000-CPM\ddinit.com"
; 
; CPU Type: Z80
; 
; Created with dZ80 2.0
; 
; on Saturday, 24 of August 2024 at 11:42 AM
; 
     		ld	sp,0c69h
     		ld	bc,0018h			;c = 24
l0106:		push	bc
     		call	printstr
		db	CR, LF,NULL
     		pop	bc
l010e:		ex	(sp),hl
     		ex	(sp),hl
     		ex	(sp),hl
     		ex	(sp),hl
     		djnz	l010e				;waste some time		
     		dec	c
     		jr	nz,l0106			;24 x newline
		
     		call	printstr
		db	".... ATR8000 Disk Formatter  2.1 ....",CR, LF,LF,NULL
		
      		ld	a,(0218h)			;first time called?
     		or	a
     		jp	nz,0219h
     		ld	a,01h
     		ld	(0218h),a
		
     		call	printstr
		db	"This program erases all recorded data",CR, LF
		db	"in the process of formatting. Please",CR, LF
		db	"remove any valuable diskettes before",CR, LF
		db	"proceeding ... ", NULL
		
l01d5:		ld	hl,0000h
l01d8:		push	hl
     		call	const
     		pop	hl
l01dd:		jr	nz,l01e6		; (+07h)
     		dec	hl
     		ld	a,h
     		or	l
     		jr	nz,l01d8		; (-0ch)
     		jr	l01e9		; (+03h)
		
l01e6:		call	getupper
l01e9:		push	af
     		call	printstr
		db	CR, LF, NULL
     		pop	af
l01f1:		cp	03h
     		jr	nz,l0219		; (+24h)
     		call	printstr
		db	CR, LF, LF, "returning to CP/M ... ", CR, LF, LF, NULL
     		jp	0000h
		
l0218:		nop
l0219:		call	printstr
		db	CR, LF
		db	"--- Disk Format Menu ---", CR, LF, LF
		db	"   density   sector size",CR, LF
		db	"   -------   -----------",CR, LF
		db	"1) single     128 bytes",CR, LF
		db	"2) double     256 bytes",CR, LF
		db	"3) double     512 bytes",CR, LF
		db	"4) double    1024 bytes", CR, LF, LF
		db	"enter number of format (1 thru 4) ... ", NULL

l02f9:		call	06c7h
     		cp	31h
     		jr	c,l02f9
     		cp	35h
     		jr	nc,l02f9
     		sub	31h
     		ld	(067bh),a			;sector size 
		
		call	printstr
		db	CR, LF, "enter drive name (A,B,C or D) ....... ", NULL
l0335:		call	06c7h
     		cp	41h
     		jr	c,l0335
     		cp	45h
     		jr	nc,l0335
     		sub	41h
     		ld	(0682h),a			;drive no
     		xor	a
     		ld	(0680h),a			;bits 7: single / double = 1
l0349:		call	printstr
		db	CR, LF, "single or double sided (S or D) ..... ", NULL
		
l0375:		call	06c7h
     		cp	53h
     		jr	z,l0385
     		cp	44h
     		jr	nz,l0375
     		ld	hl,0680h
     		set	7,(hl)
l0385:		call	printstr
		db	CR, LF, NULL
		ld      a,(0682h)
		add     a,41h
		ld      (03bch),a
		
		
		call	printstr
		db	CR, LF, "press <Y> when ready to format disk A",CR, LF
		db	"or <CTL-C> to exit to CP/M ... ", NULL
		
l03df:		call	070bh
     		call	getupper
     		cp	03h
     		jp	z,01f5h
     		cp	59h		;'Y'
     		jr	z,l03f4		; (+06h)
     		call	conout
     		jp	0100h
		
l03f4:		call	printstr
		db	CR, LF, LF, NULL
		
     		ld	ix,0681h			;select drive 
l03ff:		call	0f00fh
     		ld	a,(0689h)
     		bit	7,a
     		jr	z,l044e		; (+45h)
l0409:		call	printstr
		db	"*** DRIVE NOT READY ***",CR, LF
		db	"(drive door open?  disk in wrong?)",CR, LF, 7, NULL
l044b:		jp	038bh

l044e:		bit	6,a
     		jr	z,l048d		; (+3bh)
     		call	printstr
		db	"*** WRITE PROTECTED ***",CR, LF
		db	"(check write protect tab)",CR, LF, NULL
     		jp	038bh
		
l048d:		call	printstr
		db	CR, LF, NULL
		
     		ld	hl,0000h
     		ld	(0636h),hl
     		ld	(0638h),hl
     		ld	a,(067bh)			;sector size lo
     		or	a
     		ld	a,(0687h)
     		jr	z,l04a9		; (+04h)
     		res	7,a
l04a7:		jr	l04ab		; (+02h)
l04a9:		set	7,a				;if sec size lo = 0, set bit 7 in 687h
l04ab:		ld	(0687h),a
     		ld	hl,063bh
l04b1:		bit	6,a
     		jr	z,l04b8		; (+03h)
     		ld	hl,0657h
l04b8:		ld	de,0673h
     		ld	bc,0007h
     		ld	a,(067bh)
     		and	03h
     		jr	z,l04c9		; (+04h)
l04c5:		add	hl,bc
     		dec	a
     		jr	nz,l04c5		; (-04h)
l04c9:		ldir

     		ld	a,(0677h)
     		ld	h,a
     		ld	a,(0675h)
     		ld	l,a
l04d3:		call	09a1h
     		ld	hl,(0001h)
     		ld	bc,0018h
     		add	hl,bc
     		ld	a,(0682h)
     		ld	c,a
     		ld	b,00h
     		ld	de,0ffffh
     		call	05c2h
     		ld	bc,0023h
l04ec:		add	hl,bc
l04ed:		ld	a,(hl)
     		or	a
     		jr	z,l04f4		 
     		ld	(0676h),a
l04f4:		ld	a,(0080h)
     		cp	03h
     		jr	nz,l0532		
     		ld	hl,(0081h)
     		ld	bc,2f20h
     		or	a
     		sbc	hl,bc
     		jr	nz,l0532		
     		ld	a,(0083h)
     		cp	30h
     		jr	nz,l051a		
     		ld	hl,09d6h
     		ld	a,(0677h)
l0513:		dec	(hl)
     		inc	hl
     		dec	a
     		jr	nz,l0513		 
     		jr	l0532		 
l051a:		ld	a,50h			;80 tracks
     		ld	(0676h),a
     		xor	a
     		ld	(067fh),a
     		ld	a,(0083h)
     		cp	44h
     		jr	nz,l0532
     		ld	a,28h			;40 tracks
     		ld	(067fh),a
     		ld	(0676h),a
l0532:		di
     		ld	a,03h
     		out	(83h),a
     		ei
     		call	l0777
     		in	a,(TRKREG)
     		out	(DATREG),a
     		ld	a,10h			;FDC seek
     		out	(CMDREG),a
     		xor	a
     		out	(LATCH),a
     		call	printstr
		db	CR, LF, LF, NULL
     		ld	hl,0676h
     		ld	a,(067ah)
     		inc	a
     		cp	(hl)
     		jr	nc,l0569		; (+12h)
     		call	printstr
		db	"aborted ... ", NULL
l0567:		jr	l0576		; (+0dh)

l0569:		call	printstr
		db	"done ... ", NULL
		
l0576:		ld	hl,(0636h)
     		ld	a,h
     		or	l
     		jr	nz,l059a		; (+1dh)
     		call	printstr
		db	"no bad sectors detected", NULL
     		jr	l05b9		; (+1fh)
l059a:		ld	hl,(0636h)
     		call	072bh
l05a0:		call	printstr
		db	" bad sectors detected", NULL
		
l05b9:		call	printstr
		db	CR, LF, NULL
     		jp	038bh
		
     		jp	(hl)
		
prttrack:     	call	printstr
		db	CR, "TRACK ", NULL
		
l05ce:		ld	a,(067fh)
     		or	a
     		ld	a,(067ah)
     		jr	z,l05d8		; (+01h)
     		add	a,a
l05d8:		ld	h,00h
     		ld	l,a
     		call	072bh
     		ld	a,(0680h)
     		bit	7,a
     		jr	z,l05fa		; (+15h)
     		call	printstr
		db	"  SIDE ", NULL
		
l05f0:		ld	a,(063ah)
     		and	01h
     		add	a,30h
     		call	conout
l05fa:		ld	hl,(0636h)
     		ld	de,(0638h)
     		or	a
     		sbc	hl,de
     		jr	z,l0632		; (+2ch)
     		call	printstr
     		db	07, "  *** ", NULL
l0611:		call	072bh
     		call	printstr
		db	" bad sector(s) ***",CR, LF, NULL
l062c:		ld	hl,(0636h)
     		ld	(0638h),hl
l0632:		call	06f9h
     		ret

     		nop
     		nop
     		nop
     		nop
     		nop
     		sub	0ah
     		ld	bc,1a4dh
     		ld	e,b
     		inc	d
     		ei
     		ld	a,(bc)
     		inc	bc
     		ld	c,l
     		ld	a,(de)
     		or	b
     		jr	z,l066e		; (+24h)
     		dec	bc
     		inc	bc
     		ld	c,l
;     		djnz	l05ff		; (-50h)
		db 	10h, 0b0h
     		jr	z,l0696		; (+45h)
l0651:		dec	bc
     		inc	bc
     		ld	c,l
     		add	hl,bc
     		or	b
     		jr	z,l06be		; (+66h)
     		dec	bc
     		ld	bc,1228h
     		dec	(hl)
     		inc	c
     		add	a,l
		dec	bc
     		inc	bc
     		jr	z,l0675		; (+12h)
     		ld	l,d
;     		jr	l060c		; (-5ah)
		db	18h, 0a6h
     		dec	bc
		inc	bc
     		jr	z,l0674		; (+0ah)
     		ld	l,d
l060c:     	;jr	l05ff;		l0634		; (-39h)
		db	18h, 0c7h
     		dec	bc
l066e:		inc	bc
     		jr	z,l0676		; (+05h)
     		ld	l,d
     		jr	l0674		; (+00h)
l0674:		nop
l0675:		nop
l0676:		nop
     		nop
     		nop
     		nop
     		nop
l067b:		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
		
conout:     	push	af
     		push	bc
     		push	de
     		push	hl
     		ld	c,a
     		ld	hl,(0001h)			;BIOS
     		ld	de,0009h			;conout
     		add	hl,de
l0696:		call	callhl
     		pop	hl
     		pop	de
     		pop	bc
     		pop	af
     		ret

getupper:     	push	hl
     		push	de
     		push	bc
     		ld	hl,(0001h)
     		ld	de,0006h			;conin
     		add	hl,de
     		call	callhl
     		cp	60h
     		jr	c,l06b1		; (+02h)
     		sub	20h
l06b1:		pop	bc
     		pop	de
     		pop	hl
     		ret

const:     	push	hl
     		push	de
     		push	bc
     		ld	hl,(0001h)
     		ld	de,0003h			;const
l06be:		add	hl,de
     		call	callhl
     		pop	bc
     		pop	de
     		pop	hl
     		ret

callhl:    	jp	(hl)
		
l06c7:		call	printstr
		db	32, 8, 0
l06cd:		call	getupper
     		cp	03h
     		jp	z,01f5h
     		cp	20h
     		jr	c,l06cd		; (-0ch)
     		ld	c,a
     		call	conout
l06dd:		call	getupper
     		cp	08h
     		jr	nz,l06e9		; (+05h)
     		call	conout
     		jr	l06c7		; (-22h)
l06e9:		cp	03h
     		jp	z,01f5h
     		cp	0dh
     		jr	nz,l06dd		; (-15h)
     		call	printstr
     		ex	af,af'
     		nop
     		ld	a,c
     		ret

     		ld	a,(0be8h)
     		or	a
     		ret	nz

     		call	const
     		or	a
     		ret	z

     		call	getupper
     		ld	(0be8h),a
     		or	a
     		ret

l070b:		xor	a
     		ld	(0be8h),a
     		call	06f9h
     		jr	nz,l070b		; (-09h)
     		ret

printstr:     	ex	(sp),hl
     		call	printhl
     		ex	(sp),hl
     		ret

printhl:	ld	a,(hl)
     		inc	hl
     		cp	00h
     		ret	z

     		call	conout
     		jr	printhl		; (-0ah)
		
		
     		ld	a,20h
     		call	0731h
     		ret

     		ld	a,00h
     		call	0731h
     		ret

     		push	de
     		push	bc
     		ld	c,a
     		ld	iy,076dh
     		res	0,b
l073a:		ld	e,(iy+00h)
     		inc	iy
     		ld	d,(iy+00h)
     		inc	iy
     		dec	e
     		jr	z,l0764		
     		xor	a
l0748:		sbc	hl,de
     		jr	c,l074f		
     		inc	a
     		jr	l0748		
l074f:		add	hl,de
     		add	a,30h
     		cp	30h
     		jr	nz,l075d	
     		bit	0,b
     		jr	nz,l075f	
     		ld	a,c
     		jr	l075f		
l075d:		set	0,b
l075f:		call	conout
     		jr	l073a		
l0764:		ld	a,l
     		add	a,30h
     		call	conout
     		pop	bc
     		pop	de
     		ret

     		ld	de,0e927h
     		inc	bc
     		ld	h,l
     		nop
     		dec	bc
     		nop
;     		ld	bc,0af00h
		db	1, 0
		
l0777:		xor	a
     		ld	(067ah),a
;     		ld	a,0bh				;restore with 3ms no spinup
     		ld	a, RESTORE				;restore with 12ms spinup
     		call	cmdtout				;execute cmd
     		xor	04h
;     		and	84h
		and	04h
     		jp	nz,08e0h
     		ld	hl,0d00h			;build format code at 00d00h
     		ld	a,(0679h)
     		inc	a
     		ld	b,a
l078f:		ld	(hl),76h
     		inc	hl
     		ld	(hl),0edh
     		inc	hl
     		ld	(hl),0a3h
     		inc	hl
     		ld	(hl),20h
     		inc	hl
     		ld	(hl),0fbh
     		inc	hl
     		djnz	l078f		; (-11h)
		
     		ld	a,0c9h
     		ld	(hl),a
     		ld	(0066h),a
     		inc	hl
     		ld	(067ch),hl
     		xor	a
     		ld	(063ah),a
     		ld	(067ah),a
     		or	a
     		jr	z,l07c2		; (+0eh)
     		ld	a,STEPIN
     		call	cmdtout				;step-in, no-spinup, 3ms
     		ld	a,(067fh)
     		or	a
     		ld	a,STEPIN			;step-in, no-spinup, 3ms
     		call	nz,cmdtout
l07c2:		ld	a,(0687h)
     		ld	hl,063ah
     		bit	0,(hl)
     		jr	z,l07d0		; (+04h)
     		set	5,a
     		jr	l07d2		; (+02h)
l07d0:		res	5,a
l07d2:		out	(LATCH),a
     		ld	ix,(0673h)
     		ld	iy,09d6h
     		call	08eeh
     		ld	a,(067ah)
     		out	(TRKREG),a
     		xor	a
     		out	(56h),a
     		inc	a
     		out	(54h),a
     		ld	hl,(067ch)
     		di
     		ld	a,(063ah)
     		bit	0,a
     		ld	a,0f4h				;write track with settle time
     		jr	z,l07f9		; (+02h)
     		set	1,a
l07f9:		call	cmdout
     		ld	a,(0678h)
     		ld	b,a
     		ld	c,43h
     		call	0d00h
     		ei
     		call	waittout
     		ld	a,(0677h)
     		ld	b,a
     		ld	ix,09d6h
l0811:		call	readsector
     		bit	7,a
;     		jp	nz,08e0h
		nop
		nop
		nop
     		and	18h
     		jr	z,l0833		; (+16h)
     		call	force
     		call	readsector
     		bit	7,a
;     		jp	nz,08e0h
		nop
		nop
		nop
     		and	18h
     		jr	z,l0833		; (+07h)
     		ld	hl,(0636h)
     		inc	hl
     		ld	(0636h),hl
l0833:		inc	ix
     		djnz	l0811		; (-26h)
     		call	force
     		ld	d,20h
     		ld	a,(067eh)
     		and	03h
     		jr	z,l0848		; (+05h)
l0843:		sla	d
     		dec	a
     		jr	nz,l0843		; (-05h)
l0848:		ld	a,(0677h)
     		srl	a
     		adc	a,00h
     		ld	c,a
     		ld	hl,09d6h
l0853:		di
     		ld	b,d
     		ld	a,(hl)
     		inc	hl
     		inc	hl
     		out	(SECREG),a
     		ld	a,(063ah)
     		bit	0,a
     		ld	a,0a8h
     		jr	z,l0865		; (+02h)
     		set	1,a
l0865:		call	cmdout
     		ld	a,0e5h
l086a:		halt
     		out	(DATREG),a
     		halt
     		out	(DATREG),a
     		halt
     		out	(DATREG),a
     		halt
     		out	(DATREG),a
     		djnz	l086a		; (-0eh)
     		ei
     		call	waittout
     		dec	c
     		jr	nz,l0853		; (-2ch)
     		ld	a,(0677h)
     		srl	a
     		ld	c,a
     		ld	hl,09d7h
l0888:		di
     		ld	b,d
     		ld	a,(hl)
     		inc	hl
     		inc	hl
     		out	(SECREG),a
     		ld	a,(063ah)
     		bit	0,a
     		ld	a,0a8h
     		jr	z,l089a		; (+02h)
     		set	1,a
l089a:		call	cmdout
     		ld	a,0e5h
l089f:		halt
     		out	(DATREG),a
     		halt
     		out	(DATREG),a
     		halt
     		out	(DATREG),a
     		halt
     		out	(DATREG),a
     		djnz	l089f		; (-0eh)
     		ei
     		call	waittout
     		dec	c
     		jr	nz,l0888		; (-2ch)
     		xor	a
     		out	(54h),a
     		inc	a
     		out	(56h),a
     		call	force
     		call	prttrack
     		jr	nz,l08e0		; (+1eh)
     		ld	a,(0680h)
     		bit	7,a
     		jr	z,l08d4		; (+0bh)
     		ld	a,(063ah)
     		xor	01h
     		ld	(063ah),a
     		jp	nz,07c2h
l08d4:		ld	a,(067ah)
     		inc	a
     		ld	hl,0676h
     		cp	(hl)
     		jp	c,07aeh
     		xor	a
l08e0:		push	af
     		ld	a,(067ah)
     		out	(TRKREG),a
     		out	(DATREG),a
     		ld	a,10h				;FDC seek
     		out	(CMDREG),a
     		pop	af
     		ret

     		ld	a,(0677h)
     		ld	c,a
     		push	ix
     		pop	hl
     		ld	b,(hl)
     		inc	hl
     		ld	de,(067ch)
l08fb:		call	095ah
     		djnz	l08fb		; (-05h)
     		ld	b,(hl)
     		inc	hl
     		push	hl
     		pop	ix
l0905:		ld	a,(067ah)
     		ld	(ix+07h),a
     		ld	a,(063ah)
     		and	01h
     		ld	(ix+09h),a
     		ld	a,(iy+00h)
     		inc	iy
     		ld	(ix+0bh),a
     		push	ix
     		pop	hl
     		push	bc
l091f:		call	095ah
     		djnz	l091f		; (-05h)
     		ld	bc,0040h
     		ld	a,(ix+0dh)
     		ld	(067eh),a
     		and	03h
     		jr	z,l0938		; (+07h)
l0931:		sla	c
     		rl	b
     		dec	a
     		jr	nz,l0931		; (-07h)
l0938:		ld	a,6dh
     		ld	(de),a
     		inc	de
     		ld	a,0b6h
     		ld	(de),a
     		inc	de
     		dec	bc
     		ld	a,b
     		or	c
     		jr	nz,l0938		; (-0dh)
     		ld	b,(hl)
     		inc	hl
l0947:		call	095ah
     		djnz	l0947		; (-05h)
     		pop	bc
     		dec	c
     		jr	nz,l0905		; (-4bh)
     		ld	h,d
     		ld	l,e
     		inc	de
     		ld	bc,03e8h
     		ld	(hl),a
     		ldir
     		ret

     		push	bc
     		ld	b,(hl)
     		inc	hl
     		ld	a,(hl)
     		inc	hl
l095f:		ld	(de),a
     		inc	de
     		djnz	l095f		; (-04h)
     		pop	bc
     		ret

readsector:     ld	a,(ix+00h)
     		out	(SECREG),a
     		ld	a,(063ah)
     		bit	0,a
     		ld	a,READSEC			;read sector
     		jr	z,cmdtout	
     		set	1,a
cmdtout:	call	cmdout
waittout:     	push	bc
     		ld	bc,0000h
l097c:		ld	a,05h
     		call	cmdwait
     		in	a,(STSREG)
     		bit	0,a
     		jr	z,l098f	
     		dec	c
     		jr	nz,l097c
     		djnz	l097c	
     		ld	a,80h
     		or	a
l098f:		pop	bc
     		ret

cmdout:     	out	(CMDREG),a
     		ld	a,0eh
cmdwait:	dec	a
     		jr	nz,cmdwait		; (-03h)
     		ret

force:     	ld	a,FORCEINT			;force int!
     		call	cmdout
     		in	a,(STSREG)
     		ret

     		ld	de,09d6h
     		ld	b,h
     		xor	a
l09a6:		ld	(de),a
     		inc	de
     		djnz	l09a6		; (-04h)
     		ld	de,0000h
     		ld	c,01h
l09af:		ld	ix,09d6h
     		add	ix,de
     		ld	a,(ix+00h)
     		or	a
     		jr	z,l09c0		; (+05h)
     		call	09cfh
     		jr	l09af		; (-11h)
l09c0:		ld	(ix+00h),c
     		ld	b,l
l09c4:		call	09cfh
     		djnz	l09c4		; (-05h)
     		inc	c
     		ld	a,h
     		cp	c
     		jr	nc,l09af		; (-1fh)
     		ret

     		inc	e
     		ld	a,e
     		cp	h
     		ret	c

     		ld	e,00h
     		ret

     		ld	h,00h
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		inc	b
;     		jr	z,l0ad8		; (-01h)
		db	28h, 0ffh
     		ld	b,00h
     		ld	bc,1afch
     		rst	38h
     		dec	bc
     		inc	bc
     		nop
     		inc	bc
     		nop
     		ld	bc,01feh
     		nop
     		ld	bc,0100h
     		nop
     		ld	bc,0100h
     		rst	30h
     		dec	bc
     		rst	38h
     		ld	b,00h
     		ld	bc,02fbh
     		ld	bc,1bf7h
     		rst	38h
     		dec	b
     		ld	d,b
     		ld	c,(hl)
     		inc	c
     		nop
     		inc	bc
     		or	01h
     		call	m,4e32h
     		inc	c
     		inc	c
     		nop
     		inc	bc
     		push	af
     		ld	bc,01feh
     		nop
     		ld	bc,0100h
     		nop
     		ld	bc,0101h
     		rst	30h
     		ld	d,4eh
     		inc	c
     		nop
     		inc	bc
     		push	af
     		ld	bc,02fbh
     		ld	bc,35f7h
     		ld	c,(hl)
     		ld	bc,4e32h
     		inc	c
     		inc	c
     		nop
     		inc	bc
     		push	af
     		ld	bc,01feh
     		nop
     		ld	bc,0100h
     		nop
     		ld	bc,0102h
     		rst	30h
     		ld	d,4eh
     		inc	c
     		nop
     		inc	bc
     		push	af
     		ld	bc,02fbh
     		ld	bc,35f7h
     		ld	c,(hl)
     		ld	bc,4e32h
     		inc	c
     		inc	c
     		nop
     		inc	bc
     		push	af
     		ld	bc,01feh
     		nop
     		ld	bc,0100h
     		nop
     		ld	bc,0103h
     		rst	30h
     		ld	d,4eh
     		inc	c
     		nop
     		inc	bc
     		push	af
     		ld	bc,02fbh
     		ld	bc,28f7h
     		ld	c,(hl)
     		ld	bc,0ff10h
     		dec	bc
     		inc	bc
     		nop
     		inc	bc
     		nop
     		ld	bc,01feh
     		nop
     		ld	bc,0100h
     		nop
     		ld	bc,0100h
     		rst	30h
     		dec	bc
     		rst	38h
     		ld	b,00h
     		ld	bc,02fbh
     		ld	bc,08f7h
     		rst	38h
     		ld	bc,4e20h
     		inc	c
     		inc	c
     		nop
     		inc	bc
     		push	af
     		ld	bc,01feh
     		nop
     		ld	bc,0100h
     		nop
     		ld	bc,0101h
     		rst	30h
     		ld	d,4eh
     		inc	c
     		nop
     		inc	bc
     		push	af
     		ld	bc,02fbh
     		ld	bc,10f7h
     		ld	c,(hl)
     		ld	bc,4e20h
     		inc	c
     		inc	c
     		nop
     		inc	bc
     		push	af
     		ld	bc,01feh
     		nop
     		ld	bc,0100h
     		nop
     		ld	bc,0102h
     		rst	30h
     		ld	d,4eh
     		inc	c
     		nop
     		inc	bc
     		push	af
     		ld	bc,02fbh
     		ld	bc,18f7h
     		ld	c,(hl)
     		ld	bc,4e20h
     		inc	c
     		inc	c
     		nop
     		inc	bc
     		push	af
     		ld	bc,01feh
     		nop
     		ld	bc,0100h
     		nop
     		ld	bc,0103h
     		rst	30h
     		ld	d,4eh
     		inc	c
     		nop
     		inc	bc
     		push	af
     		ld	bc,02fbh
     		ld	bc,50f7h
     		ld	c,(hl)
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
     		nop
