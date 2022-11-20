#include "dump.h"
#include "config.h"
#include <ctype.h>
#include <endian.h>

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

usize dump_gp1b(Config *c, Context *ctx, FILE *in, FILE *out) {
  u8 fb = ctx->buffer[0];

  for (usize i = 8; i > 0; i--) {
    bool bit = (fb & (u8)((u8)1 << (i - 1)));
    if (bit) {
      fprintf(out, "1");
    } else {
      fprintf(out, "0");
    }
  }

  return 1;
}

usize dump_gp1(Config *c, Context *ctx, FILE *in, FILE *out) {
  u8 fb = ctx->buffer[0];
  fprintf(out, c->out_fmt, fb);

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
  fprintf(out, c->out_fmt, fb);

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

  fprintf(out, c->out_fmt, fb);

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
  fprintf(out, c->out_fmt, fb);

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

  fprintf(out, c->out_fmt, fb);
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
    fprintf(out, "\n");
  }
}

void dump_addr_label(Config *c, Context *ctx, FILE *out) {
  if (!c->no_addr && ctx->rowlen == 0) {
    fprintf(out, "%016zx%s", ctx->address, c->separator);
  }
}

usize dump_row(Config *c, Context *ctx, FILE *in, FILE *out, DumpMode f) {
  usize written = 0;

  // reset buffer to start
  ctx->buffer = ctx->buffer_start;

  for (usize i = 0; i < ctx->len;) {
    dump_next_line(c, ctx, out);
    dump_addr_label(c, ctx, out);

    ctx->buffer = ctx->buffer_start + i;
    fprintf(out, "%s", c->prefix);

    bool addr_contained = addr_list_contains(&c->addrs, ctx->address);
    if (addr_contained) {
      fprintf(out, "%s", c->highlight);
    }
    usize w = f(c, ctx, in, out);
    if (addr_contained) {
      fprintf(out, "%s", c->unhighlight);
    }

    i += w;
    written += w;
    ctx->rowlen += w;
    fprintf(out, "%s", c->separator);

    // does the frame end here?
    if (addr_list_contains(&c->frames, ctx->address)) {
      ctx->rowlen = 0;
    }

    ctx->address += w;

    if (ctx->address >= c->end_addr) {
      break;
    }
  }

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
    if (ctx.address >= c->end_addr) {
      break;
    }
    dump_row(c, &ctx, in, out, f);
  }

  fprintf(out, "\n");

  // clean up the context to
  // ensure we are not leaking anything
  ctx_free(&ctx);
}
