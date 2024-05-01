gdtr:
	dw gdt.end - gdt.start - 1
	dd gdt.start

gdt:
	.start:
		.null:
			dd 0x0
			dd 0x0

		.code:
			dw 0xffff      ; segment length (00-15)
			dw 0x0000      ; segment base   (00-15)
			db 0x0         ; segment base   (16-23)
			db 10011010b   ; flags (8 bits)
			db 11001111b   ; flags (4 bits), 
			               ; segment length (16-19)
			db 0x0         ; segment base   (24-31)

		.data:
			dw 0xffff
			dw 0x0000
			db 0x0
			db 10010010b
			db 11001111b
			db 0x0
	.end:


CODE_SEG equ gdt.code - gdt.start
DATA_SEG equ gdt.data - gdt.start
