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
  ctx.buffer_start = alloc.malloc(len);
  ctx.buffer = ctx.buffer_start;

  ctx.max_len = len;
  ctx.len = 0;
  return ctx;
}

void ctx_line_wr(Context *ctx, usize n) { ctx->line_wr += n; }

void ctx_row_wr(Context *ctx, usize n) { ctx->row_wr += n; }

void ctx_free(Context *ctx) {
  if (ctx->buffer_start) {
    alloc.free(ctx->buffer_start);
  }
}
