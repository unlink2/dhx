#include "config.h"
#include <stdlib.h>

Config config;
SclAlloc alloc;

Config config_init() {
  Config c = {OK};
  alloc = scl_alloc_init(malloc, free);
  return c;
}

void config_free(Config *c) {}
