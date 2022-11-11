/**
 * When built without test
 */
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

static struct argp_option options[] = {
    {"address", 'a', "ADDRESS", 0, "Address to filter for"},
    {"base", 'b', "BASE", 0, "Set base address of dump"},
    {"highlight", 'c', "HIGHLIGHT", 0, "Set highlight escape code"},
    {"rowlen", 'r', "ROWLEN", 0, "How many bytes to display in each row"},
    {0}};

static error_t parse_opt(int key, char *arg,
                         struct argp_state *state) { // NOLINT
  Config *cfg = state->input;

  switch (key) {
  case 'a':
    break;
  case 'b':
    break;
  case 'c':
    break;
  case ARGP_KEY_ARG:
    if (state->arg_num > 0) {
      // Too many arguments
      argp_usage(state); // NOLINT
    } else {
      // parse file input or stdin
      // cfg->err |= bk_find(cfg->input, arg, &cfg->rl);
    }
    break;
  case ARGP_KEY_END:
    if (state->arg_num < 1) {
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
