#ifndef CONFIG_H_
#define CONFIG_H_

#include <scl/sclalloc.h>
#include "addrlst.h"
#include "error.h"
#include "dump.h"

typedef enum Endianess { END_LITTLE, END_BIG, END_NATIVE } Endianess;

Endianess end_map(const char *inp);

typedef enum OutputGroup { OG_1, OG_2, OG_4, OG_8 } OutputGroup;

OutputGroup og_map(const char *inp);

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

  char *prefix;
  char *separator;

  u8 *buffer;

  Endianess endianess;
  OutputGroup output_grp;

  Error err;
} Config;

extern Config config;
extern SclAlloc alloc;

Config config_init();

void config_set_rowlen(Config *c, usize len);

void config_free(Config *c);

#endif
