#include <stddef.h>
#include <stdint.h>
#include <sys/fb.h>
#include <sys/flanterm.h>

struct flanterm_context *ctx;

void tty_init(uint32_t *fb_ptr, size_t width, size_t height, size_t pitch) {
    ctx = flanterm_fb_simple_init(fb_ptr, width, height, pitch);
}

void tty_write(const char *buf, size_t size) {
    flanterm_write(ctx, buf, size);
}
