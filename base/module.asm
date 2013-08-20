; module management
.code

module_load proc    lpsmodule_decriptor
    ; Try to match file size of win32find and all
    ; decriptors entries. If file size match then
    ; try to match check sum. If it match then
    ; load the module and mark it as MODULE_LOADED
    mov     esi, lpsmodules
    .while  [esi]
        mov     edx, w32find.nFileSizeLow
        .if     (smodule_decriptor ptr [esi]).dwsize == edx
            invoke  _lopen, addr w32find.cFileName, 1
            mov     hfile, eax
            
            invoke  crc32_compute, addr eax, w32find.nFileSizeLow
            .if     (smodule_decriptor ptr [esi]).dwchksum == eax
                

            .endif
            
            invoke  _lclose, hfile
        .endif
        
    .endw
	ret
module_load endp

module_unload   proc    lpsmodule_decriptor

	ret
module_unload endp

module_activate proc    lpsmodule_decriptor
    pushad
    mov     ebx, lpsmodule_decriptor
    invoke  CreateThread, 0, 0, (smodule_decriptor ptr [ebx]).dwaction, 0, 0, addr (smodule_decriptor ptr [ebx]).dwthread

    return  eax
    popad
	ret
module_activate endp