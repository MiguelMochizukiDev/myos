#include <kernel/types.h>
#include <kernel/multiboot2.h>

void kernel_main(uint32_t mb2_magic, multiboot2_info_t *mb2_info)
{
    (void)mb2_magic;
    (void)mb2_info;

    for (;;) {
        __asm__ volatile ("hlt");
    }
}
