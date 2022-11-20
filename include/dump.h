#ifndef DUMP_H_
#define DUMP_H_

#include "ctx.h"
#include <stdio.h>
#include <scl/types.h>

typedef struct Config Config;

typedef usize (*DumpMode)(Config *, Context *, FILE *, FILE *);

FILE *open_input(char *path);

FILE *open_output(char *path);

void close(FILE *f);

usize dump_byte(Config *c, Context *ctx, FILE *in, FILE *out);
usize dump_char(Config *c, Context *ctx, FILE *in, FILE *out);
usize dump_char_raw(Config *c, Context *ctx, FILE *in, FILE *out);

void dump(Config *c, FILE *in, FILE *out, DumpMode f);

#endif
