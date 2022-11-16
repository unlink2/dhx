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
  if (strncmp(inp, "1h", scl_strlen("1h")) == 0) {
    return OG_1H;
  } else if (strncmp(inp, "2h", scl_strlen("2h")) == 0) {
    return OG_2H;
  } else if (strncmp(inp, "4h", scl_strlen("4h")) == 0) {
    return OG_4H;
  } else if (strncmp(inp, "8h", scl_strlen("8h")) == 0) {
    return OG_8H;
  } else if (strncmp(inp, "1d", scl_strlen("1d")) == 0) {
    return OG_1D;
  } else if (strncmp(inp, "2d", scl_strlen("2d")) == 0) {
    return OG_2D;
  } else if (strncmp(inp, "4d", scl_strlen("4d")) == 0) {
    return OG_4D;
  } else if (strncmp(inp, "8d", scl_strlen("8d")) == 0) {
    return OG_8D;
  } else if (strncmp(inp, "c", scl_strlen("c")) == 0) {
    return OG_CHAR;
  } else if (strncmp(inp, "r", scl_strlen("r")) == 0) {
    return OG_RAW;
  } else if (strncmp(inp, "1b", scl_strlen("1b")) == 0) {
    return OG_1B;
  }
  /* else if (strncmp(inp, "2b", scl_strlen("2b")) == 0) {
    return OG_2B;
  } else if (strncmp(inp, "4b", scl_strlen("4b")) == 0) {
    return OG_4B;
  } else if (strncmp(inp, "8b", scl_strlen("8b")) == 0) {
    return OG_8B;
  }*/

  scl_log_error("Unknown output group: %s. Defaulting to 1!\n", inp);
  return OG_1H;
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
              OUT_FMT_8H,
              END_NATIVE,
              OG_1H,
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
  case OG_1H:
    c->out_fmt = OUT_FMT_8H;
    break;
  case OG_2H:
    c->out_fmt = OUT_FMT_16H;
    break;
  case OG_4H:
    c->out_fmt = OUT_FMT_32H;
    break;
  case OG_8H:
    c->out_fmt = OUT_FMT_64H;
    break;
  case OG_1D:
    c->out_fmt = OUT_FMT_1D;
    break;
  case OG_2D:
    c->out_fmt = OUT_FMT_2D;
    break;
  case OG_4D:
    c->out_fmt = OUT_FMT_4D;
    break;
  case OG_8D:
    c->out_fmt = OUT_FMT_8D;
    break;
  case OG_1B:
    c->out_fmt = OUT_FMT_1B;
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
