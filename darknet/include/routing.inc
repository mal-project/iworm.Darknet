;-----------------------------------------------------------------------
routing_listen  proc    ptrspeer
    local   data_buffer[1024]:byte, dwsocket

    mov     ebx, ptrspeer
    m2m     dwsocket, dword ptr (speer ptr [ebx]).dwsocket

    .while  (1)
        invoke  Sleep, 1000
        
        invoke  xfill, 0, addr data_buffer, sizeof data_buffer
        invoke  recv, dwsocket, addr data_buffer, sizeof data_buffer, 0
        .break  .if !eax || eax == SOCKET_ERROR

    .endw
    
    invoke  xfill, 0, ptrspeer, sizeof speer
    invoke closesocket, dwsocket
    ret
routing_listen  endp

;-----------------------------------------------------------------------
