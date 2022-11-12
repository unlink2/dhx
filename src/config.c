#include "config.h"
#include <stdlib.h>

Config config;
SclAlloc alloc;

Config config_init() {
  Config c = {NULL, 0, addr_list_init(), addr_list_init(), "\x1b[7m", "\x1b[0m",
              FALSE, 16, dump_byte, " ", OK};
  alloc = scl_alloc_init(malloc, free);
  return c;
}

void config_free(Config *c) {
  addr_list_free(&c->addrs);
  addr_list_free(&c->addrs);
}
