#include <stdint.h>

uint64_t gdt[3];

struct gdtr {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

void gdt_setup() {
    gdt[0] = 0;
    gdt[1] = 0x00af9b000000ffff;
    gdt[2] = 0x00af93000000ffff;

    struct gdtr gdtr;
    gdtr.limit = sizeof(gdt) - 1;
    gdtr.base = (uint64_t)&gdt;

    asm volatile(
        "lgdt %0\n\t"
        "push $0x8\n\t"
        "lea reload_cs(%%rip), %%rax\n\t"
        "push %%rax\n\t"
        "lretq\n\t"
        "reload_cs:\n\t"
        "mov $0x10, %%eax\n\t"
        "mov %%eax, %%ds\n\t"
        "mov %%eax, %%es\n\t"
        "mov %%eax, %%fs\n\t"
        "mov %%eax, %%gs\n\t"
        "mov %%eax, %%ss\n\t"
        :
        : "m"(gdtr)
        : "rax", "memory");
}
