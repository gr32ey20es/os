#include "vga.h"
#include "port.h"

/*
 * http://www.osdever.net/FreeVGA/home.htm
 * The website will give you enough information 
 * about VGA video programming.
 *
 */

/*-----------Cursor-----------*/
void set_cursor(uint16_t offset) 
{
	port_out8(CRTC_ADDR_REG, 0x0E);
	port_out8(CRTC_DATA_REG, offset >> 8);
	port_out8(CRTC_ADDR_REG, 0x0F);
	port_out8(CRTC_DATA_REG, offset & 0xff);
}

uint16_t get_cursor()
{
uint16_t offset = 0;

	port_out8(CRTC_ADDR_REG, 0x0E);
	offset |= port_in8(CRTC_DATA_REG) << 8;
	port_out8(CRTC_ADDR_REG, 0x0E);
	offset |= port_in8(CRTC_DATA_REG);

	return offset;
}

/*-----------Print-----------*/
void cprint(uint8_t chr)
{
uint16_t *vga = (uint16_t*) VGA_ADDR;
uint_fast16_t offset;

	offset = get_cursor();
	*(vga + offset) = chr;

	set_cursor(offset + 1);
}

void sprint(uint_fast8_t* str)
{
uint16_t *vga = (uint16_t*) VGA_ADDR;
uint_fast32_t i = 0;
uint_fast16_t offset, chr = 0x0700;

	offset = get_cursor();
	vga += offset;

	while(str[i] != '\0') 
	{
		*(vga + i) = (chr | str[i]);
		++i;
	}

	set_cursor(offset + i);
}

/*-----------Print with offset-----------*/
void coprint(uint16_t offset, uint8_t chr)
{
uint16_t *vga = (uint16_t*) VGA_ADDR;

	*(vga + offset) = chr;

	set_cursor(offset + 1);
}

void soprint(uint16_t offset, uint_fast8_t* str)
{
uint16_t *vga = (uint16_t*) VGA_ADDR;
uint_fast32_t i = 0;
uint_fast16_t chr = 0x0700;

	vga += offset;
	while(str[i] != '\0') 
	{
		*(vga + i) = (chr | str[i]);
		++i;
	}

	set_cursor(offset + i);
}

void reset()
{
uint16_t* vga = (uint16_t*) VGA_ADDR;
uint_fast32_t i, i_max;
uint_fast8_t command[COLS_MAX] =
	"Command[ r: read disk's sector, q: quit ]";

	soprint(0x00, command);

	i_max = COLS_MAX << 1;
	for (i = COLS_MAX; i < i_max; ++i)
		*(vga + i) = 0x073d;

	i_max = COLS_MAX * ROWS_MAX;
	for (i = COLS_MAX * ROWS_MIN; i < i_max; ++i)
		*(vga + i) = 0x0700;

	set_cursor(COLS_MAX << 1);
}
