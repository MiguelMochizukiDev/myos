# myOS

A tiny 32-bit toy kernel for learning OS fundamentals. Boots via GRUB/Multiboot,
sets up GDT/IDT, remaps the PIC, handles CPU exceptions and hardware IRQs, and
prints to VGA text mode.

## Features

- Multiboot entry point (NASM)
- GDT with null, code and data segments
- IDT with exception stubs (0–31) and IRQ stubs (32–47)
- PIC remap + mask + EOI handling
- Registrable IRQ handler table (`irq_register`)
- PIT tick counter via IRQ0
- VGA text mode driver with colors, scrolling, hex/dec printing

## Prerequisites

- `i686-elf-gcc` cross toolchain
- `nasm`
- `grub-mkrescue` (requires `xorriso`)
- `qemu-system-i386`

## Build & Run

| Command      | Description                              |
|-------------|------------------------------------------|
| `make`       | Compile and link `myos.bin`              |
| `make iso`   | Build `myos.iso` (GRUB bootable)         |
| `make run`   | Build ISO and boot in QEMU               |
| `make clean` | Remove all build artifacts               |

## Repository layout

```
myos/
├── boot/       — Multiboot entry point (NASM)
├── cpu/        — Architecture layer: GDT, IDT, ISR stubs, PIC
├── drivers/    — Device drivers: VGA, PIT timer
├── kernel/     — kernel_main
├── include/
│   ├── cpu/    — Headers for cpu/ modules
│   └── drivers/— Headers for drivers/ modules
├── isodir/     — GRUB layout for ISO generation
├── linker.ld   — Kernel linker script
└── Makefile    — Build, ISO, run, clean targets
```

## Adding a new IRQ handler

1. Create `drivers/mydevice.c` with a handler function
2. Call `irq_register(n, handler)` in your `mydevice_init()`
3. Call `pic_unmask(n)` to enable the IRQ
4. Call `mydevice_init()` in `kernel_main` after `idt_init()`

## Roadmap

- [x] VGA text mode driver
- [x] GDT
- [x] IDT + CPU exception handler
- [x] PIC remap + IRQ dispatch
- [x] PIT tick counter
- [ ] Keyboard driver (IRQ1)
- [ ] Physical memory manager
- [ ] Paging
- [ ] Scheduler
- [ ] Syscalls + userspace

## License

[MIT](./LICENSE)
