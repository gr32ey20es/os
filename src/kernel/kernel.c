#include <stdint.h>
#include "../lib/vga.h"

void main()
{
	reset();
	while(1);
}



/*
 * I have no idea that a function must be 
 * below the main function to be executed.
 *
 * Before the function returns, RIP points to 
 * the next instruction of the boot process 
 * (0x7c__) instead of the kernel process ???
 *
 */

#include "../lib/port.c"
#include "../lib/vga.c"
