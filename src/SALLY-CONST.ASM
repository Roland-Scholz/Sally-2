LF		equ	0ah
CR		equ	0dh

VARS		equ	0ff80h
echo		equ	0
sum		equ	1
addr		equ	2

;--------------------------------------------------
; baudrate for console-io
; 26 =  9600 baud
; 13 = 19200 baud
;--------------------------------------------------
CONBAUD		equ	26

;--------------------------------------------------
; 16 bytes keyboard buffer
;--------------------------------------------------
KEYBUF		equ	0ff00h

;--------------------------------------------------
; NMI Vector
;--------------------------------------------------
NMIVEC		equ	066h

;--------------------------------------------------
; FF10: Interrupt Table
;--------------------------------------------------
TC0INTVEC	equ	0ff10h
TC1INTVEC	equ	0ff12h
TC2INTVEC	equ	0ff14h
TC3INTVEC	equ	0ff16h

;--------------------------------------------------
; Out - ports equations, Disk-Control
;--------------------------------------------------
DISKCTRL	equ	030h				;7 = S/D\, 6 = S/B\, 5 = side, 4 = RES\, 3-0 = DSEL 4-1

;--------------------------------------------------
; In/Out - ports equations, WD177x FDC
;--------------------------------------------------
FDCSTAT		equ	040h
FDCCMD		equ	040h
FDCTRK		equ	041h
FDCSEC		equ	042h
FDCDAT		equ	043h

;--------------------------------------------------
; Out - ports equations, RS232, SIO, ROM, INDEX
;--------------------------------------------------
SIOOUT		equ	050h
RS232TX		equ	051h
ROMSWITCH	equ	052h
STROBE		equ	053h
RESIDX		equ	054h
RS232DTR	equ	055h
SETIDX		equ	056h
CMDSIO		equ	057h				;0 = CMD, 1 = SIOOUT

RS232		equ	050h
SIOIN		equ	070h

TC0		equ	080h
TC1		equ	081h
TC2		equ	082h
TC3		equ	083h

DDEVIC		equ	0c2fbh
DCOMND		equ	0c2fch
DAUX1		equ	0c2fdh
DAUX2		equ	0c2feh
DCHECK		equ	0c2ffh

PERCOMBLOCKS	equ	0ff56h