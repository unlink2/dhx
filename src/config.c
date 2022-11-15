#include "config.h"
#include <stdlib.h>
#include <scl/sclstr.h>
#include <scl/scllog.h>
#include <string.h>

Config config;
SclAlloc alloc;

Endianess end_map(const char *inp) {
  if (strncmp(inp, "little", scl_strlen("little")) == 0) {
    return END_LITTLE;
  } else if (strncmp(inp, "big", scl_strlen("big")) == 0) {
    return END_BIG;
  }

  scl_log_error("Unknown endianess: %s. Defaulting to native!\n", inp);
  return END_NATIVE;
}

OutputGroup og_map(const char *inp) {
  if (strncmp(inp, "1", scl_strlen("1")) == 0) {
    return OG_1;
  } else if (strncmp(inp, "2", scl_strlen("2")) == 0) {
    return OG_2;
  } else if (strncmp(inp, "4", scl_strlen("4")) == 0) {
    return OG_4;
  } else if (strncmp(inp, "8", scl_strlen("8")) == 0) {
    return OG_8;
  } else if (strncmp(inp, "c", scl_strlen("c")) == 0) {
    return OG_CHAR;
  } else if (strncmp(inp, "r", scl_strlen("r")) == 0) {
    return OG_RAW;
  }

  scl_log_error("Unknown output group: %s. Defaulting to 1!\n", inp);
  return OG_1;
}

#define default_row_len 16

Config config_init() {
  Config c = {NULL,
              0,
              addr_list_init(),
              addr_list_init(),
              "\x1b[7m",
              "\x1b[0m",
              FALSE,
              default_row_len,
              dump_byte,
              "",
              " ",
              malloc(default_row_len),
              OUT_FMT_8,
              END_NATIVE,
              OG_1,
              OK};
  alloc = scl_alloc_init(malloc, free);

  return c;
}

void config_set_rowlen(Config *c, usize len) {
  alloc.free(c->buffer);
  c->buffer = malloc(len);
  c->rowlen = len;
}

void config_apply_mode(Config *c, OutputGroup g) {
  c->output_grp = g;
  c->mode = dump_byte;
  switch (g) {
  case OG_CHAR:
    c->mode = dump_char;
    c->out_fmt = OUT_FMT_C;
    break;
  case OG_RAW:
    c->mode = dump_char_raw;
    c->out_fmt = OUT_FMT_R;
    break;
  case OG_1:
    c->out_fmt = OUT_FMT_8;
    break;
  case OG_2:
    c->out_fmt = OUT_FMT_16;
    break;
  case OG_4:
    c->out_fmt = OUT_FMT_32;
    break;
  case OG_8:
    c->out_fmt = OUT_FMT_64;
    break;
  default:
    break;
  }
}

void config_free(Config *c) {
  addr_list_free(&c->addrs);
  addr_list_free(&c->addrs);
  alloc.free(c->buffer);
}
