/**
 * When built without test
 */
#include "scl/sclstr.h"
#include <scl/scllog.h>
#include "dump.h"

#ifndef TEST

/// only use main if binary
#if TYPE == bin

#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <argp.h>

const char *argp_program_version = "stvi 0.1";
const char *argp_program_bug_address = "<lukas@krickl.dev>";
static char doc[] = "stvi";
static char args_doc[] = "Stack Dump Visualizer";

typedef enum LongOptions {
  NO_ADDRESS = 128,
  SEPARATOR,
  PREFIX,
  START,
  END,
  ADDR_LEN,
} LongOptions;

static struct argp_option options[] = {
    {"address", 'a', "ADDRESS", 0, "Address to filter for"},
    {"frame", 's', "ADDRESS", 0, "Frame separator address"},
    {"base", 'b', "BASE", 0, "Set base address of dump"},
    {"highlight", 'c', "HIGHLIGHT", 0, "Set highlight escape code"},
    {"no-addr", NO_ADDRESS, NULL, 0, "Do not display addresses"},
    {"separator", SEPARATOR, "SEPARATOR", 0, "Byte separator"},
    {"prefix", PREFIX, "PREFIX", 0, "Byte prefix"},
    {"unhighlight", 'u', "UNHIGHLIGHT", 0, "Set unhighlight escape code"},
    {"rowlen", 'r', "ROWLEN", 0,
     "How many bytes to display in each row (little, big)"},
    {"endianess", 'e', "ENDIANESS", 0, "Set file endianess"},
    {"loglevel", 'l', "LEVEL", 0, "Log level"},
    {"group", 'g', "GROUP", 0,
     "How to group and format output bytes (1h, 1d, 1b, 2h, 2d, 4h, 4d, "
     "8h, 8d, c, r)"},
    {"start", START, "START", 0, "Start address"},
    {"end", END, "END", 0, "End address"},
    {"addr-len", ADDR_LEN, "LEN", 0, "Address length"},
    {0}};

static error_t parse_opt(int key, char *arg,
                         struct argp_state *state) { // NOLINT
  Config *cfg = state->input;
  switch (key) {
  case 'a': {
    SclError err = SCL_OK;
    usize addr = str_to_i64(str_init(arg, scl_strlen(arg)), 16, &err);

    addr_list_add(&cfg->addrs, addr);

    cfg->err = (Error)err;
  } break;
  case 's':
    addr_list_add(&cfg->frames, str_to_i64(str_init(arg, scl_strlen(arg)), 16,
                                           (SclError *)&cfg->err));
    break;
  case 'b': {
    SclError err = SCL_OK;
    usize addr = str_to_i64(str_init(arg, scl_strlen(arg)), 16, &err);

    cfg->base_addr = addr;

    cfg->err = (Error)err;
  } break;
  case 'u':
    cfg->unhighlight = arg;
    break;
  case 'c':
    cfg->highlight = arg;
    break;
  case 'r':
    config_set_rowlen(cfg, str_to_i64(str_init(arg, scl_strlen(arg)), 10,
                                      (SclError *)&cfg->err));
    break;
  case NO_ADDRESS:
    cfg->no_addr = TRUE;
    break;
  case SEPARATOR:
    cfg->separator = arg;
    break;
  case PREFIX:
    cfg->prefix = arg;
    break;
  case START:
    cfg->start_addr =
        str_to_i64(str_init(arg, scl_strlen(arg)), 16, (SclError *)&cfg->err);
    break;
  case END:
    cfg->end_addr =
        str_to_i64(str_init(arg, scl_strlen(arg)), 16, (SclError *)&cfg->err);
    break;
  case ADDR_LEN:
    cfg->addr_len =
        str_to_i64(str_init(arg, scl_strlen(arg)), 10, (SclError *)&cfg->err);
    break;
  case 'l':
    scl_log_set_level(
        str_to_i64(str_init(arg, scl_strlen(arg)), 10, (SclError *)&cfg->err));
    break;
  case 'e':
    cfg->endianess = end_map(arg);
    break;
  case 'g':
    config_apply_mode(cfg, og_map(arg));
    break;
  case ARGP_KEY_ARG:
    if (state->arg_num > 0) {
      // Too many arguments
      argp_usage(state); // NOLINT
    } else {
      cfg->in_path = arg;
    }
    break;
  case ARGP_KEY_END:
    if (state->arg_num < 0) {
      /* Not enough arguments. */
      argp_usage(state); // NOLINT
    }
    break;
  default:
    return ARGP_ERR_UNKNOWN;
  }
  return 0;
}

static struct argp argp = {options, parse_opt, args_doc, doc};

int main(int argc, char **argv) {
  config = config_init();
  argp_parse(&argp, argc, argv, 0, 0, &config); // NOLINT

  FILE *in = open_input(config.in_path);
  FILE *out = open_output(NULL);

  if (!in || !out) {
    config.err = ERR_FILE_NOT_FOUND;
  }

  if (config.err) {
    char *err_str = error_to_string(config.err);
    scl_log_output("%s\n", err_str);
  } else {
    dump(&config, in, out, config.mode);
  }

  close(in);
  close(out);

  config_free(&config);

  return config.err;
}

#endif
#endif

/**
 * When built with test
 */
#ifdef TEST

#include <scl/macros.h>

int main(int argc, char **argv) {
  const struct CMUnitTest tests[] = {NULL};
  return cmocka_run_group_tests(tests, NULL, NULL);
}

#endif
