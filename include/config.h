#ifndef CONFIG_H_
#define CONFIG_H_

#include <scl/sclalloc.h>
#include "addrlst.h"
#include "ctx.h"
#include "error.h"
#include "dump.h"

#define OUT_FMT_8H "%02x"
#define OUT_FMT_16H "%04x"
#define OUT_FMT_32H "%08x"
#define OUT_FMT_64H "%016llx"
#define OUT_FMT_C "%c"
#define OUT_FMT_R "%c"

#define OUT_FMT_1D "%03d"
#define OUT_FMT_2D "%05d"
#define OUT_FMT_4D "%010ld"
#define OUT_FMT_8D "%020lld"

#define OUT_FMT_1B ""

typedef enum Endianess { END_LITTLE, END_BIG, END_NATIVE } Endianess;

Endianess end_map(const char *inp);

typedef enum OutputGroup {
  OG_1H,
  OG_1D,
  OG_1B,

  OG_2H,
  OG_2D,
  // OG_2B,

  OG_4H,
  OG_4D,
  // OG_4B,

  OG_8H,
  OG_8D,
  // OG_8B,

  OG_CHAR,
  OG_RAW
} OutputGroup;

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

  char *out_fmt;

  Endianess endianess;
  OutputGroup output_grp;

  usize start_addr;
  usize end_addr;

  DumpHooks hooks;
  
  Error err;
} Config;

extern Config config;
extern SclAlloc alloc;

Config config_init();

void config_set_rowlen(Config *c, usize len);

void config_apply_mode(Config *c, OutputGroup g);

void config_free(Config *c);

#endif
