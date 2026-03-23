#include <drivers/vga.h>
#include <kernel/types.h>

static uint16_t *const VGA_BUFFER = (uint16_t *)0xB8000;

static uint8_t  cursor_row;
static uint8_t  cursor_col;
static uint8_t  active_color;

static uint16_t make_entry(char c, uint8_t color)
{
    return (uint16_t)(uint8_t)c | ((uint16_t)color << 8);
}

static void put_entry(char c, uint8_t color, uint8_t row, uint8_t col)
{
    VGA_BUFFER[(uint16_t)row * VGA_COLS + (uint16_t)col] = make_entry(c, color);
}

static void scroll(void)
{
    uint16_t i;
    for (i = 0; i < (VGA_ROWS - 1) * VGA_COLS; i++) {
        VGA_BUFFER[i] = VGA_BUFFER[i + VGA_COLS];
    }
    for (i = (VGA_ROWS - 1) * VGA_COLS; i < VGA_ROWS * VGA_COLS; i++) {
        VGA_BUFFER[i] = make_entry(' ', active_color);
    }
    cursor_row = VGA_ROWS - 1;
}

void vga_init(void)
{
    active_color = (uint8_t)((VGA_COLOR_BLACK << 4) | VGA_COLOR_LIGHT_GREY);
    cursor_row   = 0;
    cursor_col   = 0;
    vga_clear();
}

void vga_clear(void)
{
    uint16_t i;
    for (i = 0; i < VGA_ROWS * VGA_COLS; i++) {
        VGA_BUFFER[i] = make_entry(' ', active_color);
    }
    cursor_row = 0;
    cursor_col = 0;
}

void vga_set_color(vga_color_t fg, vga_color_t bg)
{
    active_color = (uint8_t)(((uint8_t)bg << 4) | (uint8_t)fg);
}

void vga_putchar(char c)
{
    if (c == '\n') {
        cursor_col = 0;
        cursor_row++;
    } else if (c == '\r') {
        cursor_col = 0;
    } else if (c == '\t') {
        cursor_col = (uint8_t)((cursor_col + 8) & ~7);
        if (cursor_col >= VGA_COLS) {
            cursor_col = 0;
            cursor_row++;
        }
    } else {
        put_entry(c, active_color, cursor_row, cursor_col);
        cursor_col++;
        if (cursor_col >= VGA_COLS) {
            cursor_col = 0;
            cursor_row++;
        }
    }

    if (cursor_row >= VGA_ROWS) {
        scroll();
    }
}

void vga_puts(const char *s)
{
    while (*s) {
        vga_putchar(*s++);
    }
}
