;-----------------------------------------------------------------------
.code
;-----------------------------------------------------------------------
xcopy   proc    uses esi edi ecx lpdest, lpsource, dwlen
    mov     esi, lpsource
    mov     edi, lpdest
    mov     ecx, dwlen
    rep     movsb
    ret
    
xcopy endp

;-----------------------------------------------------------------------
xfill   proc    uses esi ecx eax dwbyte:byte, lpdest, dwlen
    movzx   eax, dwbyte
    mov     edi, lpdest
    mov     ecx, dwlen
    rep     stosb
    ret
    
xfill   endp

strip_extention	proc	_lpszname
	pushad
	invoke  lstrlen, _lpszname
	mov		esi, _lpszname
	add     esi, eax
	.repeat
		mov     byte ptr [esi], 0
		dec     esi
	.until  byte ptr [esi] == '.'
	mov     byte ptr [esi], 0
	popad
	ret
strip_extention	endp

;-----------------------------------------------------------------------
_get_local_address  proc
    local   localhost[261]:byte

    invoke  gethostname, addr localhost, sizeof localhost
    invoke  gethostbyname, addr localhost
    
    mov     eax, (hostent ptr [eax]).h_list
    mov     eax, [eax]
    mov     eax, [eax]
    
    invoke  inet_ntoa, eax
    ret
_get_local_address endp

;-----------------------------------------------------------------------
