void main()
{
char KERNEL_NOTIFY[40] = "Welcome! This is the kernel's message!  ";
unsigned char* vga = (unsigned char*) 0xb8000;

	for (int i=0; *(KERNEL_NOTIFY + i) != '\0'; i++) 
	{
		*(vga + 2*i) = *(KERNEL_NOTIFY + i);
		*(vga + 2*i + 1) = i;
	}
	while(1);
}
