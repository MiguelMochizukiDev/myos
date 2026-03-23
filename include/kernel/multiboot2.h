/**
 * multiboot2.h
 * Multiboot2 boot information structures
 *
 * When GRUB transfers control to the kernel, EAX holds the Multiboot2
 * magic value (MULTIBOOT2_BOOTLOADER_MAGIC) and EBX holds the physical
 * address of the Multiboot2 information structure described here.
 *
 * Reference: https://www.gnu.org/software/grub/manual/multiboot2/
 */

#ifndef KERNEL_MULTIBOOT2_H
#define KERNEL_MULTIBOOT2_H

#include <kernel/types.h>

/* Magic value in EAX on entry from a Multiboot2-compliant bootloader */
#define MULTIBOOT2_BOOTLOADER_MAGIC 0x36d76289U

/* Tag types embedded in the Multiboot2 info structure */
#define MULTIBOOT2_TAG_TYPE_END 0
#define MULTIBOOT2_TAG_TYPE_CMDLINE 1
#define MULTIBOOT2_TAG_TYPE_MEMINFO 4
#define MULTIBOOT2_TAG_TYPE_MMAP 6
#define MULTIBOOT2_TAG_TYPE_FRAMEBUFFER 8

/* Memory map entry types */
#define MULTIBOOT2_MEMORY_AVAILABLE 1
#define MULTIBOOT2_MEMORY_RESERVED 2
#define MULTIBOOT2_MEMORY_ACPI_RECLAIMABLE 3
#define MULTIBOOT2_MEMORY_NVS 4
#define MULTIBOOT2_MEMORY_BADRAM 5

/**
 * Fixed header at the start of the Multiboot2 info structure
 *
 * Followed by a sequence of variable-length tags, 
 * each aligned to an 8-byte boundary.
 */
typedef struct {
	uint32_t total_size;
	uint32_t reserved;
} __attribute__((packed)) multiboot2_info_t;

/**
 * Common header shared by every tag
 */
typedef struct {
    uint32_t type;
    uint32_t size;
} __attribute__((packed)) multiboot2_tag_t;

/**
 * Basic upper/lower memory info tag (type 4)
 */
typedef struct {
    uint32_t type;
    uint32_t size;
    uint32_t mem_lower; 
    uint32_t mem_upper; 
} __attribute__((packed)) multiboot2_tag_meminfo_t;

/**
 * A single entry in the memory map tag
 */
typedef struct {
    uint64_t base_addr;
    uint64_t length;
    uint32_t type;
    uint32_t reserved;
} __attribute__((packed)) multiboot2_mmap_entry_t;

/**
 * Memory map tag (type 6)
 *
 * Entries follow immediately after the tag header and are each
 * entry_size bytes wide (use this, not sizeof, to iterate).
 */
typedef struct {
    uint32_t type;
    uint32_t size;
    uint32_t entry_size;
    uint32_t entry_version;
} __attribute__((packed)) multiboot2_tag_mmap_t;

#endif
