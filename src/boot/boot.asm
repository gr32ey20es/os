;----------------------------------------------------=|
;           Intel Software Developer's Manual         |
;                                                     |
; If you are stuck somewhere, go and read the Manual  |
; first. This Manual will give you almost all the     |
; information you need to solve your problems.        |
;----------------------------------------------------=|

org 0x7c00   ; (segment, offset) = (0x0000, 0x7c00) 
start: jmp boot

include "bios_print.asm"
include "disk_load.asm"
include "protectedMode_switching.asm"

use16
boot:
	.main:
		mov si, BOOT_NOTIFY
		call bios_print

		call disk_load

		jmp protectedMode_switching


BOOT_NOTIFY: db "Welcome! You are in the Real-Mode now!", 13, 10, 0

times 510-($-$$) db 0
dw 0xAA55   ; boot signature
