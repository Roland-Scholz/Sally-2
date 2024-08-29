                    ld      sp,$0080                        ;[0080] 31 80 00
                    ld      ix,$00f3                        ;[0083] dd 21 f3 00
                    ld      b,$0e                           ;[0087] 06 0e
                    push    bc                              ;[0089] c5
                    call    $f00f                           ;[008a] cd 0f f0
                    pop     bc                              ;[008d] c1
                    ld      a,($00fb)                       ;[008e] 3a fb 00
                    or      a                               ;[0091] b7
                    jr      nz,$00ba                        ;[0092] 20 26
                    ld      a,($00f6)                       ;[0094] 3a f6 00
                    inc     a                               ;[0097] 3c
                    cp      $0b                             ;[0098] fe 0b
                    jr      c,$00a1                         ;[009a] 38 05
                    ld      a,$01                           ;[009c] 3e 01
                    ld      ($00f5),a                       ;[009e] 32 f5 00
                    ld      ($00f6),a                       ;[00a1] 32 f6 00
                    ld      hl,$00f8                        ;[00a4] 21 f8 00
                    inc     (hl)                            ;[00a7] 34
                    inc     (hl)                            ;[00a8] 34
                    djnz    $0089                           ;[00a9] 10 de
                    ld      hl,$f700                        ;[00ab] 21 00 f7
                    ld      ($00f7),hl                      ;[00ae] 22 f7 00
                    call    $f00f                           ;[00b1] cd 0f f0
                    ld      a,($00fb)                       ;[00b4] 3a fb 00
                    or      a                               ;[00b7] b7
                    jr      z,$00ca                         ;[00b8] 28 10
                    ld      a,$21                           ;[00ba] 3e 21
                    call    $f00c                           ;[00bc] cd 0c f0
                    ld      hl,$0000                        ;[00bf] 21 00 00
                    dec     hl                              ;[00c2] 2b
                    ld      a,h                             ;[00c3] 7c
                    or      l                               ;[00c4] b5
                    jr      nz,$00c2                        ;[00c5] 20 fb
                    jp      $f003                           ;[00c7] c3 03 f0
                    ld      hl,$fd00                        ;[00ca] 21 00 fd
                    ld      (hl),$00                        ;[00cd] 36 00
                    ld      de,$fd01                        ;[00cf] 11 01 fd
                    ld      bc,$00a9                        ;[00d2] 01 a9 00
                    ldir                                    ;[00d5] ed b0
                    ld      de,$ecab                        ;[00d7] 11 ab ec
                    ld      hl,$fd23                        ;[00da] 21 23 fd
                    ld      a,(de)                          ;[00dd] 1a
                    and     $7f                             ;[00de] e6 7f
                    ld      (hl),a                          ;[00e0] 77
                    inc     hl                              ;[00e1] 23
                    ld      a,(de)                          ;[00e2] 1a
                    and     $80                             ;[00e3] e6 80
                    ld      (hl),a                          ;[00e5] 77
                    ld      bc,$0024                        ;[00e6] 01 24 00
                    add     hl,bc                           ;[00e9] 09
                    inc     de                              ;[00ea] 13
                    ld      a,e                             ;[00eb] 7b
                    cp      $af                             ;[00ec] fe af
                    jr      nz,$00dd                        ;[00ee] 20 ed
                    jp      $ea00                           ;[00f0] c3 00 ea
                    ld      bc,$0000                        ;[00f3] 01 00 00
                    ld      (bc),a                          ;[00f6] 02
                    nop                                     ;[00f7] 00
                    call    nc,$0200                        ;[00f8] d4 00 02
                    nop                                     ;[00fb] 00
                    call    $0000                           ;[00fc] cd 00 00
                    nop                                     ;[00ff] 00
