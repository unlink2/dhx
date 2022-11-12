#ifndef DUMP_H_
#define DUMP_H_

#include <stdio.h>
#include <scl/types.h>

typedef struct Config Config;

typedef void (*DumpMode)(Config *, FILE *, FILE *, usize, u8);

FILE *open_input(char *path);

FILE *open_output(char *path);

void close(FILE *f);

void dump_byte(Config *c, FILE *in, FILE *out, usize address, u8 b);
void dump_char(Config *c, FILE *in, FILE *out, usize address, u8 b);

void dump(Config *c, FILE *in, FILE *out, DumpMode f);

#endif
