.686
.model flat, stdcall
option casemap:none

comment ~

See documentation for concept behind this code.

~

include  base.inc
includes memory, crypto, module

.code
_load_decriptor proc
    local   szfilename[257]:byte, hdecriptor, hdecryptor_out
    
    ; Decriptor file must be of the same name as base module except for extension.
    invoke  GetModuleFileName, 0, addr szfilename, sizeof szfilename
    invoke  strip_extention, addr szfilename
    
    ; With the name of the decriptor we try to open it. If it doesn't exists
    ; then we return FALSE
    invoke  _lopen, addr szfilename, 2
    .if     eax
        mov     hdecriptor, eax
        invoke  GetFileSize, eax, 0
        invoke  mem_alloc, eax
        mov     hdecriptor_out, eax

        ; Once open we take the first 32 bytes of the decriptor file
        ; which represent the encription key for symetric encryption
        ; of the rest of the decription file.
        invoke  rijndael_init, hdecriptor, DECRYPTOR_KEY_SIZE
        
        ; Dinamically creates a memory block for destination
        mov     eax, hdecriptor
        add     eax, DECRYPTOR_KEY_SIZE
        invoke  rijndael_decrypt, eax, hdecriptor_out
        
        mov     eax, hdecriptor_out
        .if     dword ptr [eax] == 'DECR' ; decriptor format mark
            add     eax, 4
            return  eax
            
        .else
            invoke  mem_dealloc, eax
            return  0
            
        .endif
    .else
        return eax
    .endif

	ret
_load_decriptor endp

_load_modules   proc    lpsmodules
    local hfind, hfile, w32find:WIN32_FIND_DATA
    
    invoke  crc32_init

    invoke  FindFirstFile, SADD("*.dll"), addr w32find
    mov     hfind, eax
    .while eax
        invoke  module_load, w32find.nSizeFileLow
        
        invoke  FindNextFile, hfind, addr w32find
    .endw
    
    invoke  CloseHandle, hfind
    
	ret
_load_modules endp

base_init   proc
    invoke  _load_decriptor
    .if     eax
        ; Load the modules on the decriptor list.
        ; This is done by listing all *.dll on current directory
        ; and matching file size and checksum with the ones
        ; on the decriptors.
        invoke  _load_modules, eax
        
        
    .else
        return  0
        
    .endif
	ret
base_init endp

base_deinit proc    lpsmodules
    

	ret
base_deinit endp

start:
    ; Loads decriptor file on current directory if exists. Decript module
    ; decriptor list with the key embebed in itself. Then proced to load
    ; available modules.
    ; Return a pointer to an array of pointers to smodule_decriptor structures.
    invoke  base_init

    ; If no error had occurred while loading modules.
    .if     eax
        mov     ebx, eax
        mov     hmodules, ebx
        .repeat
            inc     dwmodules_count
            
            ; Runs the action thread of the module.
            invoke  module_activate, [ebx]  
            
            add     ebx, 4
        .until  !dword ptr [ebx]
        
        ; Main message loop module<>base
        .while  (TRUE)
            m2m     dwmodules_active, dwmodules_count 
            mov     ebx, hmodules
            .repeat
                switch (smodule_decriptor ptr [ebx]).dwflag
                    case    MODULE_SHUTDOWN
                        mov (smodule_decriptor ptr [ebx]).dwflag, 0
                        invoke  module_unload, ebx
                        
                    case    MODULE_RESTART
                        mov (smodule_decriptor ptr [ebx]).dwflag, 0
                        invoke  module_unload, ebx
                        
                        invoke  module_load, ebx
                        invoke  module_activate, ebx
                    
                    case    0
                        dec     dwmodules_active
                endsw
                
                add ebx, 4
            .until  !dword ptr [ebx]
            
            .break  .if !dwmodules_active
            invoke  Sleep, BASE_SLEEP_INTERVAL
        .endw

    .else
        
    .endif
    
    ; Free used resources at loading modules and flushes decriptor updates.
    invoke  base_deinit, hmodules
    
    invoke  ExitProcess, 0
    
end start
