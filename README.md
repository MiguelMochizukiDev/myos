# myOS

A tiny 32-bit toy kernel for learning OS fundamentals. Boots via Multiboot, sets up GDT/IDT, remaps the PIC, installs CPU exception/IRQ stubs, and prints to VGA text mode.

## Features

- Multiboot entry with minimal loader (NASM)
- GDT setup and reload
- IDT with exception stubs (0–31) and IRQ stubs (32–47)
- PIC remap + EOI handling
- VGA text mode driver with colors, scrolling, hex/dec printing
- Simple exception trigger in `kernel_main` (divide-by-zero) for testing

## Prerequisites

- `i686-elf-gcc` and binutils cross toolchain
- `nasm`
- `grub-mkrescue` (needs `xorriso` installed)
- `qemu-system-i386`

## Build


```sh
make
```

Produces `myos.bin` at the repository root.

## Run (via QEMU)

```sh
make run
```

This builds the ISO (`myos.iso`) and boots it with QEMU, logging interrupts/resets to `qemu.log`.

## ISO only

```sh
make iso
```

Places `myos.bin` under `isodir/boot/` and produces `myos.iso`.

## Clean

```sh
make clean
```

Removes built objects and `myos.bin`.

## Repository layout

- `boot/` — Multiboot-compliant loader
- `cpu/` — GDT/IDT/ISR/PIC code (C + assembly stubs)
- `drivers/` — VGA text driver
- `kernel/` — `kernel_main`
- `include/` — Public headers
- `isodir/` — GRUB layout for ISO generation
- `linker.ld` — Kernel linker script
- `Makefile` — Build, ISO, run targets

## Notes

- Hardware IRQs start masked; unmask in PIC if you want timer/keyboard testing.
- The default kernel path intentionally triggers ISR0 (divide-by-zero) after enabling interrupts so you can see exception handling on boot.

## License

This project is under [MIT License](./LICENSE)