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

  scl_log_error("Unknown endianess: %s. Defaulting to little!\n", inp);
  return END_LITTLE;
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
              END_LITTLE,
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

void config_free(Config *c) {
  addr_list_free(&c->addrs);
  addr_list_free(&c->addrs);
  alloc.free(c->buffer);
}
