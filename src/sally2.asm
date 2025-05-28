;
			SALLYBUILD	EQU	1
	ASEG
	ORG	0
;
;
;
;
NOHIGHSPEEDSIO	EQU	0
;
;--------------------------------------------------
; Track-Buffer 26*256 bytes
;--------------------------------------------------
TRKBUFFER	EQU	00800h

SIONORMAL	EQU	40
SIOFAST		EQU	8
;
;
		INCLUDE EQUS.MAC
;--------------------------------------------------
; Code executed after Reset
;--------------------------------------------------
		INCLUDE SYSINIT.MAC
;
;	PHASE TO RAM-RESIDENT PART OF PROGRAM
;
MONCOPY	EQU	$
	.PHASE	0F000H
;
;
MONITOR	EQU	$
		JP	RESTART
		JP	MINIMON		;MONITOR WARM ENTRY POINT
CSV:	JP	CONST		;CONSOLE STATUS
CIV:	JP	CONIN		;CONSOLE INPUT
COV:	JP	CONOUT		;CONSOLE OUTPUT
DISKV:	JP	DISKDVR		;DISK HANDLER
LISTV:	JP	CENTOUT		;PARALLEL PRINTER OUT
		JP	CENTRDY		;PARALLEL PRINTER STATUS
RENEW:	JP	CINIT2		;CONSOLE PORT INITAILZATION
;
RESTART:	
		DI
		XOR	A
		OUT	(BANKSW),A
		JP	0			;JUMP TO ROM
;
;
;
	INCLUDE	DISKIO.MAC
	INCLUDE	MINIMON.MAC
	INCLUDE	PRINTER.MAC
		DEFB 95h                ; **** Exists on original ROM? ****
IF SALLYBUILD
		dw	0, 0, 0, 0, 0, 0
ELSE
	defs	(($ and 0ff00h)+100h)-$
ENDIF	;SALLYBUILD
	INCLUDE	SERIAL.MAC
;
;	CODE PAST THIS POINT IS ONLY USED IN ATARI DISK MODE
;
	INCLUDE	BITBANG.MAC
	INCLUDE	ATARI.MAC
	INCLUDE	FORMAT.MAC
;
;
;
	.DEPHASE
MONSIZE	EQU	$-MONCOPY
;
;
varcopy	equ	$
	.phase	0ff00h+32
ram	equ	$-32		;put ram on start of 256 byte page
KEYBUF	equ	ram		;16 byte keyboard input fifo
CTCVEC	equ	ram+16	;8 word interrupt vector table
	INCLUDE GLOBAL.MAC	;PUT GLOBAL VARIABLES AT TOP OF RAM
;
	.DEPHASE
;
IF SALLYBUILD
;
;--------------------------------------------------
; set to zero in startup code until 0ffffh
;--------------------------------------------------
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		DB  'ROLLI 1 Rev 1.00'
ELSE
;
LAST	EQU	$

; **** Hack to make sure data fits in 4K ROM ****
	IF ($+16) GT (1000H)
		.ERROR /4K ROM BOUNDARY CROSSED/
	ELSE
		DEFS (1000H-16)-LAST
		DB  'ATR8000 ver 3.02'
	ENDIF
ENDIF	;SALLYBUILD
END
