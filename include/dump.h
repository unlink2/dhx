#ifndef DUMP_H_
#define DUMP_H_

#include <stdio.h>
#include <scl/types.h>

typedef struct Config Config;

typedef usize (*DumpMode)(Config *, FILE *, FILE *, usize, const u8 *, usize);

FILE *open_input(char *path);

FILE *open_output(char *path);

void close(FILE *f);

usize dump_byte(Config *c, FILE *in, FILE *out, usize address, const u8 *b,
                usize);
usize dump_char(Config *c, FILE *in, FILE *out, usize address, const u8 *b,
                usize);
usize dump_char_raw(Config *c, FILE *in, FILE *out, usize address, const u8 *b,
                    usize);

void dump(Config *c, FILE *in, FILE *out, DumpMode f);

#endif
