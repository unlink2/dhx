#include "ctx.h"
#include <scl/sclmem.h>
#include "config.h"

Context ctx_init() {
  Context ctx;
  scl_memset(&ctx, 0, sizeof(ctx));
  return ctx;
}

Context ctx_init_buffer(usize len) {
  Context ctx = ctx_init();
  ctx.buffer = alloc.malloc(len);
  return ctx;
}

void ctx_free(Context *ctx) {
  if (ctx->buffer) {
    alloc.free(ctx->buffer);
  }
}
