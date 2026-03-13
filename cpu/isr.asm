; isr.asm

global isr0,  isr1,  isr2,  isr3,  isr4,  isr5,  isr6,  isr7
global isr8,  isr9,  isr10, isr11, isr12, isr13, isr14, isr15
global isr16, isr17, isr18, isr19, isr20, isr21, isr22, isr23
global isr24, isr25, isr26, isr27, isr28, isr29, isr30, isr31
global isr32, isr33, isr34, isr35, isr36, isr37, isr38, isr39
global isr40, isr41, isr42, isr43, isr44, isr45, isr46, isr47

extern isr_handler

; Macro for exceptions WITHOUT error code: manual stacking
%macro ISR_NO_ERR 1
isr%1:
    push dword 0
    push dword %1   ; Interruption number
    jmp isr_common
%endmacro

; Macro pfor exceptions WITHOUT error code: manual stacking
%macro ISR_ERR 1
isr%1:
    push dword %1   ; Interruption number
    jmp isr_common
%endmacro

ISR_NO_ERR 0   ; Division Error
ISR_NO_ERR 1   ; Debug
ISR_NO_ERR 2   ; Non-maskable Interrupt
ISR_NO_ERR 3   ; Breakpoint
ISR_NO_ERR 4   ; Overflow
ISR_NO_ERR 5   ; Bound Range Exceeded
ISR_NO_ERR 6   ; Invalid Opcode
ISR_NO_ERR 7   ; Device Not Available
ISR_ERR    8   ; Double Fault
ISR_NO_ERR 9   ; Coprocessor Segment Overrun
ISR_ERR    10  ; Invalid TSS
ISR_ERR    11  ; Segment Not Present
ISR_ERR    12  ; Stack-Segment Fault
ISR_ERR    13  ; General Protection Fault
ISR_ERR    14  ; Page Fault
ISR_NO_ERR 15  ; Reserved
ISR_NO_ERR 16  ; x87 Floating-Point Exception
ISR_ERR    17  ; Alignment Check
ISR_NO_ERR 18  ; Machine Check
ISR_NO_ERR 19  ; SIMD Floating-Point Exception
ISR_NO_ERR 20  ; Virtualization Exception
ISR_NO_ERR 21  ; Control Protection Exception
ISR_NO_ERR 22  ; Reserved
ISR_NO_ERR 23  ; Reserved
ISR_NO_ERR 24  ; Reserved
ISR_NO_ERR 25  ; Reserved
ISR_NO_ERR 26  ; Reserved
ISR_NO_ERR 27  ; Reserved
ISR_NO_ERR 28  ; Reserved
ISR_NO_ERR 29  ; Reserved
ISR_ERR    30  ; Security Exception
ISR_NO_ERR 31  ; Reserved
ISR_NO_ERR 32  ; IRQ0 - PIT
ISR_NO_ERR 33  ; IRQ1 - Keyboard
ISR_NO_ERR 34  ; IRQ2 - Cascade
ISR_NO_ERR 35  ; IRQ3 - COM2
ISR_NO_ERR 36  ; IRQ4 - COM1
ISR_NO_ERR 37  ; IRQ5 - LPT2
ISR_NO_ERR 38  ; IRQ6 - Floppy
ISR_NO_ERR 39  ; IRQ7 - LPT1 / spurious
ISR_NO_ERR 40  ; IRQ8 - CMOS real-time clock
ISR_NO_ERR 41  ; IRQ9 - Free / ACPI
ISR_NO_ERR 42  ; IRQ10 - Free
ISR_NO_ERR 43  ; IRQ11 - Free
ISR_NO_ERR 44  ; IRQ12 - PS/2 Mouse
ISR_NO_ERR 45  ; IRQ13 - FPU
ISR_NO_ERR 46  ; IRQ14 - Primary ATA
ISR_NO_ERR 47  ; IRQ15 - Secondary ATA

isr_common:
    pusha                  ; saves eax, ecx, edx, ebx, esp, ebp, esi, edi

    push esp               ; passes pointer to registers_t as arg
    call isr_handler
    add esp, 4             ; clear argument at stack

    popa                   ; restores general purpose reigisters
    add esp, 8             ; discard int_no and err_code
    iret                   ; restores eip, cs, eflags