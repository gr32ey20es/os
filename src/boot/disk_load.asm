;----------------------------------------------------=|
; Using Int 13/AH=02h to read disk sectors.           |
;----------------------------------------------------=|

disk_load:
	.init:
		pusha
		push es

	.main:
		mov ax, 0x0203
		mov cx, 0x0002
		mov dx, 0x0080   ; please be careful with the dl (drive number),
		                 ; it can be obtained from BIOS. If you don't
		                 ; know what the number is, leave it to BIOS  or 
		; mov dh, 0x00

		mov bx, 0x0000
		mov es, bx
		mov bx, 0x1000

		int 0x13
		
		jc disk_error

	.return:
		pop es
		popa
		ret

disk_error:
	.main:	
		mov si, DISK_ERROR
		call bios_print
		jmp $


DISK_ERROR: db "A disk read error occurred!", 13, 10, 0
