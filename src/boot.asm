org 0x7c00 ; the BIOS's role is over

jmp bios_main

include "lib/bios_print.inc"

bios_main:
	mov AH, 0x00
	int 0x16
	
	left_keystroke:
		cmp AL, 0x61
		jne right_keystroke
		
		mov AH, 0x03
		int 0x10
		
		sub DL, 1
		mov AH, 0x02
		int 0x10
		
		jmp bios_main

	right_keystroke:
		cmp AL, 0x64
		jne down_keystroke
		
		mov AH, 0x03
		int 0x10
		
		add DL, 1
		mov AH, 0x02
		int 0x10
	
		jmp bios_main

	down_keystroke:
		cmp AL, 0x73
		jne up_keystroke

		mov AH, 0x03
		int 0x10
		
		add DH, 1
		mov AH, 0x02
		int 0x10
		
		jmp bios_main

	up_keystroke:
		cmp AL, 0x77
		jne enter_keystroke
		
		mov AH, 0x03
		int 0x10
		
		sub DH, 1
		mov AH, 0x02
		int 0x10
		
		jmp bios_main

	enter_keystroke:
		cmp AL, 0x0D
		jne bios_main
		
		mov si, msg
		call bios_print
		jmp bios_main

bios_data:
	msg db "Hello, World!", 13, 10, 0
	times 510-($-$$) db 0
	dw 0xAA55		; boot signature
