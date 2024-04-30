kernel: 
	mov ebx,0xb8000    ; The video address
  mov al,'!'         ; The character to be print
  mov ah,0x0F        ; The color: white(F) on black(0)
  mov [ebx],ax        ; Put the character into the video memory
	jmp exit

exit:
	jmp exit
