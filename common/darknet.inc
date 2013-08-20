;-----------------------------------------------------------------------
speer       struct
    dwip    dword   ?
    dwport  dword   ?
    dwsocket            dword   ?
    dwconnected         dword   ?
    dwlastbrodcastsent  dword   ?
speer       ends

;-----------------------------------------------------------------------
snodedata   struct
    dwip        dword  ?
    dwport      dword  ?
    dwsock      dword  ?
    sock_listen sockaddr_in <>
snodedata   ends

;-----------------------------------------------------------------------
snetwork    struct
    peerlist    speer  MAX_PEERS   dup(<>)
    webcache    speer  MAX_PEERS   dup(<>)
    WSAData     WSADATA     <>
snetwork    ends

;-----------------------------------------------------------------------
snode    struct
    hid     dword       ?
    node    snodedata   <>
    network snetwork    <>
snode    ends

;-----------------------------------------------------------------------
