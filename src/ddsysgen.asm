; Disassembly of the file "C:\github\Sally-1\ATR8000-CPM\ddsysgen.com"
; 
; CPU Type: Z80
; 
; Created with dZ80 2.0
; 
; on Monday, 26 of August 2024 at 07:51 PM
; 
0000 c30401    jp      0104h
0003 d431c1    call    nc,0c131h
0006 0d        dec     c
0007 2a0600    ld      hl,(0006h)
000a 01e0be    ld      bc,0bee0h
000d b7        or      a
000e ed42      sbc     hl,bc
0010 da0000    jp      c,0000h
0013 cd560a    call    0a56h
0016 0d        dec     c
0017 0a        ld      a,(bc)
0018 0a        ld      a,(bc)
0019 41        ld      b,c
001a 54        ld      d,h
001b 52        ld      d,d
001c 3830      jr      c,004eh          ; (+30h)
001e 3030      jr      nc,0050h         ; (+30h)
0020 2053      jr      nz,0075h         ; (+53h)
0022 79        ld      a,c
0023 73        ld      (hl),e
0024 74        ld      (hl),h
0025 65        ld      h,l
0026 6d        ld      l,l
0027 2047      jr      nz,0070h         ; (+47h)
0029 65        ld      h,l
002a 6e        ld      l,(hl)
002b 65        ld      h,l
002c 72        ld      (hl),d
002d 61        ld      h,c
002e 74        ld      (hl),h
002f 6f        ld      l,a
0030 72        ld      (hl),d
0031 2076      jr      nz,00a9h         ; (+76h)
0033 65        ld      h,l
0034 72        ld      (hl),d
0035 73        ld      (hl),e
0036 2032      jr      nz,006ah         ; (+32h)
0038 2e31      ld      l,31h
003a 0d        dec     c
003b 0a        ld      a,(bc)
003c 2d        dec     l
003d 2d        dec     l
003e 2d        dec     l
003f 2d        dec     l
0040 2d        dec     l
0041 2d        dec     l
0042 2d        dec     l
0043 2d        dec     l
0044 2d        dec     l
0045 2d        dec     l
0046 2d        dec     l
0047 2d        dec     l
0048 2d        dec     l
0049 2d        dec     l
004a 2d        dec     l
004b 2d        dec     l
004c 2d        dec     l
004d 2d        dec     l
004e 2d        dec     l
004f 2d        dec     l
0050 2d        dec     l
0051 2d        dec     l
0052 2d        dec     l
0053 2d        dec     l
0054 2d        dec     l
0055 2d        dec     l
0056 2d        dec     l
0057 2d        dec     l
0058 2d        dec     l
0059 2d        dec     l
005a 2d        dec     l
005b 2d        dec     l
005c 2d        dec     l
005d 0d        dec     c
005e 0a        ld      a,(bc)
005f 00        nop     
0060 218000    ld      hl,0080h
0063 11be02    ld      de,02beh
0066 010300    ld      bc,0003h
0069 1a        ld      a,(de)
006a be        cp      (hl)
006b c2c102    jp      nz,02c1h
006e eda0      ldi     
0070 ea6901    jp      pe,0169h
0073 e5        push    hl
0074 fde1      pop     iy
0076 23        inc     hl
0077 23        inc     hl
0078 360d      ld      (hl),0dh
007a cddd08    call    08ddh
007d 7d        ld      a,l
007e 320301    ld      (0103h),a
0081 cdea04    call    04eah
0084 c20000    jp      nz,0000h
0087 cd560a    call    0a56h
008a 0d        dec     c
008b 0a        ld      a,(bc)
008c 0a        ld      a,(bc)
008d 72        ld      (hl),d
008e 65        ld      h,l
008f 61        ld      h,c
0090 64        ld      h,h
0091 79        ld      a,c
0092 2074      jr      nz,0108h         ; (+74h)
0094 6f        ld      l,a
0095 2063      jr      nz,00fah         ; (+63h)
0097 72        ld      (hl),d
0098 65        ld      h,l
0099 61        ld      h,c
009a 74        ld      (hl),h
009b 65        ld      h,l
009c 2062      jr      nz,0100h         ; (+62h)
009e 6f        ld      l,a
009f 6f        ld      l,a
00a0 74        ld      (hl),h
00a1 61        ld      h,c
00a2 62        ld      h,d
00a3 6c        ld      l,h
00a4 65        ld      h,l
00a5 2073      jr      nz,011ah         ; (+73h)
00a7 79        ld      a,c
00a8 73        ld      (hl),e
00a9 74        ld      (hl),h
00aa 65        ld      h,l
00ab 6d        ld      l,l
00ac 2064      jr      nz,0112h         ; (+64h)
00ae 69        ld      l,c
00af 73        ld      (hl),e
00b0 6b        ld      l,e
00b1 2873      jr      z,0126h          ; (+73h)
00b3 29        add     hl,hl
00b4 0d        dec     c
00b5 0a        ld      a,(bc)
00b6 2d        dec     l
00b7 2d        dec     l
00b8 2d        dec     l
00b9 2d        dec     l
00ba 2d        dec     l
00bb 2d        dec     l
00bc 2d        dec     l
00bd 2d        dec     l
00be 2d        dec     l
00bf 2d        dec     l
00c0 2d        dec     l
00c1 2d        dec     l
00c2 2d        dec     l
00c3 2d        dec     l
00c4 2d        dec     l
00c5 2d        dec     l
00c6 2d        dec     l
00c7 2d        dec     l
00c8 2d        dec     l
00c9 2d        dec     l
00ca 2d        dec     l
00cb 2d        dec     l
00cc 2d        dec     l
00cd 2d        dec     l
00ce 2d        dec     l
00cf 2d        dec     l
00d0 2d        dec     l
00d1 2d        dec     l
00d2 2d        dec     l
00d3 2d        dec     l
00d4 2d        dec     l
00d5 2d        dec     l
00d6 2d        dec     l
00d7 2d        dec     l
00d8 2d        dec     l
00d9 2d        dec     l
00da 2d        dec     l
00db 2d        dec     l
00dc 2d        dec     l
00dd 0d        dec     c
00de 0a        ld      a,(bc)
00df 0a        ld      a,(bc)
00e0 69        ld      l,c
00e1 6e        ld      l,(hl)
00e2 73        ld      (hl),e
00e3 65        ld      h,l
00e4 72        ld      (hl),d
00e5 74        ld      (hl),h
00e6 2061      jr      nz,0149h         ; (+61h)
00e8 2066      jr      nz,0150h         ; (+66h)
00ea 6f        ld      l,a
00eb 72        ld      (hl),d
00ec 6d        ld      l,l
00ed 61        ld      h,c
00ee 74        ld      (hl),h
00ef 74        ld      (hl),h
00f0 65        ld      h,l
00f1 64        ld      h,h
00f2 2064      jr      nz,0158h         ; (+64h)
00f4 69        ld      l,c
00f5 73        ld      (hl),e
00f6 6b        ld      l,e
00f7 2074      jr      nz,016dh         ; (+74h)
00f9 68        ld      l,b
00fa 61        ld      h,c
00fb 74        ld      (hl),h
00fc 2079      jr      nz,0177h         ; (+79h)
00fe 6f        ld      l,a
00ff 75        ld      (hl),l
0100 2077      jr      nz,0179h         ; (+77h)
0102 69        ld      l,c
0103 73        ld      (hl),e
0104 68        ld      l,b
0105 0d        dec     c
0106 0a        ld      a,(bc)
0107 74        ld      (hl),h
0108 6f        ld      l,a
0109 2077      jr      nz,0182h         ; (+77h)
010b 72        ld      (hl),d
010c 69        ld      l,c
010d 74        ld      (hl),h
010e 65        ld      h,l
010f 2043      jr      nz,0154h         ; (+43h)
0111 50        ld      d,b
0112 2f        cpl     
0113 4d        ld      c,l
0114 206f      jr      nz,0185h         ; (+6fh)
0116 6e        ld      l,(hl)
0117 74        ld      (hl),h
0118 6f        ld      l,a
0119 2069      jr      nz,0184h         ; (+69h)
011b 6e        ld      l,(hl)
011c 2064      jr      nz,0182h         ; (+64h)
011e 72        ld      (hl),d
011f 69        ld      l,c
0120 76        halt    
0121 65        ld      h,l
0122 2041      jr      nz,0165h         ; (+41h)
0124 3a2061    ld      a,(6120h)
0127 6e        ld      l,(hl)
0128 64        ld      h,h
0129 0d        dec     c
012a 0a        ld      a,(bc)
012b 00        nop     
012c af        xor     a
012d 323f0d    ld      (0d3fh),a
0130 3e02      ld      a,02h
0132 cd180b    call    0b18h
0135 20f9      jr      nz,0130h         ; (-07h)
0137 213d02    ld      hl,023dh
013a 220100    ld      (0001h),hl
013d 31c10d    ld      sp,0dc1h
0140 cd560a    call    0a56h
0143 0d        dec     c
0144 0a        ld      a,(bc)
0145 0a        ld      a,(bc)
0146 73        ld      (hl),e
0147 79        ld      a,c
0148 73        ld      (hl),e
0149 74        ld      (hl),h
014a 65        ld      h,l
014b 6d        ld      l,l
014c 2077      jr      nz,01c5h         ; (+77h)
014e 72        ld      (hl),d
014f 69        ld      l,c
0150 74        ld      (hl),h
0151 74        ld      (hl),h
0152 65        ld      h,l
0153 6e        ld      l,(hl)
0154 2061      jr      nz,01b7h         ; (+61h)
0156 6e        ld      l,(hl)
0157 64        ld      h,h
0158 2076      jr      nz,01d0h         ; (+76h)
015a 65        ld      h,l
015b 72        ld      (hl),d
015c 69        ld      l,c
015d 66        ld      h,(hl)
015e 69        ld      l,c
015f 65        ld      h,l
0160 64        ld      h,h
0161 206f      jr      nz,01d2h         ; (+6fh)
0163 6b        ld      l,e
0164 2e0d      ld      l,0dh
0166 0a        ld      a,(bc)
0167 70        ld      (hl),b
0168 72        ld      (hl),d
0169 65        ld      h,l
016a 73        ld      (hl),e
016b 73        ld      (hl),e
016c 2061      jr      nz,01cfh         ; (+61h)
016e 6e        ld      l,(hl)
016f 79        ld      a,c
0170 206b      jr      nz,01ddh         ; (+6bh)
0172 65        ld      h,l
0173 79        ld      a,c
0174 2074      jr      nz,01eah         ; (+74h)
0176 6f        ld      l,a
0177 2072      jr      nz,01ebh         ; (+72h)
0179 65        ld      h,l
017a 70        ld      (hl),b
017b 65        ld      h,l
017c 61        ld      h,c
017d 74        ld      (hl),h
017e 2061      jr      nz,01e1h         ; (+61h)
0180 67        ld      h,a
0181 61        ld      h,c
0182 69        ld      l,c
0183 6e        ld      l,(hl)
0184 206f      jr      nz,01f5h         ; (+6fh)
0186 72        ld      (hl),d
0187 0d        dec     c
0188 0a        ld      a,(bc)
0189 72        ld      (hl),d
018a 65        ld      h,l
018b 73        ld      (hl),e
018c 65        ld      h,l
018d 74        ld      (hl),h
018e 2074      jr      nz,0204h         ; (+74h)
0190 68        ld      l,b
0191 65        ld      h,l
0192 2041      jr      nz,01d5h         ; (+41h)
0194 54        ld      d,h
0195 52        ld      d,d
0196 3830      jr      c,01c8h          ; (+30h)
0198 3030      jr      nc,01cah         ; (+30h)
019a 2061      jr      nz,01fdh         ; (+61h)
019c 6e        ld      l,(hl)
019d 64        ld      h,h
019e 2072      jr      nz,0212h         ; (+72h)
01a0 65        ld      h,l
01a1 2d        dec     l
01a2 62        ld      h,d
01a3 6f        ld      l,a
01a4 6f        ld      l,a
01a5 74        ld      (hl),h
01a6 0d        dec     c
01a7 0a        ld      a,(bc)
01a8 43        ld      b,e
01a9 50        ld      d,b
01aa 2f        cpl     
01ab 4d        ld      c,l
01ac 202e      jr      nz,01dch         ; (+2eh)
01ae 2e2e      ld      l,2eh
01b0 2000      jr      nz,01b2h         ; (+00h)
01b2 cdeb09    call    09ebh
01b5 cd560a    call    0a56h
01b8 0d        dec     c
01b9 0a        ld      a,(bc)
01ba 00        nop     
01bb c33002    jp      0230h
01be 04        inc     b
01bf 2030      jr      nz,01f1h         ; (+30h)
01c1 cd560a    call    0a56h
01c4 0d        dec     c
01c5 0a        ld      a,(bc)
01c6 2020      jr      nz,01e8h         ; (+20h)
01c8 2066      jr      nz,0230h         ; (+66h)
01ca 75        ld      (hl),l
01cb 6e        ld      l,(hl)
01cc 63        ld      h,e
01cd 74        ld      (hl),h
01ce 69        ld      l,c
01cf 6f        ld      l,a
01d0 6e        ld      l,(hl)
01d1 73        ld      (hl),e
01d2 2061      jr      nz,0235h         ; (+61h)
01d4 76        halt    
01d5 61        ld      h,c
01d6 69        ld      l,c
01d7 6c        ld      l,h
01d8 61        ld      h,c
01d9 62        ld      h,d
01da 6c        ld      l,h
01db 65        ld      h,l
01dc 3a0d0a    ld      a,(0a0dh)
01df 0a        ld      a,(bc)
01e0 312920    ld      sp,2029h
01e3 52        ld      d,d
01e4 65        ld      h,l
01e5 61        ld      h,c
01e6 64        ld      h,h
01e7 2073      jr      nz,025ch         ; (+73h)
01e9 79        ld      a,c
01ea 73        ld      (hl),e
01eb 74        ld      (hl),h
01ec 65        ld      h,l
01ed 6d        ld      l,l
01ee 2074      jr      nz,0264h         ; (+74h)
01f0 72        ld      (hl),d
01f1 61        ld      h,c
01f2 63        ld      h,e
01f3 6b        ld      l,e
01f4 73        ld      (hl),e
01f5 2066      jr      nz,025dh         ; (+66h)
01f7 72        ld      (hl),d
01f8 6f        ld      l,a
01f9 6d        ld      l,l
01fa 2064      jr      nz,0260h         ; (+64h)
01fc 69        ld      l,c
01fd 73        ld      (hl),e
01fe 6b        ld      l,e
01ff 2e0d      ld      l,0dh
0201 0a        ld      a,(bc)
0202 322920    ld      (2029h),a
0205 57        ld      d,a
0206 72        ld      (hl),d
0207 69        ld      l,c
0208 74        ld      (hl),h
0209 65        ld      h,l
020a 2073      jr      nz,027fh         ; (+73h)
020c 79        ld      a,c
020d 73        ld      (hl),e
020e 74        ld      (hl),h
020f 65        ld      h,l
0210 6d        ld      l,l
0211 2074      jr      nz,0287h         ; (+74h)
0213 72        ld      (hl),d
0214 61        ld      h,c
0215 63        ld      h,e
0216 6b        ld      l,e
0217 73        ld      (hl),e
0218 2074      jr      nz,028eh         ; (+74h)
021a 6f        ld      l,a
021b 2064      jr      nz,0281h         ; (+64h)
021d 69        ld      l,c
021e 73        ld      (hl),e
021f 6b        ld      l,e
0220 2e0d      ld      l,0dh
0222 0a        ld      a,(bc)
0223 33        inc     sp
0224 29        add     hl,hl
0225 2047      jr      nz,026eh         ; (+47h)
0227 65        ld      h,l
0228 6e        ld      l,(hl)
0229 65        ld      h,l
022a 72        ld      (hl),d
022b 61        ld      h,c
022c 74        ld      (hl),h
022d 65        ld      h,l
022e 2063      jr      nz,0293h         ; (+63h)
0230 75        ld      (hl),l
0231 73        ld      (hl),e
0232 74        ld      (hl),h
0233 6f        ld      l,a
0234 6d        ld      l,l
0235 69        ld      l,c
0236 7a        ld      a,d
0237 65        ld      h,l
0238 64        ld      h,h
0239 2043      jr      nz,027eh         ; (+43h)
023b 50        ld      d,b
023c 2f        cpl     
023d 4d        ld      c,l
023e 200d      jr      nz,024dh         ; (+0dh)
0240 0a        ld      a,(bc)
0241 00        nop     
0242 cd560a    call    0a56h
0245 0d        dec     c
0246 0a        ld      a,(bc)
0247 65        ld      h,l
0248 6e        ld      l,(hl)
0249 74        ld      (hl),h
024a 65        ld      h,l
024b 72        ld      (hl),d
024c 206e      jr      nz,02bch         ; (+6eh)
024e 75        ld      (hl),l
024f 6d        ld      l,l
0250 62        ld      h,d
0251 65        ld      h,l
0252 72        ld      (hl),d
0253 206f      jr      nz,02c4h         ; (+6fh)
0255 66        ld      h,(hl)
0256 2066      jr      nz,02beh         ; (+66h)
0258 75        ld      (hl),l
0259 6e        ld      l,(hl)
025a 63        ld      h,e
025b 74        ld      (hl),h
025c 69        ld      l,c
025d 6f        ld      l,a
025e 6e        ld      l,(hl)
025f 2028      jr      nz,0289h         ; (+28h)
0261 312c32    ld      sp,322ch
0264 206f      jr      nz,02d5h         ; (+6fh)
0266 72        ld      (hl),d
0267 2033      jr      nz,029ch         ; (+33h)
0269 29        add     hl,hl
026a 200d      jr      nz,0279h         ; (+0dh)
026c 0a        ld      a,(bc)
026d 6f        ld      l,a
026e 72        ld      (hl),d
026f 2070      jr      nz,02e1h         ; (+70h)
0271 72        ld      (hl),d
0272 65        ld      h,l
0273 73        ld      (hl),e
0274 73        ld      (hl),e
0275 203c      jr      nz,02b3h         ; (+3ch)
0277 43        ld      b,e
0278 54        ld      d,h
0279 4c        ld      c,h
027a 2d        dec     l
027b 43        ld      b,e
027c 3e20      ld      a,20h
027e 74        ld      (hl),h
027f 6f        ld      l,a
0280 2065      jr      nz,02e7h         ; (+65h)
0282 78        ld      a,b
0283 69        ld      l,c
0284 74        ld      (hl),h
0285 2074      jr      nz,02fbh         ; (+74h)
0287 6f        ld      l,a
0288 2043      jr      nz,02cdh         ; (+43h)
028a 50        ld      d,b
028b 2f        cpl     
028c 4d        ld      c,l
028d 202e      jr      nz,02bdh         ; (+2eh)
028f 2e2e      ld      l,2eh
0291 2000      jr      nz,0293h         ; (+00h)
0293 cd0b0a    call    0a0bh
0296 fe31      cp      31h
0298 2006      jr      nz,02a0h         ; (+06h)
029a cdcd0a    call    0acdh
029d c3dd03    jp      03ddh
02a0 fe32      cp      32h
02a2 2006      jr      nz,02aah         ; (+06h)
02a4 cd7e0a    call    0a7eh
02a7 c3dd03    jp      03ddh
02aa fe33      cp      33h
02ac c2b503    jp      nz,03b5h
02af cd6e04    call    046eh
02b2 c3dd03    jp      03ddh
02b5 fe58      cp      58h
02b7 ca340a    jp      z,0a34h
02ba cd560a    call    0a56h
02bd 0d        dec     c
02be 0a        ld      a,(bc)
02bf 69        ld      l,c
02c0 6e        ld      l,(hl)
02c1 76        halt    
02c2 61        ld      h,c
02c3 6c        ld      l,h
02c4 69        ld      l,c
02c5 64        ld      h,h
02c6 2063      jr      nz,032bh         ; (+63h)
02c8 6f        ld      l,a
02c9 6d        ld      l,l
02ca 6d        ld      l,l
02cb 61        ld      h,c
02cc 6e        ld      l,(hl)
02cd 64        ld      h,h
02ce 2c        inc     l
02cf 2074      jr      nz,0345h         ; (+74h)
02d1 72        ld      (hl),d
02d2 79        ld      a,c
02d3 2061      jr      nz,0336h         ; (+61h)
02d5 67        ld      h,a
02d6 61        ld      h,c
02d7 69        ld      l,c
02d8 6e        ld      l,(hl)
02d9 00        nop     
02da c34203    jp      0342h
02dd b7        or      a
02de 2836      jr      z,0316h          ; (+36h)
02e0 cd560a    call    0a56h
02e3 0d        dec     c
02e4 0a        ld      a,(bc)
02e5 0a        ld      a,(bc)
02e6 70        ld      (hl),b
02e7 72        ld      (hl),d
02e8 65        ld      h,l
02e9 73        ld      (hl),e
02ea 73        ld      (hl),e
02eb 2061      jr      nz,034eh         ; (+61h)
02ed 6e        ld      l,(hl)
02ee 79        ld      a,c
02ef 206b      jr      nz,035ch         ; (+6bh)
02f1 65        ld      h,l
02f2 79        ld      a,c
02f3 2074      jr      nz,0369h         ; (+74h)
02f5 6f        ld      l,a
02f6 2072      jr      nz,036ah         ; (+72h)
02f8 65        ld      h,l
02f9 74        ld      (hl),h
02fa 75        ld      (hl),l
02fb 72        ld      (hl),d
02fc 6e        ld      l,(hl)
02fd 2074      jr      nz,0373h         ; (+74h)
02ff 6f        ld      l,a
0300 206d      jr      nz,036fh         ; (+6dh)
0302 61        ld      h,c
0303 69        ld      l,c
0304 6e        ld      l,(hl)
0305 206d      jr      nz,0374h         ; (+6dh)
0307 65        ld      h,l
0308 6e        ld      l,(hl)
0309 75        ld      (hl),l
030a 202e      jr      nz,033ah         ; (+2eh)
030c 2e2e      ld      l,2eh
030e 2000      jr      nz,0310h         ; (+00h)
0310 cdeb09    call    09ebh
0313 c30401    jp      0104h
0316 cd560a    call    0a56h
0319 0d        dec     c
031a 0a        ld      a,(bc)
031b 0a        ld      a,(bc)
031c 66        ld      h,(hl)
031d 75        ld      (hl),l
031e 6e        ld      l,(hl)
031f 63        ld      h,e
0320 74        ld      (hl),h
0321 69        ld      l,c
0322 6f        ld      l,a
0323 6e        ld      l,(hl)
0324 2063      jr      nz,0389h         ; (+63h)
0326 6f        ld      l,a
0327 6d        ld      l,l
0328 70        ld      (hl),b
0329 6c        ld      l,h
032a 65        ld      h,l
032b 74        ld      (hl),h
032c 65        ld      h,l
032d 202e      jr      nz,035dh         ; (+2eh)
032f 2e2e      ld      l,2eh
0331 2000      jr      nz,0333h         ; (+00h)
0333 21e803    ld      hl,03e8h
0336 0600      ld      b,00h
0338 10fe      djnz    0338h            ; (-02h)
033a 2b        dec     hl
033b 7c        ld      a,h
033c b5        or      l
033d 20f7      jr      nz,0336h         ; (-09h)
033f cd560a    call    0a56h
0342 0d        dec     c
0343 0a        ld      a,(bc)
0344 0a        ld      a,(bc)
0345 00        nop     
0346 c30401    jp      0104h
0349 00        nop     
034a 015359    ld      bc,5953h
034d 53        ld      d,e
034e 54        ld      d,h
034f 45        ld      b,l
0350 4d        ld      c,l
0351 2020      jr      nz,0373h         ; (+20h)
0353 53        ld      d,e
0354 57        ld      d,a
0355 50        ld      d,b
0356 00        nop     
0357 00        nop     
0358 00        nop     
0359 00        nop     
035a 00        nop     
035b 00        nop     
035c 00        nop     
035d 00        nop     
035e 00        nop     
035f 00        nop     
0360 00        nop     
0361 00        nop     
0362 00        nop     
0363 00        nop     
0364 00        nop     
0365 00        nop     
0366 00        nop     
0367 00        nop     
0368 00        nop     
0369 00        nop     
036a 00        nop     
036b 00        nop     
036c 00        nop     
036d 00        nop     
036e cd560a    call    0a56h
0371 0d        dec     c
0372 0a        ld      a,(bc)
0373 0a        ld      a,(bc)
0374 2d        dec     l
0375 2d        dec     l
0376 2d        dec     l
0377 2047      jr      nz,03c0h         ; (+47h)
0379 65        ld      h,l
037a 6e        ld      l,(hl)
037b 65        ld      h,l
037c 72        ld      (hl),d
037d 61        ld      h,c
037e 74        ld      (hl),h
037f 65        ld      h,l
0380 2043      jr      nz,03c5h         ; (+43h)
0382 75        ld      (hl),l
0383 73        ld      (hl),e
0384 74        ld      (hl),h
0385 6f        ld      l,a
0386 6d        ld      l,l
0387 2073      jr      nz,03fch         ; (+73h)
0389 79        ld      a,c
038a 73        ld      (hl),e
038b 74        ld      (hl),h
038c 65        ld      h,l
038d 6d        ld      l,l
038e 202d      jr      nz,03bdh         ; (+2dh)
0390 2d        dec     l
0391 2d        dec     l
0392 0d        dec     c
0393 0a        ld      a,(bc)
0394 0a        ld      a,(bc)
0395 69        ld      l,c
0396 6e        ld      l,(hl)
0397 73        ld      (hl),e
0398 65        ld      h,l
0399 72        ld      (hl),d
039a 74        ld      (hl),h
039b 2064      jr      nz,0401h         ; (+64h)
039d 69        ld      l,c
039e 73        ld      (hl),e
039f 6b        ld      l,e
03a0 2077      jr      nz,0419h         ; (+77h)
03a2 69        ld      l,c
03a3 74        ld      (hl),h
03a4 68        ld      l,b
03a5 2053      jr      nz,03fah         ; (+53h)
03a7 59        ld      e,c
03a8 53        ld      d,e
03a9 54        ld      d,h
03aa 45        ld      b,l
03ab 4d        ld      c,l
03ac 2e53      ld      l,53h
03ae 57        ld      d,a
03af 50        ld      d,b
03b0 2066      jr      nz,0418h         ; (+66h)
03b2 69        ld      l,c
03b3 6c        ld      l,h
03b4 65        ld      h,l
03b5 0d        dec     c
03b6 0a        ld      a,(bc)
03b7 69        ld      l,c
03b8 6e        ld      l,(hl)
03b9 74        ld      (hl),h
03ba 6f        ld      l,a
03bb 2064      jr      nz,0421h         ; (+64h)
03bd 72        ld      (hl),d
03be 69        ld      l,c
03bf 76        halt    
03c0 65        ld      h,l
03c1 2041      jr      nz,0404h         ; (+41h)
03c3 2061      jr      nz,0426h         ; (+61h)
03c5 6e        ld      l,(hl)
03c6 64        ld      h,h
03c7 2070      jr      nz,0439h         ; (+70h)
03c9 72        ld      (hl),d
03ca 65        ld      h,l
03cb 73        ld      (hl),e
03cc 73        ld      (hl),e
03cd 203c      jr      nz,040bh         ; (+3ch)
03cf 52        ld      d,d
03d0 45        ld      b,l
03d1 54        ld      d,h
03d2 55        ld      d,l
03d3 52        ld      d,d
03d4 4e        ld      c,(hl)
03d5 3e20      ld      a,20h
03d7 2e2e      ld      l,2eh
03d9 2e20      ld      l,20h
03db 00        nop     
03dc cdeb09    call    09ebh
03df fe03      cp      03h
03e1 ca340a    jp      z,0a34h
03e4 cd560a    call    0a56h
03e7 0d        dec     c
03e8 0a        ld      a,(bc)
03e9 00        nop     
03ea cd560a    call    0a56h
03ed 0d        dec     c
03ee 0a        ld      a,(bc)
03ef 77        ld      (hl),a
03f0 6f        ld      l,a
03f1 72        ld      (hl),d
03f2 6b        ld      l,e
03f3 69        ld      l,c
03f4 6e        ld      l,(hl)
03f5 67        ld      h,a
03f6 2000      jr      nz,03f8h         ; (+00h)
03f8 0e0d      ld      c,0dh
03fa cd0500    call    0005h
03fd 114a04    ld      de,044ah
0400 0e0f      ld      c,0fh
0402 cd0500    call    0005h
0405 feff      cp      0ffh
0407 ca5105    jp      z,0551h
040a 3e2b      ld      a,2bh
040c cdce09    call    09ceh
040f 213000    ld      hl,0030h
0412 226b04    ld      (046bh),hl
0415 210019    ld      hl,1900h
0418 063d      ld      b,3dh
041a e5        push    hl
041b c5        push    bc
041c eb        ex      de,hl
041d 0e1a      ld      c,1ah
041f cd0500    call    0005h
0422 114a04    ld      de,044ah
0425 0e21      ld      c,21h
0427 cd0500    call    0005h
042a 216b04    ld      hl,046bh
042d 34        inc     (hl)
042e c1        pop     bc
042f e1        pop     hl
0430 b7        or      a
0431 2010      jr      nz,0443h         ; (+10h)
0433 3a6b04    ld      a,(046bh)
0436 e607      and     07h
0438 3e2b      ld      a,2bh
043a ccce09    call    z,09ceh
043d 118000    ld      de,0080h
0440 19        add     hl,de
0441 18d7      jr      041ah            ; (-29h)
0443 22780d    ld      (0d78h),hl
0446 2a6b04    ld      hl,(046bh)
0449 116d00    ld      de,006dh
044c b7        or      a
044d ed52      sbc     hl,de
044f 302f      jr      nc,0480h         ; (+2fh)
0451 cd560a    call    0a56h
0454 07        rlca    
0455 0d        dec     c
0456 0a        ld      a,(bc)
0457 0a        ld      a,(bc)
0458 2a2a2a    ld      hl,(2a2ah)
045b 2043      jr      nz,04a0h         ; (+43h)
045d 41        ld      b,c
045e 4e        ld      c,(hl)
045f 4e        ld      c,(hl)
0460 4f        ld      c,a
0461 54        ld      d,h
0462 2052      jr      nz,04b6h         ; (+52h)
0464 45        ld      b,l
0465 41        ld      b,c
0466 44        ld      b,h
0467 2041      jr      nz,04aah         ; (+41h)
0469 3a5359    ld      a,(5953h)
046c 53        ld      d,e
046d 54        ld      d,h
046e 45        ld      b,l
046f 4d        ld      c,l
0470 2e53      ld      l,53h
0472 57        ld      d,a
0473 50        ld      d,b
0474 202a      jr      nz,04a0h         ; (+2ah)
0476 2a2a0d    ld      hl,(0d2ah)
0479 0a        ld      a,(bc)
047a 0a        ld      a,(bc)
047b 00        nop     
047c 3eff      ld      a,0ffh
047e b7        or      a
047f c9        ret     

0480 dd210019  ld      ix,1900h
0484 01001e    ld      bc,1e00h
0487 fd21003c  ld      iy,3c00h
048b 3a0301    ld      a,(0103h)
048e 57        ld      d,a
048f 1e80      ld      e,80h
0491 fd7e00    ld      a,(iy+00h)
0494 a3        and     e
0495 2807      jr      z,049eh          ; (+07h)
0497 dd7e00    ld      a,(ix+00h)
049a 82        add     a,d
049b dd7700    ld      (ix+00h),a
049e cb0b      rrc     e
04a0 3002      jr      nc,04a4h         ; (+02h)
04a2 fd23      inc     iy
04a4 dd23      inc     ix
04a6 0b        dec     bc
04a7 78        ld      a,b
04a8 b1        or      c
04a9 20e6      jr      nz,0491h         ; (-1ah)
04ab 218037    ld      hl,3780h
04ae 22760d    ld      (0d76h),hl
04b1 3a0301    ld      a,(0103h)
04b4 67        ld      h,a
04b5 2e00      ld      l,00h
04b7 2b        dec     hl
04b8 227a0d    ld      (0d7ah),hl
04bb af        xor     a
04bc 327e0d    ld      (0d7eh),a
04bf 3e20      ld      a,20h
04c1 cdce09    call    09ceh
04c4 3a0301    ld      a,(0103h)
04c7 c61c      add     a,1ch
04c9 f5        push    af
04ca cb3f      srl     a
04cc cb3f      srl     a
04ce 6f        ld      l,a
04cf 2600      ld      h,00h
04d1 cd1509    call    0915h
04d4 f1        pop     af
04d5 e603      and     03h
04d7 2815      jr      z,04eeh          ; (+15h)
04d9 211f08    ld      hl,081fh
04dc 3d        dec     a
04dd cc5c0a    call    z,0a5ch
04e0 212308    ld      hl,0823h
04e3 3d        dec     a
04e4 cc5c0a    call    z,0a5ch
04e7 212608    ld      hl,0826h
04ea 3d        dec     a
04eb cc5c0a    call    z,0a5ch
04ee cd560a    call    0a56h
04f1 4b        ld      c,e
04f2 2073      jr      nz,0567h         ; (+73h)
04f4 79        ld      a,c
04f5 73        ld      (hl),e
04f6 74        ld      (hl),h
04f7 65        ld      h,l
04f8 6d        ld      l,l
04f9 2062      jr      nz,055dh         ; (+62h)
04fb 75        ld      (hl),l
04fc 69        ld      l,c
04fd 6c        ld      l,h
04fe 74        ld      (hl),h
04ff 0d        dec     c
0500 0a        ld      a,(bc)
0501 0a        ld      a,(bc)
0502 73        ld      (hl),e
0503 79        ld      a,c
0504 6d        ld      l,l
0505 62        ld      h,d
0506 6f        ld      l,a
0507 6c        ld      l,h
0508 69        ld      l,c
0509 63        ld      h,e
050a 2070      jr      nz,057ch         ; (+70h)
050c 61        ld      h,c
050d 74        ld      (hl),h
050e 63        ld      h,e
050f 68        ld      l,b
0510 2075      jr      nz,0587h         ; (+75h)
0512 74        ld      (hl),h
0513 69        ld      l,c
0514 6c        ld      l,h
0515 69        ld      l,c
0516 74        ld      (hl),h
0517 79        ld      a,c
0518 0d        dec     c
0519 0a        ld      a,(bc)
051a 2d        dec     l
051b 2d        dec     l
051c 2d        dec     l
051d 2d        dec     l
051e 2d        dec     l
051f 2d        dec     l
0520 2d        dec     l
0521 2d        dec     l
0522 2d        dec     l
0523 2d        dec     l
0524 2d        dec     l
0525 2d        dec     l
0526 2d        dec     l
0527 2d        dec     l
0528 2d        dec     l
0529 2d        dec     l
052a 2d        dec     l
052b 2d        dec     l
052c 2d        dec     l
052d 2d        dec     l
052e 2d        dec     l
052f 2d        dec     l
0530 0d        dec     c
0531 0a        ld      a,(bc)
0532 0a        ld      a,(bc)
0533 65        ld      h,l
0534 6e        ld      l,(hl)
0535 74        ld      (hl),h
0536 65        ld      h,l
0537 72        ld      (hl),d
0538 206c      jr      nz,05a6h         ; (+6ch)
053a 61        ld      h,c
053b 62        ld      h,d
053c 65        ld      h,l
053d 6c        ld      l,h
053e 206f      jr      nz,05afh         ; (+6fh)
0540 72        ld      (hl),d
0541 2068      jr      nz,05abh         ; (+68h)
0543 65        ld      h,l
0544 78        ld      a,b
0545 2061      jr      nz,05a8h         ; (+61h)
0547 64        ld      h,h
0548 64        ld      h,h
0549 72        ld      (hl),d
054a 65        ld      h,l
054b 73        ld      (hl),e
054c 73        ld      (hl),e
054d 206f      jr      nz,05beh         ; (+6fh)
054f 66        ld      h,(hl)
0550 0d        dec     c
0551 0a        ld      a,(bc)
0552 70        ld      (hl),b
0553 61        ld      h,c
0554 72        ld      (hl),d
0555 61        ld      h,c
0556 6d        ld      l,l
0557 65        ld      h,l
0558 74        ld      (hl),h
0559 65        ld      h,l
055a 72        ld      (hl),d
055b 2074      jr      nz,05d1h         ; (+74h)
055d 6f        ld      l,a
055e 2063      jr      nz,05c3h         ; (+63h)
0560 68        ld      l,b
0561 61        ld      h,c
0562 6e        ld      l,(hl)
0563 67        ld      h,a
0564 65        ld      h,l
0565 2c        inc     l
0566 203c      jr      nz,05a4h         ; (+3ch)
0568 52        ld      d,d
0569 45        ld      b,l
056a 54        ld      d,h
056b 55        ld      d,l
056c 52        ld      d,d
056d 4e        ld      c,(hl)
056e 3e0d      ld      a,0dh
0570 0a        ld      a,(bc)
0571 74        ld      (hl),h
0572 6f        ld      l,a
0573 2067      jr      nz,05dch         ; (+67h)
0575 6f        ld      l,a
0576 2066      jr      nz,05deh         ; (+66h)
0578 6f        ld      l,a
0579 77        ld      (hl),a
057a 61        ld      h,c
057b 72        ld      (hl),d
057c 64        ld      h,h
057d 206f      jr      nz,05eeh         ; (+6fh)
057f 72        ld      (hl),d
0580 2051      jr      nz,05d3h         ; (+51h)
0582 2074      jr      nz,05f8h         ; (+74h)
0584 6f        ld      l,a
0585 2065      jr      nz,05ech         ; (+65h)
0587 78        ld      a,b
0588 69        ld      l,c
0589 74        ld      (hl),h
058a 0d        dec     c
058b 0a        ld      a,(bc)
058c 00        nop     
058d cd560a    call    0a56h
0590 0d        dec     c
0591 0a        ld      a,(bc)
0592 70        ld      (hl),b
0593 61        ld      h,c
0594 72        ld      (hl),d
0595 61        ld      h,c
0596 6d        ld      l,l
0597 203f      jr      nz,05d8h         ; (+3fh)
0599 202e      jr      nz,05c9h         ; (+2eh)
059b 2e2e      ld      l,2eh
059d 2000      jr      nz,059fh         ; (+00h)
059f 2a2c08    ld      hl,(082ch)
05a2 e5        push    hl
05a3 112a08    ld      de,082ah
05a6 cd660a    call    0a66h
05a9 c1        pop     bc
05aa 2a2b08    ld      hl,(082bh)
05ad 1651      ld      d,51h
05af 1e01      ld      e,01h
05b1 b7        or      a
05b2 ed52      sbc     hl,de
05b4 cadd06    jp      z,06ddh
05b7 cd560a    call    0a56h
05ba 70        ld      (hl),b
05bb 61        ld      h,c
05bc 72        ld      (hl),d
05bd 61        ld      h,c
05be 6d        ld      l,l
05bf 203f      jr      nz,0600h         ; (+3fh)
05c1 202e      jr      nz,05f1h         ; (+2eh)
05c3 2e2e      ld      l,2eh
05c5 2000      jr      nz,05c7h         ; (+00h)
05c7 19        add     hl,de
05c8 7d        ld      a,l
05c9 b7        or      a
05ca cae306    jp      z,06e3h
05cd 3d        dec     a
05ce cadf06    jp      z,06dfh
05d1 7c        ld      a,h
05d2 fe20      cp      20h
05d4 3043      jr      nc,0619h         ; (+43h)
05d6 3e07      ld      a,07h
05d8 cdce09    call    09ceh
05db 18c2      jr      059fh            ; (-3eh)
05dd af        xor     a
05de c9        ret     

05df ed432c08  ld      (082ch),bc
05e3 2a7a0d    ld      hl,(0d7ah)
05e6 23        inc     hl
05e7 227a0d    ld      (0d7ah),hl
05ea 3a7e0d    ld      a,(0d7eh)
05ed b7        or      a
05ee 2009      jr      nz,05f9h         ; (+09h)
05f0 2a7a0d    ld      hl,(0d7ah)
05f3 cdfb08    call    08fbh
05f6 c39b07    jp      079bh
05f9 212c08    ld      hl,082ch
05fc 7e        ld      a,(hl)
05fd 23        inc     hl
05fe fe20      cp      20h
0600 f5        push    af
0601 d4ce09    call    nc,09ceh
0604 f1        pop     af
0605 30f5      jr      nc,05fch         ; (-0bh)
0607 3e2b      ld      a,2bh
0609 cdce09    call    09ceh
060c 2a7f0d    ld      hl,(0d7fh)
060f 23        inc     hl
0610 227f0d    ld      (0d7fh),hl
0613 cd1509    call    0915h
0616 c39b07    jp      079bh
0619 fd212c08  ld      iy,082ch
061d fe30      cp      30h
061f 382a      jr      c,064bh          ; (+2ah)
0621 fe3a      cp      3ah
0623 3026      jr      nc,064bh         ; (+26h)
0625 cddd08    call    08ddh
0628 3804      jr      c,062eh          ; (+04h)
062a d60d      sub     0dh
062c 2852      jr      z,0680h          ; (+52h)
062e cd560a    call    0a56h
0631 07        rlca    
0632 2020      jr      nz,0654h         ; (+20h)
0634 69        ld      l,c
0635 6e        ld      l,(hl)
0636 76        halt    
0637 61        ld      h,c
0638 6c        ld      l,h
0639 69        ld      l,c
063a 64        ld      h,h
063b 2068      jr      nz,06a5h         ; (+68h)
063d 65        ld      h,l
063e 78        ld      a,b
063f 2061      jr      nz,06a2h         ; (+61h)
0641 64        ld      h,h
0642 64        ld      h,h
0643 72        ld      (hl),d
0644 65        ld      h,l
0645 73        ld      (hl),e
0646 73        ld      (hl),e
0647 00        nop     
0648 c38d06    jp      068dh
064b dd2a760d  ld      ix,(0d76h)
064f ed5b780d  ld      de,(0d78h)
0653 cd6708    call    0867h
0656 301f      jr      nc,0677h         ; (+1fh)
0658 cd560a    call    0a56h
065b 07        rlca    
065c 2020      jr      nz,067eh         ; (+20h)
065e 6e        ld      l,(hl)
065f 6f        ld      l,a
0660 2073      jr      nz,06d5h         ; (+73h)
0662 75        ld      (hl),l
0663 63        ld      h,e
0664 68        ld      l,b
0665 2073      jr      nz,06dah         ; (+73h)
0667 79        ld      a,c
0668 6d        ld      l,l
0669 62        ld      h,d
066a 6f        ld      l,a
066b 6c        ld      l,h
066c 2065      jr      nz,06d3h         ; (+65h)
066e 78        ld      a,b
066f 69        ld      l,c
0670 73        ld      (hl),e
0671 74        ld      (hl),h
0672 73        ld      (hl),e
0673 00        nop     
0674 c38d06    jp      068dh
0677 3a0301    ld      a,(0103h)
067a 57        ld      d,a
067b 1e00      ld      e,00h
067d 19        add     hl,de
067e 3e01      ld      a,01h
0680 327e0d    ld      (0d7eh),a
0683 227a0d    ld      (0d7ah),hl
0686 210000    ld      hl,0000h
0689 227f0d    ld      (0d7fh),hl
068c 212c08    ld      hl,082ch
068f 7e        ld      a,(hl)
0690 fe20      cp      20h
0692 d4ce09    call    nc,09ceh
0695 7e        ld      a,(hl)
0696 23        inc     hl
0697 fe0d      cp      0dh
0699 20f4      jr      nz,068fh         ; (-0ch)
069b 3a0301    ld      a,(0103h)
069e 57        ld      d,a
069f 1e00      ld      e,00h
06a1 2a7a0d    ld      hl,(0d7ah)
06a4 b7        or      a
06a5 ed52      sbc     hl,de
06a7 3858      jr      c,0701h          ; (+58h)
06a9 44        ld      b,h
06aa 4d        ld      c,l
06ab 21001c    ld      hl,1c00h
06ae 19        add     hl,de
06af eb        ex      de,hl
06b0 2a7a0d    ld      hl,(0d7ah)
06b3 b7        or      a
06b4 ed52      sbc     hl,de
06b6 3049      jr      nc,0701h         ; (+49h)
06b8 218019    ld      hl,1980h
06bb 09        add     hl,bc
06bc 227c0d    ld      (0d7ch),hl
06bf 3e3d      ld      a,3dh
06c1 cdce09    call    09ceh
06c4 2a7c0d    ld      hl,(0d7ch)
06c7 7e        ld      a,(hl)
06c8 cd0009    call    0900h
06cb cd560a    call    0a56h
06ce 2020      jr      nz,06f0h         ; (+20h)
06d0 63        ld      h,e
06d1 68        ld      l,b
06d2 61        ld      h,c
06d3 6e        ld      l,(hl)
06d4 67        ld      h,a
06d5 65        ld      h,l
06d6 2074      jr      nz,074ch         ; (+74h)
06d8 6f        ld      l,a
06d9 203f      jr      nz,071ah         ; (+3fh)
06db 2000      jr      nz,06ddh         ; (+00h)
06dd 113708    ld      de,0837h
06e0 cd660a    call    0a66h
06e3 3a3808    ld      a,(0838h)
06e6 b7        or      a
06e7 ca8d06    jp      z,068dh
06ea fd213908  ld      iy,0839h
06ee cddd08    call    08ddh
06f1 da2e07    jp      c,072eh
06f4 7c        ld      a,h
06f5 b7        or      a
06f6 c22e07    jp      nz,072eh
06f9 7d        ld      a,l
06fa 2a7c0d    ld      hl,(0d7ch)
06fd 77        ld      (hl),a
06fe c38d06    jp      068dh
0701 cd560a    call    0a56h
0704 07        rlca    
0705 2020      jr      nz,0727h         ; (+20h)
0707 61        ld      h,c
0708 64        ld      h,h
0709 64        ld      h,h
070a 72        ld      (hl),d
070b 65        ld      h,l
070c 73        ld      (hl),e
070d 73        ld      (hl),e
070e 206f      jr      nz,077fh         ; (+6fh)
0710 75        ld      (hl),l
0711 74        ld      (hl),h
0712 206f      jr      nz,0783h         ; (+6fh)
0714 66        ld      h,(hl)
0715 2072      jr      nz,0789h         ; (+72h)
0717 61        ld      h,c
0718 6e        ld      l,(hl)
0719 67        ld      h,a
071a 65        ld      h,l
071b 00        nop     
071c c38d06    jp      068dh
071f 2e32      ld      l,32h
0721 35        dec     (hl)
0722 00        nop     
0723 2e35      ld      l,35h
0725 00        nop     
0726 2e37      ld      l,37h
0728 35        dec     (hl)
0729 00        nop     
072a 0a        ld      a,(bc)
072b 00        nop     
072c 00        nop     
072d 00        nop     
072e 00        nop     
072f 00        nop     
0730 00        nop     
0731 00        nop     
0732 00        nop     
0733 00        nop     
0734 00        nop     
0735 00        nop     
0736 00        nop     
0737 0a        ld      a,(bc)
0738 1600      ld      d,00h
073a 00        nop     
073b 00        nop     
073c 00        nop     
073d 00        nop     
073e 00        nop     
073f 00        nop     
0740 00        nop     
0741 00        nop     
0742 00        nop     
0743 00        nop     
0744 cdeb09    call    09ebh
0747 fe1b      cp      1bh
0749 37        scf     
074a c8        ret     z

074b fe03      cp      03h
074d 37        scf     
074e c8        ret     z

074f fe0d      cp      0dh
0751 37        scf     
0752 c8        ret     z

0753 57        ld      d,a
0754 cdcd08    call    08cdh
0757 3807      jr      c,0760h          ; (+07h)
0759 f5        push    af
075a 7a        ld      a,d
075b cdce09    call    09ceh
075e f1        pop     af
075f c9        ret     

0760 3e07      ld      a,07h
0762 cdce09    call    09ceh
0765 18dd      jr      0744h            ; (-23h)
0767 cda208    call    08a2h
076a d8        ret     c

076b 210000    ld      hl,0000h
076e cdba08    call    08bah
0771 d8        ret     c

0772 cdba08    call    08bah
0775 d8        ret     c

0776 cdba08    call    08bah
0779 d8        ret     c

077a cdba08    call    08bah
077d d8        ret     c

077e cda208    call    08a2h
0781 d8        ret     c

0782 fde5      push    iy
0784 c1        pop     bc
0785 dd7e00    ld      a,(ix+00h)
0788 fe21      cp      21h
078a 3f        ccf     
078b d0        ret     nc

078c 0a        ld      a,(bc)
078d ddbe00    cp      (ix+00h)
0790 2005      jr      nz,0797h         ; (+05h)
0792 dd23      inc     ix
0794 03        inc     bc
0795 18ee      jr      0785h            ; (-12h)
0797 dd23      inc     ix
0799 dd7e00    ld      a,(ix+00h)
079c fe21      cp      21h
079e 30f7      jr      nc,0797h         ; (-09h)
07a0 18c5      jr      0767h            ; (-3bh)
07a2 dd7e00    ld      a,(ix+00h)
07a5 fe1a      cp      1ah
07a7 37        scf     
07a8 c8        ret     z

07a9 fe21      cp      21h
07ab d0        ret     nc

07ac e5        push    hl
07ad dde5      push    ix
07af e1        pop     hl
07b0 b7        or      a
07b1 ed52      sbc     hl,de
07b3 e1        pop     hl
07b4 3f        ccf     
07b5 d8        ret     c

07b6 dd23      inc     ix
07b8 18e8      jr      07a2h            ; (-18h)
07ba 29        add     hl,hl
07bb 29        add     hl,hl
07bc 29        add     hl,hl
07bd 29        add     hl,hl
07be dd7e00    ld      a,(ix+00h)
07c1 dd23      inc     ix
07c3 cdcd08    call    08cdh
07c6 d8        ret     c

07c7 0600      ld      b,00h
07c9 4f        ld      c,a
07ca 09        add     hl,bc
07cb b7        or      a
07cc c9        ret     

07cd d630      sub     30h
07cf d8        ret     c

07d0 fe0a      cp      0ah
07d2 3f        ccf     
07d3 d0        ret     nc

07d4 d607      sub     07h
07d6 fe0a      cp      0ah
07d8 d8        ret     c

07d9 fe10      cp      10h
07db 3f        ccf     
07dc c9        ret     

07dd 210000    ld      hl,0000h
07e0 110000    ld      de,0000h
07e3 29        add     hl,hl
07e4 d8        ret     c

07e5 29        add     hl,hl
07e6 d8        ret     c

07e7 29        add     hl,hl
07e8 d8        ret     c

07e9 29        add     hl,hl
07ea d8        ret     c

07eb 19        add     hl,de
07ec fd7e00    ld      a,(iy+00h)
07ef fd23      inc     iy
07f1 4f        ld      c,a
07f2 cdcd08    call    08cdh
07f5 5f        ld      e,a
07f6 30eb      jr      nc,07e3h         ; (-15h)
07f8 79        ld      a,c
07f9 b7        or      a
07fa c9        ret     

07fb 7c        ld      a,h
07fc cd0009    call    0900h
07ff 7d        ld      a,l
0800 f5        push    af
0801 1f        rra     
0802 1f        rra     
0803 1f        rra     
0804 1f        rra     
0805 cd0909    call    0909h
0808 f1        pop     af
0809 e60f      and     0fh
080b c690      add     a,90h
080d 27        daa     
080e ce40      adc     a,40h
0810 27        daa     
0811 cdce09    call    09ceh
0814 c9        ret     

0815 0e00      ld      c,00h
0817 111027    ld      de,2710h
081a cd3609    call    0936h
081d 11e803    ld      de,03e8h
0820 cd3609    call    0936h
0823 116400    ld      de,0064h
0826 cd3609    call    0936h
0829 110a00    ld      de,000ah
082c cd3609    call    0936h
082f 7d        ld      a,l
0830 f630      or      30h
0832 cdce09    call    09ceh
0835 c9        ret     

0836 3eff      ld      a,0ffh
0838 3c        inc     a
0839 b7        or      a
083a ed52      sbc     hl,de
083c 30fa      jr      nc,0838h         ; (-06h)
083e 19        add     hl,de
083f f630      or      30h
0841 fe30      cp      30h
0843 2003      jr      nz,0848h         ; (+03h)
0845 0c        inc     c
0846 0d        dec     c
0847 c8        ret     z

0848 cdce09    call    09ceh
084b 0c        inc     c
084c c9        ret     

084d dd4e00    ld      c,(ix+00h)
0850 dd23      inc     ix
0852 0608      ld      b,08h
0854 79        ld      a,c
0855 ac        xor     h
0856 07        rlca    
0857 ed6a      adc     hl,hl
0859 0f        rrca    
085a 3008      jr      nc,0864h         ; (+08h)
085c 7c        ld      a,h
085d ee10      xor     10h
085f 67        ld      h,a
0860 7d        ld      a,l
0861 ee20      xor     20h
0863 6f        ld      l,a
0864 cb11      rl      c
0866 10ec      djnz    0854h            ; (-14h)
0868 1b        dec     de
0869 7a        ld      a,d
086a b3        or      e
086b 20e0      jr      nz,084dh         ; (-20h)
086d c9        ret     

086e cd0b0a    call    0a0bh
0871 fe41      cp      41h
0873 38f9      jr      c,086eh          ; (-07h)
0875 fe45      cp      45h
0877 30f5      jr      nc,086eh         ; (-0bh)
0879 d641      sub     41h
087b 323f0d    ld      (0d3fh),a
087e cd560a    call    0a56h
0881 0d        dec     c
0882 0a        ld      a,(bc)
0883 00        nop     
0884 c9        ret     

0885 f5        push    af
0886 cd560a    call    0a56h
0889 0d        dec     c
088a 0a        ld      a,(bc)
088b 73        ld      (hl),e
088c 74        ld      (hl),h
088d 73        ld      (hl),e
088e 3d        dec     a
088f 00        nop     
0890 f1        pop     af
0891 cd0009    call    0900h
0894 cd560a    call    0a56h
0897 2020      jr      nz,08b9h         ; (+20h)
0899 74        ld      (hl),h
089a 72        ld      (hl),d
089b 6b        ld      l,e
089c 3d        dec     a
089d 00        nop     
089e db41      in      a,(41h)
08a0 cd0009    call    0900h
08a3 cd560a    call    0a56h
08a6 2020      jr      nz,08c8h         ; (+20h)
08a8 2020      jr      nz,08cah         ; (+20h)
08aa 70        ld      (hl),b
08ab 72        ld      (hl),d
08ac 65        ld      h,l
08ad 73        ld      (hl),e
08ae 73        ld      (hl),e
08af 2061      jr      nz,0912h         ; (+61h)
08b1 6e        ld      l,(hl)
08b2 79        ld      a,c
08b3 206b      jr      nz,0920h         ; (+6bh)
08b5 65        ld      h,l
08b6 79        ld      a,c
08b7 2074      jr      nz,092dh         ; (+74h)
08b9 6f        ld      l,a
08ba 2065      jr      nz,0921h         ; (+65h)
08bc 78        ld      a,b
08bd 69        ld      l,c
08be 74        ld      (hl),h
08bf 202e      jr      nz,08efh         ; (+2eh)
08c1 2e20      ld      l,20h
08c3 00        nop     
08c4 cdeb09    call    09ebh
08c7 c9        ret     

08c8 3e07      ld      a,07h
08ca cdce09    call    09ceh
08cd c9        ret     

08ce f5        push    af
08cf c5        push    bc
08d0 d5        push    de
08d1 e5        push    hl
08d2 21480d    ld      hl,0d48h
08d5 fe0d      cp      0dh
08d7 2002      jr      nz,08dbh         ; (+02h)
08d9 3600      ld      (hl),00h
08db fe20      cp      20h
08dd 3801      jr      c,08e0h          ; (+01h)
08df 34        inc     (hl)
08e0 5f        ld      e,a
08e1 0e02      ld      c,02h
08e3 cd0500    call    0005h
08e6 e1        pop     hl
08e7 d1        pop     de
08e8 c1        pop     bc
08e9 f1        pop     af
08ea c9        ret     

08eb e5        push    hl
08ec d5        push    de
08ed c5        push    bc
08ee 11080a    ld      de,0a08h
08f1 0e0a      ld      c,0ah
08f3 cd0500    call    0005h
08f6 3a090a    ld      a,(0a09h)
08f9 b7        or      a
08fa 3a0a0a    ld      a,(0a0ah)
08fd 2002      jr      nz,0901h         ; (+02h)
08ff 3e0d      ld      a,0dh
0901 cd280a    call    0a28h
0904 c1        pop     bc
0905 d1        pop     de
0906 e1        pop     hl
0907 c9        ret     

0908 010000    ld      bc,0000h
090b 11300a    ld      de,0a30h
090e 0e0a      ld      c,0ah
0910 cd0500    call    0005h
0913 3a310a    ld      a,(0a31h)
0916 fe01      cp      01h
0918 20f1      jr      nz,090bh         ; (-0fh)
091a 3a320a    ld      a,(0a32h)
091d fe20      cp      20h
091f 38ea      jr      c,090bh          ; (-16h)
0921 4f        ld      c,a
0922 3e0a      ld      a,0ah
0924 cdce09    call    09ceh
0927 79        ld      a,c
0928 cbbf      res     7,a
092a fe60      cp      60h
092c d8        ret     c

092d d620      sub     20h
092f c9        ret     

0930 02        ld      (bc),a
0931 00        nop     
0932 00        nop     
0933 00        nop     
0934 cd560a    call    0a56h
0937 0d        dec     c
0938 0a        ld      a,(bc)
0939 0a        ld      a,(bc)
093a 72        ld      (hl),d
093b 65        ld      h,l
093c 74        ld      (hl),h
093d 75        ld      (hl),l
093e 72        ld      (hl),d
093f 6e        ld      l,(hl)
0940 69        ld      l,c
0941 6e        ld      l,(hl)
0942 67        ld      h,a
0943 2074      jr      nz,09b9h         ; (+74h)
0945 6f        ld      l,a
0946 2043      jr      nz,098bh         ; (+43h)
0948 50        ld      d,b
0949 2f        cpl     
094a 4d        ld      c,l
094b 202e      jr      nz,097bh         ; (+2eh)
094d 2e2e      ld      l,2eh
094f 200d      jr      nz,095eh         ; (+0dh)
0951 0a        ld      a,(bc)
0952 00        nop     
0953 c30000    jp      0000h
0956 e3        ex      (sp),hl
0957 cd5c0a    call    0a5ch
095a e3        ex      (sp),hl
095b c9        ret     

095c 7e        ld      a,(hl)
095d 23        inc     hl
095e fe00      cp      00h
0960 c8        ret     z

0961 cdce09    call    09ceh
0964 18f6      jr      095ch            ; (-0ah)
0966 d5        push    de
0967 0e0a      ld      c,0ah
0969 cd0500    call    0005h
096c e1        pop     hl
096d 23        inc     hl
096e 7e        ld      a,(hl)
096f b7        or      a
0970 c8        ret     z

0971 23        inc     hl
0972 47        ld      b,a
0973 7e        ld      a,(hl)
0974 cd280a    call    0a28h
0977 77        ld      (hl),a
0978 23        inc     hl
0979 10f8      djnz    0973h            ; (-08h)
097b 360d      ld      (hl),0dh
097d c9        ret     

097e cd560a    call    0a56h
0981 0d        dec     c
0982 0a        ld      a,(bc)
0983 0a        ld      a,(bc)
0984 2d        dec     l
0985 2d        dec     l
0986 2d        dec     l
0987 2057      jr      nz,09e0h         ; (+57h)
0989 72        ld      (hl),d
098a 69        ld      l,c
098b 74        ld      (hl),h
098c 65        ld      h,l
098d 2053      jr      nz,09e2h         ; (+53h)
098f 79        ld      a,c
0990 73        ld      (hl),e
0991 74        ld      (hl),h
0992 65        ld      h,l
0993 6d        ld      l,l
0994 2054      jr      nz,09eah         ; (+54h)
0996 72        ld      (hl),d
0997 61        ld      h,c
0998 63        ld      h,e
0999 6b        ld      l,e
099a 73        ld      (hl),e
099b 202d      jr      nz,09cah         ; (+2dh)
099d 2d        dec     l
099e 2d        dec     l
099f 0d        dec     c
09a0 0a        ld      a,(bc)
09a1 0a        ld      a,(bc)
09a2 65        ld      h,l
09a3 6e        ld      l,(hl)
09a4 74        ld      (hl),h
09a5 65        ld      h,l
09a6 72        ld      (hl),d
09a7 2064      jr      nz,0a0dh         ; (+64h)
09a9 72        ld      (hl),d
09aa 69        ld      l,c
09ab 76        halt    
09ac 65        ld      h,l
09ad 206e      jr      nz,0a1dh         ; (+6eh)
09af 61        ld      h,c
09b0 6d        ld      l,l
09b1 65        ld      h,l
09b2 2028      jr      nz,09dch         ; (+28h)
09b4 41        ld      b,c
09b5 2c        inc     l
09b6 42        ld      b,d
09b7 2c        inc     l
09b8 43        ld      b,e
09b9 2c        inc     l
09ba 206f      jr      nz,0a2bh         ; (+6fh)
09bc 72        ld      (hl),d
09bd 2044      jr      nz,0a03h         ; (+44h)
09bf 29        add     hl,hl
09c0 202e      jr      nz,09f0h         ; (+2eh)
09c2 2e2e      ld      l,2eh
09c4 2000      jr      nz,09c6h         ; (+00h)
09c6 cd6e09    call    096eh
09c9 3e02      ld      a,02h
09cb 184b      jr      0a18h            ; (+4bh)
09cd cd560a    call    0a56h
09d0 0d        dec     c
09d1 0a        ld      a,(bc)
09d2 0a        ld      a,(bc)
09d3 2d        dec     l
09d4 2d        dec     l
09d5 2d        dec     l
09d6 2052      jr      nz,0a2ah         ; (+52h)
09d8 65        ld      h,l
09d9 61        ld      h,c
09da 64        ld      h,h
09db 2053      jr      nz,0a30h         ; (+53h)
09dd 79        ld      a,c
09de 73        ld      (hl),e
09df 74        ld      (hl),h
09e0 65        ld      h,l
09e1 6d        ld      l,l
09e2 2054      jr      nz,0a38h         ; (+54h)
09e4 72        ld      (hl),d
09e5 61        ld      h,c
09e6 63        ld      h,e
09e7 6b        ld      l,e
09e8 73        ld      (hl),e
09e9 202d      jr      nz,0a18h         ; (+2dh)
09eb 2d        dec     l
09ec 2d        dec     l
09ed 0d        dec     c
09ee 0a        ld      a,(bc)
09ef 0a        ld      a,(bc)
09f0 65        ld      h,l
09f1 6e        ld      l,(hl)
09f2 74        ld      (hl),h
09f3 65        ld      h,l
09f4 72        ld      (hl),d
09f5 2064      jr      nz,0a5bh         ; (+64h)
09f7 72        ld      (hl),d
09f8 69        ld      l,c
09f9 76        halt    
09fa 65        ld      h,l
09fb 206e      jr      nz,0a6bh         ; (+6eh)
09fd 61        ld      h,c
09fe 6d        ld      l,l
09ff 65        ld      h,l
0a00 2028      jr      nz,0a2ah         ; (+28h)
0a02 41        ld      b,c
0a03 2c        inc     l
0a04 42        ld      b,d
0a05 2c        inc     l
0a06 43        ld      b,e
0a07 206f      jr      nz,0a78h         ; (+6fh)
0a09 72        ld      (hl),d
0a0a 2044      jr      nz,0a50h         ; (+44h)
0a0c 29        add     hl,hl
0a0d 202e      jr      nz,0a3dh         ; (+2eh)
0a0f 2e2e      ld      l,2eh
0a11 2000      jr      nz,0a13h         ; (+00h)
0a13 cd6e09    call    096eh
0a16 3e01      ld      a,01h
0a18 32470d    ld      (0d47h),a
0a1b cd570b    call    0b57h
0a1e c0        ret     nz

0a1f cdd60b    call    0bd6h
0a22 c8        ret     z

0a23 f5        push    af
0a24 cd560a    call    0a56h
0a27 0d        dec     c
0a28 0a        ld      a,(bc)
0a29 0a        ld      a,(bc)
0a2a 2a2a2a    ld      hl,(2a2ah)
0a2d 2044      jr      nz,0a73h         ; (+44h)
0a2f 49        ld      c,c
0a30 53        ld      d,e
0a31 4b        ld      c,e
0a32 2049      jr      nz,0a7dh         ; (+49h)
0a34 2f        cpl     
0a35 4f        ld      c,a
0a36 2045      jr      nz,0a7dh         ; (+45h)
0a38 52        ld      d,d
0a39 52        ld      d,d
0a3a 4f        ld      c,a
0a3b 52        ld      d,d
0a3c 204f      jr      nz,0a8dh         ; (+4fh)
0a3e 4e        ld      c,(hl)
0a3f 2053      jr      nz,0a94h         ; (+53h)
0a41 59        ld      e,c
0a42 53        ld      d,e
0a43 54        ld      d,h
0a44 45        ld      b,l
0a45 4d        ld      c,l
0a46 2041      jr      nz,0a89h         ; (+41h)
0a48 52        ld      d,d
0a49 45        ld      b,l
0a4a 41        ld      b,c
0a4b 202a      jr      nz,0a77h         ; (+2ah)
0a4d 2a2a00    ld      hl,(002ah)
0a50 f1        pop     af
0a51 f5        push    af
0a52 cd8509    call    0985h
0a55 f1        pop     af
0a56 c9        ret     

0a57 cd560a    call    0a56h
0a5a 70        ld      (hl),b
0a5b 72        ld      (hl),d
0a5c 65        ld      h,l
0a5d 73        ld      (hl),e
0a5e 73        ld      (hl),e
0a5f 203c      jr      nz,0a9dh         ; (+3ch)
0a61 52        ld      d,d
0a62 45        ld      b,l
0a63 54        ld      d,h
0a64 55        ld      d,l
0a65 52        ld      d,d
0a66 4e        ld      c,(hl)
0a67 3e20      ld      a,20h
0a69 77        ld      (hl),a
0a6a 68        ld      l,b
0a6b 65        ld      h,l
0a6c 6e        ld      l,(hl)
0a6d 2072      jr      nz,0ae1h         ; (+72h)
0a6f 65        ld      h,l
0a70 61        ld      h,c
0a71 64        ld      h,h
0a72 79        ld      a,c
0a73 0d        dec     c
0a74 0a        ld      a,(bc)
0a75 6f        ld      l,a
0a76 72        ld      (hl),d
0a77 203c      jr      nz,0ab5h         ; (+3ch)
0a79 43        ld      b,e
0a7a 54        ld      d,h
0a7b 4c        ld      c,h
0a7c 2d        dec     l
0a7d 43        ld      b,e
0a7e 3e20      ld      a,20h
0a80 74        ld      (hl),h
0a81 6f        ld      l,a
0a82 2061      jr      nz,0ae5h         ; (+61h)
0a84 62        ld      h,d
0a85 6f        ld      l,a
0a86 72        ld      (hl),d
0a87 74        ld      (hl),h
0a88 202e      jr      nz,0ab8h         ; (+2eh)
0a8a 2e2e      ld      l,2eh
0a8c 2000      jr      nz,0a8eh         ; (+00h)
0a8e 11300a    ld      de,0a30h
0a91 0e0a      ld      c,0ah
0a93 cd0500    call    0005h
0a96 3e00      ld      a,00h
0a98 323e0d    ld      (0d3eh),a
0a9b dd213e0d  ld      ix,0d3eh
0a9f cd0ff0    call    0f00fh
0aa2 3a460d    ld      a,(0d46h)
0aa5 e680      and     80h
0aa7 c8        ret     z

0aa8 cd560a    call    0a56h
0aab 0d        dec     c
0aac 0a        ld      a,(bc)
0aad 0a        ld      a,(bc)
0aae 2a2a2a    ld      hl,(2a2ah)
0ab1 2044      jr      nz,0af7h         ; (+44h)
0ab3 49        ld      c,c
0ab4 53        ld      d,e
0ab5 4b        ld      c,e
0ab6 2045      jr      nz,0afdh         ; (+45h)
0ab8 52        ld      d,d
0ab9 52        ld      d,d
0aba 4f        ld      c,a
0abb 52        ld      d,d
0abc 2c        inc     l
0abd 2044      jr      nz,0b03h         ; (+44h)
0abf 52        ld      d,d
0ac0 49        ld      c,c
0ac1 56        ld      d,(hl)
0ac2 45        ld      b,l
0ac3 204e      jr      nz,0b13h         ; (+4eh)
0ac5 4f        ld      c,a
0ac6 54        ld      d,h
0ac7 2052      jr      nz,0b1bh         ; (+52h)
0ac9 45        ld      b,l
0aca 41        ld      b,c
0acb 44        ld      b,h
0acc 59        ld      e,c
0acd 202a      jr      nz,0af9h         ; (+2ah)
0acf 2a2a00    ld      hl,(002ah)
0ad2 3eff      ld      a,0ffh
0ad4 b7        or      a
0ad5 c9        ret     

0ad6 3e03      ld      a,03h
0ad8 323e0d    ld      (0d3eh),a
0adb af        xor     a
0adc 32400d    ld      (0d40h),a
0adf 216e0d    ld      hl,0d6eh
0ae2 22420d    ld      (0d42h),hl
0ae5 dd213e0d  ld      ix,0d3eh
0ae9 cd0ff0    call    0f00fh
0aec 3a460d    ld      a,(0d46h)
0aef b7        or      a
0af0 c0        ret     nz

0af1 3a470d    ld      a,(0d47h)
0af4 fe01      cp      01h
0af6 2837      jr      z,0b2fh          ; (+37h)
0af8 210090    ld      hl,9000h
0afb 110190    ld      de,9001h
0afe 01b028    ld      bc,28b0h
0b01 3a440d    ld      a,(0d44h)
0b04 cb77      bit     6,a
0b06 2803      jr      z,0b0bh          ; (+03h)
0b08 015118    ld      bc,1851h
0b0b 3676      ld      (hl),76h
0b0d edb0      ldir    
0b0f 36c9      ld      (hl),0c9h
0b11 af        xor     a
0b12 32340e    ld      (0e34h),a
0b15 32530e    ld      (0e53h),a
0b18 3e01      ld      a,01h
0b1a 32380e    ld      (0e38h),a
0b1d 3e02      ld      a,02h
0b1f 32570e    ld      (0e57h),a
0b22 cdf00d    call    0df0h
0b25 3a440d    ld      a,(0d44h)
0b28 cbbf      res     7,a
0b2a d330      out     (30h),a
0b2c cdc20d    call    0dc2h
0b2f 210019    ld      hl,1900h
0b32 060a      ld      b,0ah
0b34 0e01      ld      c,01h
0b36 110002    ld      de,0200h
0b39 79        ld      a,c
0b3a fe02      cp      02h
0b3c 3003      jr      nc,0b41h         ; (+03h)
0b3e 118000    ld      de,0080h
0b41 3a470d    ld      a,(0d47h)
0b44 cd0f0d    call    0d0fh
0b47 c0        ret     nz

0b48 19        add     hl,de
0b49 0c        inc     c
0b4a 10ea      djnz    0b36h            ; (-16h)
0b4c 3a470d    ld      a,(0d47h)
0b4f fe01      cp      01h
0b51 2831      jr      z,0b84h          ; (+31h)
0b53 210050    ld      hl,5000h
0b56 060a      ld      b,0ah
0b58 0e01      ld      c,01h
0b5a 110002    ld      de,0200h
0b5d 79        ld      a,c
0b5e fe02      cp      02h
0b60 3003      jr      nc,0b65h         ; (+03h)
0b62 118000    ld      de,0080h
0b65 3e01      ld      a,01h
0b67 cd0f0d    call    0d0fh
0b6a c0        ret     nz

0b6b 19        add     hl,de
0b6c 0c        inc     c
0b6d 10eb      djnz    0b5ah            ; (-15h)
0b6f 210019    ld      hl,1900h
0b72 110050    ld      de,5000h
0b75 018012    ld      bc,1280h
0b78 1a        ld      a,(de)
0b79 be        cp      (hl)
0b7a c20b0d    jp      nz,0d0bh
0b7d 23        inc     hl
0b7e 13        inc     de
0b7f 0b        dec     bc
0b80 78        ld      a,b
0b81 b1        or      c
0b82 20f4      jr      nz,0b78h         ; (-0ch)
0b84 3e03      ld      a,03h
0b86 323e0d    ld      (0d3eh),a
0b89 3e01      ld      a,01h
0b8b 32400d    ld      (0d40h),a
0b8e 216e0d    ld      hl,0d6eh
0b91 22420d    ld      (0d42h),hl
0b94 dd213e0d  ld      ix,0d3eh
0b98 cd0ff0    call    0f00fh
0b9b 3a460d    ld      a,(0d46h)
0b9e b7        or      a
0b9f c0        ret     nz

0ba0 3a470d    ld      a,(0d47h)
0ba3 fe01      cp      01h
0ba5 281d      jr      z,0bc4h          ; (+1dh)
0ba7 3e01      ld      a,01h
0ba9 32340e    ld      (0e34h),a
0bac 32530e    ld      (0e53h),a
0baf 32570e    ld      (0e57h),a
0bb2 3e64      ld      a,64h
0bb4 32380e    ld      (0e38h),a
0bb7 cdf00d    call    0df0h
0bba 3a440d    ld      a,(0d44h)
0bbd cbbf      res     7,a
0bbf d330      out     (30h),a
0bc1 cdc20d    call    0dc2h
0bc4 21802b    ld      hl,2b80h
0bc7 0609      ld      b,09h
0bc9 0e01      ld      c,01h
0bcb 110002    ld      de,0200h
0bce 3a470d    ld      a,(0d47h)
0bd1 cd0f0d    call    0d0fh
0bd4 c0        ret     nz

0bd5 19        add     hl,de
0bd6 0c        inc     c
0bd7 10f2      djnz    0bcbh            ; (-0eh)
0bd9 3a470d    ld      a,(0d47h)
0bdc fe01      cp      01h
0bde 2829      jr      z,0c09h          ; (+29h)
0be0 210050    ld      hl,5000h
0be3 0609      ld      b,09h
0be5 0e01      ld      c,01h
0be7 110002    ld      de,0200h
0bea 3e01      ld      a,01h
0bec cd0f0d    call    0d0fh
0bef c0        ret     nz

0bf0 19        add     hl,de
0bf1 0c        inc     c
0bf2 10f3      djnz    0be7h            ; (-0dh)
0bf4 21802b    ld      hl,2b80h
0bf7 110050    ld      de,5000h
0bfa 010012    ld      bc,1200h
0bfd 1a        ld      a,(de)
0bfe be        cp      (hl)
0bff c20b0d    jp      nz,0d0bh
0c02 23        inc     hl
0c03 13        inc     de
0c04 0b        dec     bc
0c05 78        ld      a,b
0c06 b1        or      c
0c07 20f4      jr      nz,0bfdh         ; (-0ch)
0c09 af        xor     a
0c0a c9        ret     

0c0b 3eff      ld      a,0ffh
0c0d b7        or      a
0c0e c9        ret     

0c0f e5        push    hl
0c10 d5        push    de
0c11 c5        push    bc
0c12 dd213e0d  ld      ix,0d3eh
0c16 dd7103    ld      (ix+03h),c
0c19 dd7504    ld      (ix+04h),l
0c1c dd7405    ld      (ix+05h),h
0c1f dd7306    ld      (ix+06h),e
0c22 dd7207    ld      (ix+07h),d
0c25 fe02      cp      02h
0c27 2006      jr      nz,0c2fh         ; (+06h)
0c29 dd360002  ld      (ix+00h),02h
0c2d 1804      jr      0c33h            ; (+04h)
0c2f dd360001  ld      (ix+00h),01h
0c33 cd0ff0    call    0f00fh
0c36 dd7e08    ld      a,(ix+08h)
0c39 b7        or      a
0c3a c1        pop     bc
0c3b d1        pop     de
0c3c e1        pop     hl
0c3d c9        ret     

0c3e 00        nop     
0c3f 00        nop     
0c40 00        nop     
0c41 00        nop     
0c42 00        nop     
0c43 00        nop     
0c44 00        nop     
0c45 00        nop     
0c46 00        nop     
0c47 320000    ld      (0000h),a
0c4a 00        nop     
0c4b 00        nop     
0c4c 00        nop     
0c4d 00        nop     
0c4e 00        nop     
0c4f 00        nop     
0c50 00        nop     
0c51 00        nop     
0c52 00        nop     
0c53 00        nop     
0c54 00        nop     
0c55 00        nop     
0c56 00        nop     
0c57 00        nop     
0c58 00        nop     
0c59 00        nop     
0c5a 00        nop     
0c5b 00        nop     
0c5c 00        nop     
0c5d 00        nop     
0c5e 00        nop     
0c5f 00        nop     
0c60 00        nop     
0c61 00        nop     
0c62 00        nop     
0c63 00        nop     
0c64 00        nop     
0c65 00        nop     
0c66 00        nop     
0c67 00        nop     
0c68 00        nop     
0c69 00        nop     
0c6a 00        nop     
0c6b 00        nop     
0c6c 00        nop     
0c6d 00        nop     
0c6e 00        nop     
0c6f 00        nop     
0c70 00        nop     
0c71 00        nop     
0c72 00        nop     
0c73 00        nop     
0c74 00        nop     
0c75 00        nop     
0c76 00        nop     
0c77 00        nop     
0c78 00        nop     
0c79 00        nop     
0c7a 00        nop     
0c7b 00        nop     
0c7c 00        nop     
0c7d 00        nop     
0c7e 00        nop     
0c7f 00        nop     
0c80 00        nop     
0c81 00        nop     
0c82 00        nop     
0c83 00        nop     
0c84 00        nop     
0c85 00        nop     
0c86 00        nop     
0c87 00        nop     
0c88 00        nop     
0c89 00        nop     
0c8a 00        nop     
0c8b 00        nop     
0c8c 00        nop     
0c8d 00        nop     
0c8e 00        nop     
0c8f 00        nop     
0c90 00        nop     
0c91 00        nop     
0c92 00        nop     
0c93 00        nop     
0c94 00        nop     
0c95 00        nop     
0c96 00        nop     
0c97 00        nop     
0c98 00        nop     
0c99 00        nop     
0c9a 00        nop     
0c9b 00        nop     
0c9c 00        nop     
0c9d 00        nop     
0c9e 00        nop     
0c9f 00        nop     
0ca0 00        nop     
0ca1 00        nop     
0ca2 00        nop     
0ca3 00        nop     
0ca4 00        nop     
0ca5 00        nop     
0ca6 00        nop     
0ca7 00        nop     
0ca8 00        nop     
0ca9 00        nop     
0caa 00        nop     
0cab 00        nop     
0cac 00        nop     
0cad 00        nop     
0cae 00        nop     
0caf 00        nop     
0cb0 00        nop     
0cb1 00        nop     
0cb2 00        nop     
0cb3 00        nop     
0cb4 00        nop     
0cb5 00        nop     
0cb6 00        nop     
0cb7 00        nop     
0cb8 00        nop     
0cb9 00        nop     
0cba 00        nop     
0cbb 00        nop     
0cbc 00        nop     
0cbd 00        nop     
0cbe 00        nop     
0cbf 00        nop     
0cc0 00        nop     
0cc1 00        nop     
0cc2 f3        di      
0cc3 af        xor     a
0cc4 d356      out     (56h),a
0cc6 3c        inc     a
0cc7 d354      out     (54h),a
0cc9 216600    ld      hl,0066h
0ccc 36ed      ld      (hl),0edh
0cce 23        inc     hl
0ccf 36a3      ld      (hl),0a3h
0cd1 23        inc     hl
0cd2 36c9      ld      (hl),0c9h
0cd4 af        xor     a
0cd5 d341      out     (41h),a
0cd7 3ef4      ld      a,0f4h
0cd9 d340      out     (40h),a
0cdb 3e0c      ld      a,0ch
0cdd 3d        dec     a
0cde 20fd      jr      nz,0cddh         ; (-03h)
0ce0 210050    ld      hl,5000h
0ce3 0e43      ld      c,43h
0ce5 cd0090    call    9000h
0ce8 fb        ei      
0ce9 db40      in      a,(40h)
0ceb cb47      bit     0,a
0ced 20fa      jr      nz,0ce9h         ; (-06h)
0cef c9        ret     

0cf0 110050    ld      de,5000h
0cf3 212a0e    ld      hl,0e2ah
0cf6 46        ld      b,(hl)
0cf7 23        inc     hl
0cf8 cd1f0e    call    0e1fh
0cfb 10fb      djnz    0cf8h            ; (-05h)
0cfd 0e09      ld      c,09h
0cff 214b0e    ld      hl,0e4bh
0d02 46        ld      b,(hl)
0d03 23        inc     hl
0d04 cd1f0e    call    0e1fh
0d07 10fb      djnz    0d04h            ; (-05h)
0d09 21570e    ld      hl,0e57h
0d0c 34        inc     (hl)
0d0d 0d        dec     c
0d0e 20ef      jr      nz,0cffh         ; (-11h)
0d10 21b078    ld      hl,78b0h
0d13 b7        or      a
0d14 ed52      sbc     hl,de
0d16 44        ld      b,h
0d17 4d        ld      c,l
0d18 62        ld      h,d
0d19 6b        ld      l,e
0d1a 13        inc     de
0d1b 77        ld      (hl),a
0d1c edb0      ldir    
0d1e c9        ret     

0d1f c5        push    bc
0d20 46        ld      b,(hl)
0d21 23        inc     hl
0d22 7e        ld      a,(hl)
0d23 23        inc     hl
0d24 12        ld      (de),a
0d25 13        inc     de
0d26 10fc      djnz    0d24h            ; (-04h)
0d28 c1        pop     bc
0d29 c9        ret     

0d2a 1020      djnz    0d4ch            ; (+20h)
0d2c 4e        ld      c,(hl)
0d2d 0c        inc     c
0d2e 00        nop     
0d2f 03        inc     bc
0d30 f5        push    af
0d31 01fe01    ld      bc,01feh
0d34 00        nop     
0d35 010001    ld      bc,0100h
0d38 00        nop     
0d39 010001    ld      bc,0100h
0d3c f7        rst     30h
0d3d 164e      ld      d,4eh
0d3f 0c        inc     c
0d40 00        nop     
0d41 03        inc     bc
0d42 f5        push    af
0d43 01fb80    ld      bc,80fbh
0d46 e5        push    hl
0d47 01f732    ld      bc,32f7h
0d4a 4e        ld      c,(hl)
0d4b 100c      djnz    0d59h            ; (+0ch)
0d4d 00        nop     
0d4e 03        inc     bc
0d4f f5        push    af
0d50 01fe01    ld      bc,01feh
0d53 00        nop     
0d54 010001    ld      bc,0100h
0d57 00        nop     
0d58 010201    ld      bc,0102h
0d5b f7        rst     30h
0d5c 164e      ld      d,4eh
0d5e 0c        inc     c
0d5f 00        nop     
0d60 03        inc     bc
0d61 f5        push    af
0d62 01fb00    ld      bc,00fbh
0d65 e5        push    hl
0d66 00        nop     
0d67 e5        push    hl
0d68 01f732    ld      bc,32f7h
0d6b 4e        ld      c,(hl)
0d6c 47        ld      b,a
0d6d 20fa      jr      nz,0d69h         ; (-06h)
0d6f c9        ret     

0d70 110050    ld      de,5000h
0d73 212a0e    ld      hl,0e2ah
0d76 46        ld      b,(hl)
0d77 23        inc     hl
0d78 cd1f0e    call    0e1fh
0d7b 10fb      djnz    0d78h            ; (-05h)
0d7d 0e09      ld      c,09h
0d7f 210000    ld      hl,0000h
