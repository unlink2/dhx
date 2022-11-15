#include "config.h"
#include <stdlib.h>

Config config;
SclAlloc alloc;

#define default_row_len 16 

Config config_init() {
  Config c = {NULL, 0, addr_list_init(), addr_list_init(), "\x1b[7m", "\x1b[0m",
              FALSE, default_row_len, dump_byte, ""," ", malloc(default_row_len), OK};
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
