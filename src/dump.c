#include "dump.h"
#include "config.h"
#include <ctype.h>

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

usize dump_byte(Config *c, FILE *in, FILE *out, usize address, u8 *b,
                usize len) {
  u8 fb = b[0];
  if (addr_list_contains(&c->addrs, address)) {
    fprintf(out, "%s%02x%s", c->highlight, fb, c->unhighlight);
  } else {
    fprintf(out, "%02x", fb);
  }

  return 1;
}

usize dump_char_adv(Config *c, FILE *in, FILE *out, usize address, u8 *b,
                    usize len, bool raw) {
  char fb = b[0];
  if (!isprint(fb) && !raw) {
    fb = '.';
  }

  if (addr_list_contains(&c->addrs, address)) {
    fprintf(out, "%s%c%s", c->highlight, fb, c->unhighlight);
  } else {
    fprintf(out, "%c", fb);
  }
  return 1;
}

usize dump_char(Config *c, FILE *in, FILE *out, usize address, u8 *b,
                usize len) {
  return dump_char_adv(c, in, out, address, b, len, FALSE);
}

usize dump_char_raw(Config *c, FILE *in, FILE *out, usize address, u8 *b,
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
    usize w = f(c, in, out, *address, fb, 1);
    i += w;
    written += w;
    *rowlen += w;
    fprintf(out, "%s", c->separator);

    // does the frame end here?
    if (addr_list_contains(&c->frames, *address)) {
      *rowlen = 0;
    }

    (*address)++;
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
