#include <kernel/types.h>
#include <kernel/multiboot2.h>
#include <kernel/kprintf.h>
#include <drivers/vga.h>

void kernel_main(uint32_t mb2_magic, multiboot2_info_t *mb2_info)
{
    (void)mb2_info;

    vga_init();

    kprintf("myOS\n");
    kprintf("Multiboot2 magic: 0x%x\n", (uint64_t)mb2_magic);

    if (mb2_magic == MULTIBOOT2_BOOTLOADER_MAGIC) {
        kprintf("Bootloader OK\n");
    } else {
        vga_set_color(VGA_COLOR_RED, VGA_COLOR_BLACK);
        kprintf("Bad magic!\n");
    }

    kprintf("Halting.\n");

    for (;;) {
        __asm__ volatile ("hlt");
    }
}
