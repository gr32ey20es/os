#ifndef VGA_H
#define VGA_H

#define VGA_ADDR 0xb8000
#define COLS_MIN 0x00000
#define COLS_MAX 0x00050
#define ROWS_MIN 0x00002
#define ROWS_MAX 0x00019

void reset();
void vga();

#endif
