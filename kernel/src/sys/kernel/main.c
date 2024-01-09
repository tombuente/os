#include <limine.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/gdt.h>
#include <sys/tty.h>

// Set the base revision to 1, this is recommended as this is the latest
// base revision described by the Limine boot protocol specification.
// See specification for further info.

LIMINE_BASE_REVISION(1)

// The Limine requests can be placed anywhere, but it is important that
// the compiler does not optimise them away, so, in C, they should
// NOT be made "static".

struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

// Halt and catch fire function.
static void hcf(void) {
    asm("cli");
    for (;;) {
        asm("hlt");
    }
}

// Kernel entry point
void _start(void) {
    // Ensure the bootloader actually understands our base revision (see spec).
    if (LIMINE_BASE_REVISION_SUPPORTED == false) {
        hcf();
    }

    // Ensure we got a framebuffer.
    if (framebuffer_request.response == NULL || framebuffer_request.response->framebuffer_count < 1) {
        hcf();
    }

    // Fetch the first framebuffer.
    struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];

    volatile uint32_t *fb_ptr = framebuffer->address;
    tty_init((uint32_t *)fb_ptr, framebuffer->width, framebuffer->height, framebuffer->pitch);
    const char msg[] = "Kernel\n";
    tty_write(msg, sizeof(msg));

    const char done_msg[] = " Done\n";

    const char gdt_msg[] = "Loading Global Descriptor Table...";
    tty_write(gdt_msg, sizeof(gdt_msg));
    gdt_setup();
    tty_write(done_msg, sizeof(done_msg));

    hcf();
}
