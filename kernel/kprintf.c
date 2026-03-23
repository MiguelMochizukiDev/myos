#include <kernel/kprintf.h>
#include <kernel/types.h>
#include <drivers/vga.h>
#include <stdarg.h>

static void print_uint(uint64_t value, uint8_t base)
{
    static const char digits[] = "0123456789abcdef";
    char buf[64];
    uint8_t i = 0;

    if (value == 0) {
        vga_putchar('0');
        return;
    }

    while (value > 0) {
        buf[i++] = digits[value % base];
        value /= base;
    }

    while (i > 0) {
        vga_putchar(buf[--i]);
    }
}

static void print_int(int64_t value)
{
    if (value < 0) {
        vga_putchar('-');
        print_uint((uint64_t)(-value), 10);
    } else {
        print_uint((uint64_t)value, 10);
    }
}

static void print_pointer(uint64_t value)
{
    uint8_t i;
    static const char digits[] = "0123456789abcdef";
    char buf[16];

    vga_puts("0x");

    for (i = 0; i < 16; i++) {
        buf[i] = digits[(value >> (60 - i * 4)) & 0xF];
    }

    for (i = 0; i < 16; i++) {
        vga_putchar(buf[i]);
    }
}

void kprintf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    while (*fmt) {
        if (*fmt != '%') {
            vga_putchar(*fmt++);
            continue;
        }

        fmt++;

        switch (*fmt) {
        case 'c':
            vga_putchar((char)va_arg(args, int));
            break;
        case 's': {
            const char *s = va_arg(args, const char *);
            vga_puts(s ? s : "(null)");
            break;
        }
        case 'd':
            print_int(va_arg(args, int64_t));
            break;
        case 'u':
            print_uint(va_arg(args, uint64_t), 10);
            break;
        case 'x':
            print_uint(va_arg(args, uint64_t), 16);
            break;
        case 'p':
            print_pointer((uint64_t)va_arg(args, void *));
            break;
        case '%':
            vga_putchar('%');
            break;
        default:
            vga_putchar('%');
            vga_putchar(*fmt);
            break;
        }

        fmt++;
    }

    va_end(args);
}
