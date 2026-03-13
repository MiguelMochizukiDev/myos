; boot.asm
MBOOT_MAGIC     equ 0x1BADB002
MBOOT_FLAGS     equ 0x0
MBOOT_CHECKSUM  equ -(MBOOT_MAGIC + MBOOT_FLAGS)

section .multiboot
align 4
    dd MBOOT_MAGIC
    dd MBOOT_FLAGS
    dd MBOOT_CHECKSUM

section .text
extern kernel_main
global _start
_start:
    cli
    mov esp, stack_top
    call kernel_main
    hlt

section .bss
stack_bottom:
    resb 16384
stack_top: