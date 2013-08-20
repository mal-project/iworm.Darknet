;-----------------------------------------------------------------------
include     routing.inc

;-----------------------------------------------------------------------
network_initialize  proc    ptrsnode
    mov     ebx, ptrsnode
    invoke  WSAStartup, 202h, addr (snode ptr [ebx]).network.WSAData
    .if     !eax
        invoke  _get_local_address
        .if     eax
            mov     (snode ptr [ebx]).node.dwip, eax
        .else
            return  -1
        .endif
    .else
        return  -1
        
    .endif

    ret
network_initialize  endp

;-----------------------------------------------------------------------
network_deinitialize    proc    ptrsnode   
    invoke  WSACleanup
    mov     ebx, ptrsnode
    invoke  xfill, 0, addr (snode ptr [ebx]).network, sizeof snode.network
    ret
network_deinitialize    endp

;-----------------------------------------------------------------------
_network_listen_thread  proc    ptrsnode
    local   sock_remote:sockaddr_in, dwret

    mov     ebx, ptrsnode
    invoke  listen, (snode ptr [ebx]).node.dwsock, MAX_CONNECTIONS
    .if eax != SOCKET_ERROR

        .while (1)
            mov     dwret, sizeof sockaddr_in
            mov     ebx, ptrsnode
            invoke  accept, (snode ptr [ebx]).node.dwsock, addr (snode ptr [ebx]).node.sock_listen, addr dwret
            .if eax != INVALID_SOCKET

                lea     edi, (snode ptr [ebx]).network.peerlist
                mov     dword ptr (speer ptr [edi]).dwsocket, eax
                
                invoke  send, (speer ptr [edi]).dwsocket, SADD("You're connected to a bot. This is wrost than u think"), 54, 0       
            .endif
        .endw
        invoke  Sleep, CONNECTION_SLEEP
    .endif
    invoke closesocket, (snode ptr [ebx]).node.dwsock
    ret
_network_listen_thread  endp

;-----------------------------------------------------------------------
network_listen          proc    ptrsnode

    mov     ebx, ptrsnode
    invoke  socket, AF_INET, SOCK_STREAM, IPPROTO_TCP
    mov     (snode ptr [ebx]).node.dwsock, eax

    mov     (snode ptr [ebx]).node.sock_listen.sin_family, AF_INET;
    
    ;mov     ebx, ptrsnode    
    ;m2m     sockAddrIn.sin_addr.S_un.S_addr, (speer ptr [ebx]).network.node.ip
    invoke  inet_addr, SADD("127.0.0.1")
    mov     (snode ptr [ebx]).node.sock_listen.sin_addr.S_un.S_addr, eax

    invoke  htons, 1337
    mov     (snode ptr [ebx]).node.sock_listen.sin_port, ax
    
    invoke  bind, (snode ptr [ebx]).node.dwsock, addr (snode ptr [ebx]).node.sock_listen, sizeof sockaddr_in
    .if     eax != SOCKET_ERROR

        invoke  CreateThread, 0, 0, addr _network_listen_thread, ptrsnode, 0, 0

    .elseif eax == SOCKET_ERROR
        return  -1

    .endif
    
    ret
network_listen          endp

;-----------------------------------------------------------------------
_network_connect_peer proc    ptrspeer, dwaddr, dwport
    local   dwsocket, sockAddrIn:sockaddr_in, dwcounter
    
    and     dwcounter, 0
    .while  dwcounter <= MAX_TRIES
        invoke  socket, PF_INET, SOCK_STREAM, 0
        mov     (speer ptr [ebx]).dwsocket, eax
        mov     dwsocket, eax

        mov     sockAddrIn.sin_family, AF_INET
        invoke  htons, dwport
        mov     sockAddrIn.sin_port, ax
        m2m     sockAddrIn.sin_addr, dwaddr
        
        invoke  connect, dwsocket, addr sockAddrIn, sizeof sockaddr_in
        .break  .if eax != SOCKET_ERROR

        invoke  Sleep, CONNECTION_SLEEP
        inc     dwcounter
    .endw
    
    .if     dwcounter <= MAX_TRIES
        mov     (speer ptr [ebx]).dwconnected, TRUE
        invoke  CreateThread, 0, 0, addr routing_listen, ptrspeer, 0, 0
        return  TRUE
    .else
        return  FALSE
    .endif
    ret
_network_connect_peer endp

;-----------------------------------------------------------------------
network_connect proc    ptrpeerlist, ptrsnode
    local   dwconnections

    and     dwconnections, 0

    mov     ebx, ptrpeerlist
    xor     ecx, ecx
    .repeat
        invoke  _network_connect_peer, ebx, (speer ptr [ebx]).dwip, (speer ptr [ebx]).dwport
        .if     eax
            inc     dwconnections
        .endif

        add     ebx, sizeof speer
        inc     ecx
    .until  ecx == MAX_PEERS
    
    mov     eax, dwconnections
    return  eax
    ret
network_connect endp

;-----------------------------------------------------------------------
