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

void dump_byte(Config *c, FILE *in, FILE *out, usize address, u8 b) {
  if (addr_list_contains(&c->addrs, address)) {
    fprintf(out, "%s%02x%s ", c->highlight, b, c->unhighlight);
  } else {
    fprintf(out, "%02x ", b);
  }
}

void dump_char(Config *c, FILE *in, FILE *out, usize address, u8 b) {
  if (!isprint(b)) {
    b = '.';
  }

  if (addr_list_contains(&c->addrs, address)) {
    fprintf(out, "%s%c%s ", c->highlight, b, c->unhighlight);
  } else {
    fprintf(out, "%c ", b);
  }
}

u32 dump_row(Config *c, FILE *in, FILE *out, usize *address, DumpMode f) {
  fprintf(out, "%08zx\t", *address);
  u32 b = 0;
  usize i = c->rowlen;
  while (i > 0 && (b = getc(in)) != EOF) {
    f(c, in, out, *address, b);

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
