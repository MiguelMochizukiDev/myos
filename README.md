# myOS

Small x86-64 hobby kernel that boots with GRUB (Multiboot2), switches to long mode, and runs a freestanding C kernel with VGA text output.

## Current Snapshot

- Build status: passing (`make clean && make` produces `myos.iso`)
- Boot path: GRUB -> Multiboot2 entry -> 32-bit setup -> long mode -> `kernel_main`
- Kernel output: VGA text mode with a minimal `kprintf`

## Implemented Architecture

### Boot and CPU mode transition

- Multiboot2 header is provided in the loader.
- Boot entry stores Multiboot2 registers (`EAX` magic, `EBX` info pointer).
- CPU feature checks for CPUID and long mode are present.
- Minimal page tables are created to enable long mode.
- A minimal 64-bit GDT is loaded before entering 64-bit code.

### Kernel runtime

- Freestanding C kernel entry: `kernel_main(uint32_t mb2_magic, multiboot2_info_t *mb2_info)`
- VGA text driver:
	- screen clear
	- color selection
	- character and string output
	- newline/tab handling
	- scrolling
- `kprintf` supports `%c`, `%s`, `%d`, `%u`, `%x`, `%p`, `%%`

## Roadmap Status (Based on Requested Stages)

| Stage | Name | Status | Notes |
|---|---|---|---|
| 0 | Toolchain and build | Done | Cross-compiler, Makefile, GRUB ISO generation, QEMU target in Makefile. |
| 1 | VGA output | Done | VGA driver + `kprintf` are working. |
| 2 | GDT (proper) | Partial | Minimal 64-bit GDT exists; no full descriptor set + TSS setup yet. |
| 3 | IDT and exceptions | Not started | No IDT entries or exception handlers yet. |
| 4 | Physical memory manager | Not started | Multiboot2 memory structures exist, allocator not implemented. |
| 5 | Virtual memory manager | Not started | Boot paging exists only for transition; no `map_page` / `unmap_page` API. |
| 6 | Kernel heap | Not started | No `kmalloc` / `kfree` yet. |
| 7 | APIC and timer | Not started | No Local APIC init or timer interrupts yet. |
| 8 | Processes | Not started | No PCB/context switch/scheduler yet. |
| 9 | Syscalls | Not started | No ring 3 transition or syscall entry path yet. |
| 10 | VFS + initrd | Not started | No filesystem abstraction yet. |
| 11 | ELF loader + shell | Not started | No user program loader or shell yet. |

## Known Gaps and Technical Debt

- The current long-mode paging setup is intentionally minimal and only enough for early boot.
- No interrupt handling path exists yet; faults will not be reported cleanly.
- No tests or CI pipeline yet.

## Build and Run

### Requirements

- `x86_64-elf-gcc`
- `x86_64-elf-ld`
- `nasm`
- `grub-mkrescue`
- `qemu-system-x86_64`

### Commands

```bash
make clean
make
make run
```

Debug run:

```bash
make debug
```

## Suggested Execution Plan for Next Stages

1. Finish Stage 2 by introducing a full GDT layout with kernel/user segments and TSS descriptor.
2. Implement Stage 3 with a complete IDT and first-class handlers for common CPU exceptions (#PF, #GP, #UD, #DF).
3. Use Multiboot2 memory map to build Stage 4 frame allocator.
4. Build Stage 5 page mapping API around the frame allocator.
5. Add Stage 6 heap (`kmalloc` / `kfree`) on top of virtual memory.
6. Move to Stage 7 timer interrupts to drive preemptive scheduling later.

## Project Goal

Progress from bootable educational kernel to a small multitasking system capable of loading and running user ELF binaries with a basic shell.