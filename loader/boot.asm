; boot.asm — Multiboot2 header and long mode transition
;
; GRUB loads this file and jumps to _start in 32-bit protected mode.
; We verify CPU capabilities, build minimal identity-mapped page tables,
; enable long mode, and transfer control to kernel_main in 64-bit C.

MULTIBOOT2_MAGIC    equ 0xE85250D6
MULTIBOOT2_ARCH     equ 0
MULTIBOOT2_LENGTH   equ (multiboot2_header_end - multiboot2_header_start)
MULTIBOOT2_CHECKSUM equ (0x100000000 - (MULTIBOOT2_MAGIC + MULTIBOOT2_ARCH + MULTIBOOT2_LENGTH))

; ==============================
; Multiboot2 header
; ==============================
section .multiboot2
align 8
multiboot2_header_start:
    dd MULTIBOOT2_MAGIC
    dd MULTIBOOT2_ARCH
    dd MULTIBOOT2_LENGTH
    dd MULTIBOOT2_CHECKSUM
    ; End tag
    dw 0
    dw 0
    dd 8
multiboot2_header_end:

; ==============================
; BSS: stack and page table storage (zero-initialised by the linker script)
; ==============================
section .bss
align 16
stack_bottom:
    resb 16384
stack_top:

align 4096
pml4_table:  resb 4096   ; Page Map Level 4
pdp_table:   resb 4096   ; Page Directory Pointer
pd_table:    resb 4096   ; Page Directory (2 MiB huge pages)

; Holding area for Multiboot2 registers across the mode switch
mb2_magic:   resd 1
mb2_info:    resd 1

; ==============================
; 32-bit entry point
; ==============================
section .text
bits 32

global _start
_start:
    mov esp, stack_top
    mov [mb2_magic], eax
    mov [mb2_info],  ebx

    call check_cpuid
    call check_long_mode
    call setup_page_tables
    call enable_paging

    lgdt [gdt64.pointer]
    jmp  gdt64.code:long_mode_entry

    hlt

; ==============================
; CPUID availability check
; ==============================
check_cpuid:
    pushfd
    pop   eax
    mov   ecx, eax
    xor   eax, (1 << 21)
    push  eax
    popfd
    pushfd
    pop   eax
    push  ecx
    popfd
    cmp   eax, ecx
    je    .fail
    ret
.fail:
    hlt

; ==============================
; Long mode availability check
; ==============================
check_long_mode:
    mov  eax, 0x80000000
    cpuid
    cmp  eax, 0x80000001
    jb   .fail
    mov  eax, 0x80000001
    cpuid
    test edx, (1 << 29)
    jz   .fail
    ret
.fail:
    hlt

; ==============================
; Build page tables
; ==============================
setup_page_tables:
    mov eax, pdp_table
    or  eax, 0x3
    mov [pml4_table], eax

    mov eax, pd_table
    or  eax, 0x3
    mov [pdp_table], eax

    mov eax, 0x83
    mov [pd_table], eax
    ret

; ==============================
; Enable PAE
; load CR3
; set IA32_EFER.LME
; enable paging
; ==============================
enable_paging:
    mov eax, pml4_table
    mov cr3, eax

    mov eax, cr4
    or  eax, (1 << 5)       ; CR4.PAE
    mov cr4, eax

    mov ecx, 0xC0000080     ; IA32_EFER MSR
    rdmsr
    or  eax, (1 << 8)       ; EFER.LME
    wrmsr

    mov eax, cr0
    or  eax, (1 << 31)      ; CR0.PG — activates long mode
    mov cr0, eax
    ret

; ==============================
; Minimal 64-bit GDT
; Later to be replaced by kernel
; ==============================
section .rodata
gdt64:
    dq 0
.code: equ $ - gdt64
    dq (1<<43) | (1<<44) | (1<<47) | (1<<53)
.pointer:
    dw $ - gdt64 - 1
    dq gdt64

; ==============================
; 64-bit entry
; ==============================
bits 64
default rel
long_mode_entry:
    mov ax, 0
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov edi, dword [rel mb2_magic]
    mov esi, dword [rel mb2_info]

    extern kernel_main
    call   kernel_main

.halt:
    cli
    hlt
    jmp .halt
