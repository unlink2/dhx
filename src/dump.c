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

usize dump_byte(Config *c, FILE *in, FILE *out, usize address, u8 *b, usize len) {
  u8 fb = b[0];
  if (addr_list_contains(&c->addrs, address)) {
    fprintf(out, "%s%02x%s", c->highlight, fb, c->unhighlight);
  } else {
    fprintf(out, "%02x", fb);
  }

  return 1;
}

usize dump_char_adv(Config *c, FILE *in, FILE *out, usize address, u8 *b, usize len, bool raw) {
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

usize dump_char(Config *c, FILE *in, FILE *out, usize address, u8 *b, usize len) {
  return dump_char_adv(c, in, out, address, b, len, FALSE);
}

usize dump_char_raw(Config *c, FILE *in, FILE *out, usize address, u8 *b, usize len) {
  return dump_char_adv(c, in, out, address, b, len, TRUE);
}

u32 dump_row(Config *c, FILE *in, FILE *out, usize *address, DumpMode f) {
  if (!c->no_addr) {
    fprintf(out, "%08zx\t", *address);
  }
  u32 b = 0;
  usize i = c->rowlen;
  while (i > 0 && (b = getc(in)) != EOF) {
    fprintf(out, "%s", c->prefix);
    f(c, in, out, *address, &b, 1);
    fprintf(out, "%s", c->separator);

    // does the frame end here?
    if (addr_list_contains(&c->frames, *address)) {
      (*address)++;
      break;
    }

    (*address)++;
    i--;
  }

  return b;
}

void dump(Config *c, FILE *in, FILE *out, DumpMode f) {
  usize address = c->base_addr;

  while (dump_row(c, in, out, &address, f) != EOF) {
    fprintf(out, "\n");
  }

  fprintf(out, "\n");
}
