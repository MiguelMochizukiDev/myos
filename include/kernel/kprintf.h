/* kprintf.h
 * Kernel formatted output
 *
 *
 * Provides a minimal printf-style function for use anywhere in the kernel
 * before a proper I/O subsystem exists. Output goes directly to the VGA
 * text buffer via vga_putchar().
 *
 * Supported format specifiers:
 *   %c   single character
 *   %s   null-terminated string
 *   %d   signed 64-bit integer, decimal
 *   %u   unsigned 64-bit integer, decimal
 *   %x   unsigned 64-bit integer, lowercase hexadecimal
 *   %p   pointer, printed as 0x followed by 16 hex digits
 *   %%   literal percent sign
 *
 * Width, precision, and length modifiers are not supported.
 */

#ifndef KERNEL_KPRINTF_H
#define KERNEL_KPRINTF_H

/**
 * Write a formatted string to the VGA text buffer.
 *
 * Args:
 * fmt - printf-style format string.
 * ... - variadic arguments matching the format specifiers.
 */
void kprintf(const char *fmt, ...);

#endif
