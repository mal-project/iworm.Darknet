;-----------------------------------------------------------------------
; Esta rutina la obtuve de los fuentes de z0mbie/29a y la modifique un poco
.code
; input  : esi=ptr data
; output : eax=crc, esi=ptr data + size + 1
; crc a given asciiz string
    xcrcsz      proc    lpszstr
                mov     esi, lpszstr
                push    ebx
                ;not    eax
                xor     eax, eax    ; 0
                not     eax         ; -1
        @@1:    xor     al, [esi]   ; Sn ^ eax
                inc     esi         ; n=0, n+=1
                mov     bl, 8       ; i=8
        @@2:    shr     eax, 1      ; Sn ^ eax / 2
                jnc     @@3         ; > 0
                xor     eax, 0EDB88320h
        @@3:    dec     bl          ; i-=1
                jnz     @@2
                cmp     byte ptr [esi], 0;end of string(data)
                jnz     @@1
                not     eax
                inc     esi
                pop     ebx
                ret
    xcrcsz      endp

;-----------------------------------------------------------------------