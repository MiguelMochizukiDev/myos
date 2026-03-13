/* vga.h */

#ifndef VGA_H
#define VGA_H

#include <stdint.h>
#include <stddef.h>

/* VGA colors */
typedef enum {
	VGA_BLACK 		= 0,
	VGA_BLUE 		= 1,
	VGA_GREEN 		= 2,
	VGA_CYAN 		= 3,
	VGA_RED 		= 4,
	VGA_MAGENTA 		= 5,
	VGA_BROWN 		= 6,
	VGA_LIGHT_GREY 		= 7,
	VGA_DARK_GREY 		= 8,
	VGA_LIGHT_BLUE 		= 9,
	VGA_LIGHT_GREEN 	= 10,
	VGA_LIGHT_CYAN 		= 11,
	VGA_LIGHT_RED 		= 12,
	VGA_LIGHT_MAGENTA 	= 13,
	VGA_YELLOW 		= 14,
	VGA_WHITE 		= 15,
} vga_color_t;

void terminal_init(void);
void terminal_clear(void);
void terminal_setcolor(vga_color_t fg, vga_color_t bg);
void kputchar(char c);
void kprint(const char *str);
void kprint_uint(uint32_t n, uint8_t base);
void kprint_int(int32_t n);
void kprint_hex(uint32_t n);

#endif
