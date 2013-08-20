.code
mem_alloc  proc    dwbytes
    pushad
    invoke  GlobalAlloc, GMEM_ZEROINIT, dwbytes
    return  eax
    popad
	ret
mem_alloc endp
mem_dealloc    proc    hmem
    pushad
    invoke  GlobalFree, hmem
    popad
	ret
mem_dealloc endp