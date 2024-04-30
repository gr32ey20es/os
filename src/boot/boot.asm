org 0x7c00

start: jmp bootMain

bootMain:
	mov si, msg
	call biosPrint
	
	call kernelRead	
	
	jmp protectedSwitch	

kernelRead:
	push ax
	push bx
	push cx
	push es

	mov ax, 0x0201
	mov cx, 0x0002
	mov dx, 0x0080	

	mov bx, 0x0000
	mov es, bx
	mov bx, 0x1000

	int 0x13

	pop es
	pop cx
	pop bx
	pop ax
	
	ret


protectedSwitch:
	cli

	lgdt [gdtr]

	mov eax, cr0
	or eax, 0x01
	mov cr0, eax

	call 0x1000

biosPrint:
	push ax

	mov ah, 0x0E
	.loop: lodsb
		or al, al
		jz .return
		int 0x10
		jmp .loop
	
	.return:
		pop ax
		ret

bootData:
	msg db "Welcome!", 13, 10, 0
	gdtr: 
		dw 0xffff ; for limit
		dd 0x0000 ; for base	

	times 510-($-$$) db 0
	dw 0xAA55		; boot signature
