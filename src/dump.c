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

usize dump_gp1(Config *c, FILE *in, FILE *out, usize address, const u8 *b,
               usize len) {
  u8 fb = b[0];
  fprintf(out, "%02x", fb);

  return 1;
}

usize dump_gp2(Config *c, FILE *in, FILE *out, usize address, const u8 *b,
               usize len) {
  if (len < 2) {
    return dump_gp1(c, in, out, address, b, len);
  }

  u16 fb = *(u16 *)b;
  if (c->endianess == END_LITTLE) {
    fb = htole16(fb);
  } else if (c->endianess == END_BIG) {
    fb = htobe16(fb);
  }
  fprintf(out, "%04x", fb);

  return 2;
}

usize dump_gp4(Config *c, FILE *in, FILE *out, usize address, const u8 *b,
               usize len) {
  if (len < 4) {
    return dump_gp2(c, in, out, address, b, len);
  }

  u32 fb = *(u32 *)b;
  if (c->endianess == END_LITTLE) {
    fb = htole32(fb);
  } else if (c->endianess == END_BIG) {
    fb = htobe32(fb);
  }

  fprintf(out, "%08x", fb);

  return 4;
}

usize dump_gp8(Config *c, FILE *in, FILE *out, usize address, const u8 *b,
               usize len) {
  if (len < 8) {
    return dump_gp4(c, in, out, address, b, len);
  }

  u64 fb = *(u64 *)b;
  if (c->endianess == END_LITTLE) {
    fb = htole64(fb);
  } else if (c->endianess == END_BIG) {
    fb = htobe64(fb);
  }
  fprintf(out, "%016llx", fb);

  return 8;
}

usize dump_byte(Config *c, FILE *in, FILE *out, usize address, const u8 *b,
                usize len) {
  switch (c->output_grp) {
  case OG_2:
    return dump_gp2(c, in, out, address, b, len);
  case OG_4:
    return dump_gp4(c, in, out, address, b, len);
  case OG_8:
    return dump_gp8(c, in, out, address, b, len);
  case OG_1:
  default:
    return dump_gp1(c, in, out, address, b, len);
  }

  return 0;
}

usize dump_char_adv(Config *c, FILE *in, FILE *out, usize address, const u8 *b,
                    usize len, bool raw) {
  char fb = (char)b[0];
  if (!isprint(fb) && !raw) {
    fb = '.';
  }

  fprintf(out, "%c", fb);
  return 1;
}

usize dump_char(Config *c, FILE *in, FILE *out, usize address, const u8 *b,
                usize len) {
  return dump_char_adv(c, in, out, address, b, len, FALSE);
}

usize dump_char_raw(Config *c, FILE *in, FILE *out, usize address, const u8 *b,
                    usize len) {
  return dump_char_adv(c, in, out, address, b, len, TRUE);
}

void dump_addr_label(Config *c, FILE *out, usize address, usize *rowlen) {
  if (*rowlen >= c->rowlen) {
    *rowlen = 0;
  }
  if (!c->no_addr && *rowlen == 0) {
    if (address != c->base_addr) {
      // only add a new line when we are not at the beginning
      fprintf(out, "\n");
    }
    fprintf(out, "%08zx\t", address);
  }
}

usize dump_row(Config *c, FILE *in, FILE *out, usize *address, DumpMode f,
               u8 *b, usize len, usize *rowlen) {
  usize written = 0;

  for (usize i = 0; i < len;) {
    dump_addr_label(c, out, *address, rowlen);

    u8 *fb = b + i;
    fprintf(out, "%s", c->prefix);

    bool addr_contained = addr_list_contains(&c->addrs, *address);
    if (addr_contained) {
      fprintf(out, "%s", c->highlight);
    }
    usize w = f(c, in, out, *address, fb, len - written);
    if (addr_contained) {
      fprintf(out, "%s", c->unhighlight);
    }

    i += w;
    written += w;
    *rowlen += w;
    fprintf(out, "%s", c->separator);

    // does the frame end here?
    if (addr_list_contains(&c->frames, *address)) {
      *rowlen = 0;
    }

    (*address) += w;
  }

  return written;
}

void dump(Config *c, FILE *in, FILE *out, DumpMode f) {
  usize address = c->base_addr;

  usize rowlen = 0;
  usize read = 0;

  while ((read = fread(c->buffer, 1, c->rowlen, in)) > 0) {
    dump_row(c, in, out, &address, f, c->buffer, read, &rowlen);
  }

  fprintf(out, "\n");
}
