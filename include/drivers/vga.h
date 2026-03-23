/**
 * vga.h
 * VGA text mode driver interface
 *
 * Provides direct access to the VGA text buffer mapped at physical address
 * 0xB8000. The buffer is a 80x25 grid of 16-bit cells where the low byte
 * holds the ASCII character and the high byte holds the color attribute.
 *
 * Color attribute layout:
 *   bits [7:4] = background color (0-7, bit 7 enables blink on some hardware)
 *   bits [3:0] = foreground color (0-15)
 */

#ifndef DRIVERS_VGA_H
#define DRIVERS_VGA_H

#include <kernel/types.h>

/* Width of the VGA text buffer in columns */
#define VGA_COLS 80

/* Height of the VGA text buffer in rows */
#define VGA_ROWS 25

/**
 * Standard VGA 4-bit color palette
 *
 * Values 0-7 are dark variants; 8-15 are bright variants.
 * Background color is limited to 0-7 in standard text mode.
 */
typedef enum {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_YELLOW = 14,
    VGA_COLOR_WHITE = 15,
} vga_color_t;

/**
 * Initialise the VGA driver
 *
 * Clears the screen and sets the default color to light grey on black.
 * Must be called before any other vga_* function.
 */
void vga_init(void);

/**
 * Clear the entire screen with the current background color.
 */
void vga_clear(void);

/**
 * Set the active foreground and background colors.
 *
 * All subsequent calls to vga_putchar() and vga_puts() will use these
 * colors until changed again.
 *
 * Args:
 * fg - Foreground color (0-15).
 * bg - Background color (0-7).
 */
void vga_set_color(vga_color_t fg, vga_color_t bg);

/**
 * Write a single character at the current cursor position.
 *
 * Handles the following control characters:
 *   '\n'  advances to the next row
 *   '\r'  returns to column 0
 *   '\t'  advances to the next 8-column tab stop
 *
 * Scrolls the buffer up by one row when the cursor passes the last row.
 *
 * Args:
 * c - ASCII character to write.
 */
void vga_putchar(char c);

/**
 * Write a null-terminated string at the current cursor position.
 *
 * Equivalent to calling vga_putchar() for each character in @p s.
 *
 * Args:
 * s - Null-terminated ASCII string.
 */
void vga_puts(const char *s);

#endif
