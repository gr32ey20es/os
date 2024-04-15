bios_print:
	push ax

	print_loop:
		lodsb
		; check if al not NULL
		or al, al
		jz print_ret
		; display a character
		mov ah, 0x0E
		int 0x10
		
		jmp print_loop

	print_ret:
		pop ax
		ret
