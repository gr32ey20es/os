;----------------------------------------------------=|
;             BIOS Print: Real-Mode Print             |
;                                                     |
; Before entering the Protected-Mode, we can use BIOS |
; to perform hardware initialization.                 |
;----------------------------------------------------=|
;             Ralf Brown's Interrupt List             |
;                                                     |
; For the use of an interrupt (e.g. Int 10/AH=0Eh),   |
; please refer to the RBIL.                           |
;----------------------------------------------------=|
;                    Flat Assembler                   |
;                                                     |
; If you have some trouble with an instruction, but   |
; you have no time, please refer to the FASM website. |
;-----------------------------------------------------|

bios_print:
	.init:
		push ax        ; store registers that were changed 
		               ; value during the function process
	.main:
		mov ah, 0x0E

		.loop: lodsb
			or al, al
			jz .return   ; if (al != 0)
			int 0x10     ; else
			jmp .loop

	.return:
		pop ax         ; restore registers
		ret
