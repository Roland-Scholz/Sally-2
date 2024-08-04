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
chrout		equ	001a2h
printhex	equ	00229h

		org	0f000h
		
		jmp	start
		
;--------------------------------------------------
; conin T/C 0 (here counter) interrupt for start-bit
;--------------------------------------------------		
coninInt:	push	af
     		ld	a, 87h
     		out	(80h), a			;set T/C 0 to 9600 Baud timer 
baud9600:     	ld	a, 1ah
     		out	(80h), a
     		ld	a, coninIntB & 255		;set T/C 0 interrupt to next routine
     		ld	(0ff10h), a
     		ld	a, 7fh
coninInt1:	ld	(inbyte + 1), a
     		pop	af
     		ei
     		reti

coninIntB:    	push	af
     		in	a, (70h)
     		rla
inbyte:    	ld	a, 00h				;<- set by previous routine to 7fh
     		rra
     		jr	c, coninInt1			;loop 7 times
coninIntPtr:    ld	(0ff00h), a			;store result in ff00
     		ld	a, coninIntC & 255		;timer 0 interrupt vector = 0ff2c
		ld	(0ff10h), a
     		pop	af
     		ei
     		reti

coninIntC:     	push	af
     		ld	a, 0c7h				;timer 0 counter
     		out	(80h), a
     		ld	a, 01h
     		out	(80h), a
     		ld	a, (coninIntPtr + 1)
     		inc	a
     		and	0fh
     		ld	(coninIntPtr + 1), a		;increment buffer (16 bytes)
     		ld	a, coninInt & 255		;reset T/C 0 interrupt to start-bit routine
     		ld	(0ff10h), a
     		pop	af
     		ei
     		reti


;--------------------------------------------------
; conout T/C 1 interrupt for start-bit
;--------------------------------------------------		
conoutIntA:     push	af
     		xor	a
     		out	(50h), a
     		ld	a, conoutIntB & 255
     		ld	(0ff12h), a
     		pop	af
     		ei
     		reti

conoutIntB:	push	af
		ld	a, 00h
     		out	(50h), a
     		rra
     		ld	(conoutIntC + 2), a
     		ld	a, conoutIntC & 255
     		ld	(0ff12h), a
     		pop	af
     		ei
     		reti

conoutIntC:	push	af
     		ld	a, 00h
     		out	(50h), a
     		rra
     		ld	(conoutIntD + 2), a
     		ld	a, conoutIntD & 255
     		ld	(0ff12h), a
     		pop	af
     		ei
     		reti

conoutIntD:	push	af
     		ld	a, 00h
     		out	(50h), a
     		rra
     		ld	(conoutIntE + 2), a
     		ld	a, conoutIntE & 255
     		ld	(0ff12h), a
     		pop	af
     		ei
     		reti

conoutIntE:	push	af
     		ld	a, 00h
     		out	(50h), a
     		rra
     		ld	(conoutIntF + 2), a
     		ld	a, conoutIntF & 255
     		ld	(0ff12h), a
     		pop	af
     		ei
     		reti

conoutIntF:	push	af
     		ld	a, 00h
     		out	(50h), a
     		rra
     		ld	(conoutIntG + 2), a
     		ld	a, conoutIntG & 255
     		ld	(0ff12h), a
     		pop	af
     		ei
     		reti

conoutIntG:	push	af
     		ld	a, 00h
     		out	(50h), a
     		rra
     		ld	(conoutIntH + 2), a
     		ld	a, conoutIntH & 255
     		ld	(0ff12h), a
     		pop	af
     		ei
     		reti

conoutIntH:	push	af
     		ld	a, 00h
     		out	(50h), a
     		rra
     		ld	(conoutIntI + 2), a
     		ld	a, conoutIntI & 255
     		ld	(0ff12h), a
     		pop	af
     		ei
     		reti

conoutIntI:	push	af
     		ld	a, 00h
     		out	(50h), a
     		ld	a, conoutIntJ & 255
     		ld	(0ff12h), a
     		pop	af
     		ei
     		reti

conoutIntJ:	push	af
     		ld	a, 01h				;stop bit
     		out	(50h), a
     		ld	a, resetConin & 255
     		ld	(0ff12h), a
     		pop	af
     		ei
     		reti

resetConin: 	push	af				;disable timer 1 interrupt
     		ld	a, 01h
     		out	(81h), a
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

		ld      sp, 0ff00h			; stack-pointer to 0ff10h
		
		ld      a, 0ffh				; load interrupt-vector register
		call	printhex
		ld      i, a				; with 0ffh
		im      2				; enable interrupt mode 2 (vectored)
			
     		call	init9600			;init 9600 baud send/receive

		call	printstr
		db	"Rolli-1\n", 0
		call	conin
		
		xor	a
loop:		out	(30h), a
		inc	a
		call	time
		jp	loop

save:		db	0
		
time:		ld	bc, 02000h
		push	af
		push	bc
time1:		dec	bc
		ld	a, b
		or	c
		jr	nz, time1
		pop	bc
		pop	af
		ret
		
		
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


		
;--------------------------------------------------
; SALLY Monitor
;--------------------------------------------------		
init9600:	di
     		ld	hl, resetConin			;point Timer 1 interrupt (send)
     		ld	(0ff12h), hl			;to 0f5eeh
     		ld	a, 07h				;Timer 1 reset, no int
     		out	(81h), a			;load time constant
     		ld	a, (baud9600 + 1)		;01ah = 9600 Baud constant from conin
     		out	(81h), a	
     		ld	hl, 0ff00h	
     		ld	(coninIntPtr+1), hl	
     		ld	(coninPtr), hl	
     		di	
     		ld	a, 01h	
     		out	(57h), a			;enable SIO-Trig
init9600a:	ld	b, 7eh	
init9600c:	in	a, (70h)			;read SIO
     		rla					;D7 (RX) to carry
     		jr	nc, init9600a			;0? repeat
     		djnz	init9600c			;high for 126 loops?
     		ld	a, 0c7h				;T/C 0 interrupt on, counter mode, falling edge (start bit)
     		out	(80h), a	
     		ld	a, 01h				;T/C 0, count just 1 
     		out	(80h), a	
     		ld	hl, coninInt			;set T/C 0 int to 0f500h
     		ld	(0ff10h), hl
     		ei
     		ret

coninPtr:	dw	0ff00h
		
;--------------------------------------------------
; const, is input from SIO ready?
;--------------------------------------------------		
f635:
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
     		and	7fh
     		jp	po, conout1			;set parity
     		or	80h
conout1:     	ld	(conoutIntB + 2), a		;set byte to output
     		ld	a, conoutIntA & 255		;set conout interrupt
     		ld	(0ff12h), a
     		ld	a, 81h				;enable timer 1 int
     		out	(81h), a		
     		ret


;--------------------------------------------------
; 11 times port:value
;--------------------------------------------------
portval:	db	050h, 001h			;Bit0	set ATARI DATA
		db	050h, 001h			;Bit1	set RS232 TX		051h fake!
		db	080h, 003h			;CTC	Channel 0 reset
		db 	080h, 010h			;CTC	Channel 0 interrupt vector
		db	081h, 007h			;CTC	Channel 1 reset + set time constant
		db	081h, 001h			;CTC	Channel 1 time contant
		db	082h, 003h			;CTC	Channel 2 reset
		db	083h, 003h			;CTC	Channel 3 reset
		db	057h, 001h			;Bit7	ATARI RXD
		db	030h, 040h			;DSE	Floppy Control (74LS273)
		db	040h, 0d0h			;DWR/DRW	FDC read-write	d0 = force int (with no interrupt)