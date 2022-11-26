#ifndef DUMP_H_
#define DUMP_H_

#include "ctx.h"
#include <stdio.h>
#include <scl/types.h>

typedef struct Config Config;

typedef usize (*DumpMode)(Config *, Context *, FILE *, FILE *);
typedef void (*DumpHook)(Config *, Context *, FILE *, FILE *);

typedef struct DumpHooks {
  DumpHook on_line_start;
  DumpHook on_line_end;
  DumpHook on_row_start;
  DumpHook on_row_end;
} DumpHooks;

DumpHooks dump_hooks_init(DumpHook ls, DumpHook le, DumpHook rs, DumpHook re);
void dump_hook_nop(Config *c, Context *ctx, FILE *in, FILE *out);

FILE *dump_open_input(char *path);

FILE *dump_open_output(char *path);

void dump_close(FILE *f);

usize dump_byte(Config *c, Context *ctx, FILE *in, FILE *out);
usize dump_char(Config *c, Context *ctx, FILE *in, FILE *out);
usize dump_char_raw(Config *c, Context *ctx, FILE *in, FILE *out);

void dump(Config *c, FILE *in, FILE *out, DumpMode f);

#endif
