;----------------------------------------------------=|
; See Intel Manual Volume 3, Section 10.9 for more    |
; details about mode switching.                       |
;----------------------------------------------------=|

protectedMode_switching:
	.main:
		cli
		lgdt [gdtr]

		mov eax, cr0
		or eax, 0x01
		mov cr0, eax

		jmp CODE_SEG:.switch

use32
	.switch:
		mov ax, DATA_SEG
		mov ds, ax
		mov ss, ax
		mov es, ax
		mov fs, ax
		mov gs, ax
		
		mov ebp, 0x90000
		mov esp, ebp

	.return:
		call 0x1000        ; call the kernel loader


include "gdt.asm"
