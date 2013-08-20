include ..\common\project.inc

DECRYPTOR_KEY_SIZE  equ 32;for AES encription

.data?
    hmodules            dword   ?
    dwmodules_count     dword   ?
    dwmodules_active    dword   ?