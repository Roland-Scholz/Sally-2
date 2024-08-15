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
;chrin		equ	00185h
;chrout		equ	001a2h
;printhex	equ	00229h

		include "SALLY-CONST.asm"


		org	00000h
		
		jmp	start
		
;--------------------------------------------------
; conin T/C 0 (here counter) interrupt for start-bit
;--------------------------------------------------		
coninInt:	push	af
     		ld	a, 87h
     		out	(TC0), a			;set T/C 0 to 9600 Baud timer 
baud9600:     	ld	a, 1ah
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
coninIntPtr:    ld	(0ff00h), a			;store result in ff00
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
     		ld	(0ff12h), a
     		pop	af
     		ei
     		reti

conoutIntB:	push	af
		ld	a, 00h
     		out	(SIOOUT), a
     		rra
     		ld	(conoutIntC + 2), a
     		ld	a, conoutIntC & 255
     		ld	(0ff12h), a
     		pop	af
     		ei
     		reti

conoutIntC:	push	af
     		ld	a, 00h
     		out	(SIOOUT), a
     		rra
     		ld	(conoutIntD + 2), a
     		ld	a, conoutIntD & 255
     		ld	(0ff12h), a
     		pop	af
     		ei
     		reti

conoutIntD:	push	af
     		ld	a, 00h
     		out	(SIOOUT), a
     		rra
     		ld	(conoutIntE + 2), a
     		ld	a, conoutIntE & 255
     		ld	(0ff12h), a
     		pop	af
     		ei
     		reti

conoutIntE:	push	af
     		ld	a, 00h
     		out	(SIOOUT), a
     		rra
     		ld	(conoutIntF + 2), a
     		ld	a, conoutIntF & 255
     		ld	(0ff12h), a
     		pop	af
     		ei
     		reti

conoutIntF:	push	af
     		ld	a, 00h
     		out	(SIOOUT), a
     		rra
     		ld	(conoutIntG + 2), a
     		ld	a, conoutIntG & 255
     		ld	(0ff12h), a
     		pop	af
     		ei
     		reti

conoutIntG:	push	af
     		ld	a, 00h
     		out	(SIOOUT), a
     		rra
     		ld	(conoutIntH + 2), a
     		ld	a, conoutIntH & 255
     		ld	(0ff12h), a
     		pop	af
     		ei
     		reti

conoutIntH:	push	af
     		ld	a, 00h
     		out	(SIOOUT), a
     		rra
     		ld	(conoutIntI + 2), a
     		ld	a, conoutIntI & 255
     		ld	(0ff12h), a
     		pop	af
     		ei
     		reti

conoutIntI:	push	af
     		ld	a, 00h
     		out	(SIOOUT), a
     		ld	a, conoutIntJ & 255
     		ld	(0ff12h), a
     		pop	af
     		ei
     		reti

conoutIntJ:	push	af
     		ld	a, 01h				;stop bit
     		out	(SIOOUT), a
     		ld	a, resetConin & 255
     		ld	(0ff12h), a
     		pop	af
     		ei
     		reti

resetConin: 	push	af				;disable timer 1 interrupt
     		ld	a, 01h
     		out	(TC1), a
     		ld	a, 0ffh
     		ld	(0ff12h), a
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
		ld      b, 0bh              
portinit:	ld      c, (hl)             
		inc     hl                 
		outi   			 	 
		jr      nz, portinit			; loop

		ld      sp, 0f000h			; stack-pointer to TC0INTVEC

		ld	hl, 00000h			; source
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

		call	printstr
		db	"Rolli-2\n", 0
		
		ld	a, 50h				;reset FDC
		out	(DISKCTRL), a
		ld	a, 41h
		out	(DISKCTRL), a
		
		ld	a, 0d0h				;force int
		out	(FDCCMD), a
		call	fdcwait
			
seektrk0:	ld	a, 61h				;stepout
		out	(FDCCMD), a
		call	fdcwait
		in 	a, (FDCSTAT)
		bit	2, a				;trk00
		jr	z, seektrk0

		ld	b, 70
stepin:		ld	a, 043h				;stepin, 12ms
		out	(FDCCMD), a
		call	fdcwait
		djnz	stepin
		
loop:		call	fdcwait
		in	a, (FDCSTAT)
		bit	7, a
		jr	nz, loop
		
		ld	a, 40h
		out	(DISKCTRL), a
		
		call	conin
		jp	code0000
;--------------------------------------------------
; SALLY Monitor
;--------------------------------------------------		
;f3de
sallymon:     	;ld	a, 01h				
     		;out	(ROMSWITCH), a			;turn off ROM
     		;call	init9600			;init send/receive at 9600 baud (T/C 0 interrupt)
     		call	printstr
		db	"\r\n",  "SALLY1",  0

monloop:	ld	hl,  monloop			;loop here
     		push	hl
     		call	printstr
		db	"\r\n# ",  0 
		
f3fd:		call	getchar				;read from conin
     		cp	20h				;< 20h = control-character?
     		ret	c				;yes, return monloop

		call	putchar
		ret


		
fdcwait:	call	fdctime
		in 	a, (FDCSTAT)
		bit	0, a
		ret	z
		jr	fdcwait
		
fdctime:	ld	a, 10h
fdctime1:	dec	a
		jr	nz, fdctime1
		ret
		
		
puthex:		push	af
		push	af
		rrca
		rrca
		rrca
		rrca
		call	putnibble
		pop	af
		call	putnibble
		pop	af
		ret

putnibble:	and	0fh
		add	'0'
		cp	'9'+1
		jr	c, putnibble1
		add	7
putnibble1:	jp	putchar



		
		
printstr:    	ex	(sp), hl			;exchange (load) hl with stackpointer
printstr1:	ld	a, (hl)				;load char
     		call	putchar				;print it
     		ld	a, (hl)				;load it again
     		inc	hl				;increment hl
     		or	a				;char <> 0?
     		jr	nz, printstr1			;loop
     		ex	(sp), hl			;write hl back to stack
     		ret
		
putchar:	push	hl				;echo CONOUT
     		push	bc
     		push	af
     		ld	c, a
     		call	conout
     		pop	af
     		pop	bc
     		pop	hl
     		ret

getchar:     	push	hl
     		push	bc
     		call	conin				;CONIN in jumptable
     		pop	bc
f4b8:		pop	hl
;     		res	7, a				;reset parity bit
     		cp	20h				;char < 20h = control char?
     		ret	c				;yes, return

		
;--------------------------------------------------
; SALLY Monitor
;--------------------------------------------------		
init9600:	di
     		ld	hl, resetConin			;point Timer 1 interrupt (send)
     		ld	(0ff12h), hl			;to 0f5eeh
     		ld	a, 07h				;Timer 1 reset, no int
     		out	(TC1), a			;load time constant
     		ld	a, (baud9600 + 1)		;01ah = 9600 Baud constant from conin
     		out	(TC1), a	
     		ld	hl, 0ff00h	
     		ld	(coninIntPtr+1), hl	
     		ld	(coninPtr), hl	
     		di	
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
     		ld	(	TC0INTVEC), hl
     		ei
     		ret

coninPtr:	dw	0ff00h
		
;--------------------------------------------------
; const, is input from SIO ready?
;--------------------------------------------------		
;f635:
const:     	ld	hl, coninPtr
     		ld	a, (coninIntPtr + 1)
     		sub	(hl)
     		ret	z

     		ld	a, 0ffh
     		ret

;--------------------------------------------------
; conin
;--------------------------------------------------		
conin:		call	const				;0f635h
     		jr	z, conin			;input available?
     		ld	hl, (coninPtr)			;read from buffer
     		ld	a, (hl)				;increment buffer pointer
     		inc	l
     		res	4, l				;wrap at 16 bytes
     		ld	(coninPtr), hl
     		ret

;f650:
conout:		ld	a, (0ff12h)
     		cp	resetConin & 255
     		jr	c, conout			; (-07h)		
     		ld	a, c
;     		and	7fh
;    		jp	po, conout1			;set parity
;   		or	80h
conout1:     	ld	(conoutIntB + 2), a		;set byte to output
     		ld	a, conoutIntA & 255		;set conout interrupt
     		ld	(0ff12h), a
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
		db	057h, 001h			;Bit7	ATARI RXD
		db	030h, 040h			;DSE	Floppy Control (74LS273)
		db	040h, 0d0h			;DWR/DRW	FDC read-write	d0 = force int (with no interrupt)