;-----------------------------------------------------------------------
.386
.model flat, stdcall
option casemap:none

;-----------------------------------------------------------------------
include     project.inc

;-----------------------------------------------------------------------
.code
includes    include\node.inc, include\network.inc

;-----------------------------------------------------------------------
start:
    ; 
    invoke  node_initialize, addr node
   
    ; inicializa winsock; obtiene la IP local, checkea puertos, etc
    invoke  network_initialize, addr node
    
    ; Escucha en un puerto determinado la llegada de conexiones de otros peers
    invoke  network_listen, addr node
    
    ; se intenta conectar con los nodos en la lista dada. Depende que tipo de lista se le de.
    ; si se conecta a algunos de los nodos pasa a escuchar conexiones entrantes etc.
    invoke  network_connect, addr node.network.peerlist, addr node
    .if     !eax
        invoke  network_connect, addr node.network.webcache, addr node
    .endif
    
    .if     eax
        ; libera recursos etc.
        invoke  network_deinitialize, addr node
    .endif
    
    invoke  node_deinitialize, addr node
    invoke  ExitProcess, eax
end start
  
;-----------------------------------------------------------------------
