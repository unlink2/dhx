#ifndef CONFIG_H_
#define CONFIG_H_

#include <scl/sclalloc.h>
#include "addrlst.h"
#include "error.h"
#include "dump.h"


typedef struct Config {
  char *in_path;

  usize base_addr;

  AddrList addrs;
  AddrList frames;

  char *highlight;
  char *unhighlight;
  
  bool no_addr;

  usize rowlen;

  DumpMode mode;

  char *separator;

  Error err;
} Config;

extern Config config;
extern SclAlloc alloc;

Config config_init();

void config_free(Config *c);

#endif
