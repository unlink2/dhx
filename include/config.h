#ifndef CONFIG_H_
#define CONFIG_H_

#include <scl/sclalloc.h>
#include "error.h"

typedef struct Config {
  Error err;
} Config;

extern Config config;
extern SclAlloc alloc;

Config config_init();

void config_free(Config *c);

#endif
