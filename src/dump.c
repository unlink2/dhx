#include "dump.h"
#include "config.h"
#include <ctype.h>
#include <endian.h>

DumpHooks dump_hooks_init(DumpHook ls, DumpHook le, DumpHook rs, DumpHook re) {
  DumpHooks hooks = {ls, le, rs, re};

  return hooks;
}

void dump_hook_nop(Config *c, Context *ctx, FILE *in, FILE *out) {}

FILE *open_input(char *path) {
  if (path) {
    return fopen(path, "rbe");
  }
  return stdin;
}

FILE *open_output(char *path) {
  if (path) {
    return fopen(path, "wbe");
  }
  return stdout;
}

void close(FILE *f) {
  if (f != stdin && f != stdout && f != stderr && f) {
    fclose(f);
  }
}

void dump_highlight(Config *c, Context *ctx, FILE *in, FILE *out) {
  bool addr_contained = addr_list_contains(&c->addrs, ctx->address);
  if (addr_contained) {
    ctx_row_wr(ctx, fprintf(out, "%s", c->highlight));
  }
}

void dump_unhighlight(Config *c, Context *ctx, FILE *in, FILE *out) {
  bool addr_contained = addr_list_contains(&c->addrs, ctx->address);
  if (addr_contained) {
    ctx_row_wr(ctx, fprintf(out, "%s", c->unhighlight));
  }
}

usize dump_gp1b(Config *c, Context *ctx, FILE *in, FILE *out) {
  u8 fb = ctx->buffer[0];

  for (usize i = 8; i > 0; i--) {
    bool bit = (fb & (u8)((u8)1 << (i - 1)));
    if (bit) {
      ctx_row_wr(ctx, fprintf(out, "1"));
    } else {
      ctx_row_wr(ctx, fprintf(out, "0"));
    }
  }

  return 1;
}

usize dump_gp1(Config *c, Context *ctx, FILE *in, FILE *out) {
  u8 fb = ctx->buffer[0];
  ctx_row_wr(ctx, fprintf(out, c->out_fmt, fb));

  return 1;
}

usize dump_gp2(Config *c, Context *ctx, FILE *in, FILE *out) {
  if (ctx->len < 2) {
    return dump_gp1(c, ctx, in, out);
  }

  u16 fb = *(u16 *)ctx->buffer;
  if (c->endianess == END_LITTLE) {
    fb = htole16(fb);
  } else if (c->endianess == END_BIG) {
    fb = htobe16(fb);
  }
  ctx_row_wr(ctx, fprintf(out, c->out_fmt, fb));

  return 2;
}

usize dump_gp4(Config *c, Context *ctx, FILE *in, FILE *out) {
  if (ctx->len < 4) {
    return dump_gp2(c, ctx, in, out);
  }

  u32 fb = *(u32 *)ctx->buffer;
  if (c->endianess == END_LITTLE) {
    fb = htole32(fb);
  } else if (c->endianess == END_BIG) {
    fb = htobe32(fb);
  }

  ctx_row_wr(ctx, fprintf(out, c->out_fmt, fb));

  return 4;
}

usize dump_gp8(Config *c, Context *ctx, FILE *in, FILE *out) {
  if (ctx->len < 8) {
    return dump_gp4(c, ctx, in, out);
  }

  u64 fb = *(u64 *)ctx->buffer;
  if (c->endianess == END_LITTLE) {
    fb = htole64(fb);
  } else if (c->endianess == END_BIG) {
    fb = htobe64(fb);
  }
  ctx_row_wr(ctx, fprintf(out, c->out_fmt, fb));

  return 8;
}

usize dump_byte(Config *c, Context *ctx, FILE *in, FILE *out) {
  switch (c->output_grp) {
  case OG_2H:
  case OG_2D:
    return dump_gp2(c, ctx, in, out);
  case OG_4H:
  case OG_4D:
    return dump_gp4(c, ctx, in, out);
  case OG_8H:
  case OG_8D:
    return dump_gp8(c, ctx, in, out);
  case OG_1B:
    return dump_gp1b(c, ctx, in, out);
  case OG_1H:
  case OG_1D:
  default:
    return dump_gp1(c, ctx, in, out);
  }

  return 0;
}

usize dump_char_adv(Config *c, Context *ctx, FILE *in, FILE *out, bool raw) {
  char fb = (char)ctx->buffer[0];
  if (!isprint(fb) && !raw) {
    fb = '.';
  }

  ctx_row_wr(ctx, fprintf(out, c->out_fmt, fb));
  return 1;
}

usize dump_char(Config *c, Context *ctx, FILE *in, FILE *out) {
  return dump_char_adv(c, ctx, in, out, FALSE);
}

usize dump_char_raw(Config *c, Context *ctx, FILE *in, FILE *out) {
  return dump_char_adv(c, ctx, in, out, TRUE);
}

void dump_next_line(Config *c, Context *ctx, FILE *out) {
  if (ctx->rowlen >= c->rowlen) {
    ctx->rowlen = 0;
  }

  if (ctx->address != c->base_addr && ctx->rowlen == 0) {
    // only add a new line when we are not at the beginning
    ctx_line_wr(ctx, fprintf(out, "\n"));
  }
}

void dump_addr_label(Config *c, Context *ctx, FILE *out) {
  if (!c->no_addr && ctx->rowlen == 0) {
    ctx_row_wr(ctx, fprintf(out, "%016zx%s", ctx->address, c->separator));
  }
}

// dumps exactly one row and calls all corresponding hooks
usize dump_row(Config *c, Context *ctx, FILE *in, FILE *out, DumpMode f) {
  usize written = 0;
  c->hooks.on_line_start(c, ctx, in, out);

  // reset buffer to start
  ctx->buffer = ctx->buffer_start;
  ctx->row_wr = 0;
  ctx->line_wr = 0;

  // restart output from last written counter
  // this should be set to 0 before calling
  // when new data is fetched.
  // it should be left unchanged when an early bail occurs
  // e.g. due to a frame end
  for (usize i = ctx->written; i < ctx->len;) {
    dump_next_line(c, ctx, out);
    dump_addr_label(c, ctx, out);

    ctx->buffer = ctx->buffer_start + i;
    ctx_row_wr(ctx, fprintf(out, "%s", c->prefix));

    dump_highlight(c, ctx, in, out);
    usize w = f(c, ctx, in, out);
    dump_unhighlight(c, ctx, in, out);

    i += w;
    written += w;
    ctx->rowlen += w;
    ctx_row_wr(ctx, fprintf(out, "%s", c->separator));

    // does the frame end here?
    if (addr_list_contains(&c->frames, ctx->address)) {
      // if so we end the row, increment the address and bail
      ctx->rowlen = 0;
      ctx->address += w;
      break;
    }

    ctx->address += w;

    // end of dump region?
    if (ctx->address >= c->end_addr) {
      break;
    }
  }

  c->hooks.on_line_end(c, ctx, in, out);

  return written;
}

void dump(Config *c, FILE *in, FILE *out, DumpMode f) {
  // reset context
  // it is only valid while
  // this function is active
  Context ctx = ctx_init_buffer(c->rowlen);
  ctx.address = c->base_addr;
  ctx.rowlen = 0;

  // skip until we reach start address
  while (ctx.address < c->start_addr &&
         (ctx.len = fread(ctx.buffer_start, 1, 1, in)) > 0) {
    ctx.address++;
  }

  while ((ctx.len = fread(ctx.buffer_start, 1, c->rowlen, in)) > 0) {
    // reset written counter
    ctx.written = 0;
    while (ctx.written < ctx.len) {
      if (ctx.address >= c->end_addr) {
        goto dump_end_reached;
      }
      ctx.written += dump_row(c, &ctx, in, out, f);
    }
  }
dump_end_reached:

  ctx_line_wr(&ctx, fprintf(out, "\n"));

  // clean up the context to
  // ensure we are not leaking anything
  ctx_free(&ctx);
}
