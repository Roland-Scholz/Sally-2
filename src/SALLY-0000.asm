
		include "SALLY-CONST.asm"

;--------------------------------------------------
; Code executed after Reset
;--------------------------------------------------
		ORG	00000h
;
		di      				; disable interrupt
		xor     a				; set a to zero
restime:	dec     a				; do 256 times nothing
		jr      nz, restime			; loop 
		
		ld      hl, portval			; init 11 ports with values at 0a3h
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
ramerr:		jr      nz, ramerr			; if error, loop forever
		djnz    testram1              
		dec     c                  
		jr      nz, testram1           
		add     a, a                
		jr      nz, testram2		   
			
		ld      hl, sallycode			; copy BIOS from $00b9 to $f000
		ld      de, 0f000h			; length efc
		ld      bc, 0efch        		   
		ldir                    		   
		ld      hl, sallycode + 0efch		; sallycode + 0fb5h - 0b9h ; copy from fb5 to $ff20
		ld      de, 0ff20h			; length $2F
		ld      bc, 002fh        		   
		ldir                    		   
							
		xor     a				; fill up to $FFFF with zeros
ramfill:	ld      (de),a             
		inc     e                  
		jr      nz, ramfill           
					
		ld      sp, 0ff10h			; stack-pointer to 0ff10h
			
		ld      a, 0ffh				; load interrupt-vector register
		ld      i, a				; with 0ffh
		im      2				; enable interrupt mode 2 (vectored)	
							
;--------------------------------------------------
; step 5 times in and then out to trk00
; set bit 6 for each online floppy in ff5eh
; percom block (16 bytes, byte 8 bit 6)
;--------------------------------------------------
		ld      a, 4fh				; select drive 1-4, Motor off, side 0, B/S=1, DD
		out     (DISKCTRL),a         		   
							
		ld      d, a				; d = 4fh
		ld      b, 5				; step 5 times
stepin:		
		ld      a, 4bh				; 4b = 0100 1011 = step-in 
		call    0f36bh				; write A to FDC command and wait
		djnz    stepin          		    
	
		ld      b, 64h				; seek track 00 for all 4 drives
outloop1:	ld      a, d				; select all drives
		out     (DISKCTRL),a         		   
		ex      (sp),hl				; ???
		ex      (sp),hl				; ???
		ld      a, 6bh				; step out
		call    0f36bh				; write A to FDC command and wait
							
		ld      e, 01h          		    
outloop:	ld      a, e            		    
		or      40h     		
		out     (DISKCTRL), a			; select one drive
		ex      (sp), hl        		    
		ex      (sp), hl        		    
		call    0f391h				; stop command, get status
		bit     2, a            		    
		jr      z, excldrv			; bit not set, not at track 00
		ld      a, e				; drive at track 00
		cpl     				; exclude drive from seeking
		and     d               		   
		ld      d, a            		    
excldrv:		
		sla     e               		   
		bit     4, e				  
		jr      z, outloop			; status checked for all 4 drives?
		djnz    outloop1			; step out again
								
		call    0f068h				; deselect floppies and seek current track?
			
		ld      hl, 0ff5eh			; set bit for each connected floppy?
		ld      bc, 0010h       	    
		ld      a, 04h          	    
nextdrv:	rr      d               	   
		jr      c, noset        	   
		set     6, (hl)         	    
noset:		add     hl, bc          	    
		dec     a               	   
		jr      nz, nextdrv     	      
						
		ld      sp, 0c100h			; set stack to 0c100h
;		jp      0f762h				; jump to code in DRAM
		jp	0f003h
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
		db	030h, 000h			;DSE	Floppy Control (74LS273)
		db	040h, 0d0h			;DWR/DRW	FDC read-write	d0 = force int (with no interrupt)
		
sallycode	equ	ASMPC
