#ifndef ERROR_H_
#define ERROR_H_

#include <scl/error.h>

typedef enum Error {
  OK,
  ERR_BAD_FORMAT = SCL_ERR_LEN,
  ERR_FILE_NOT_FOUND
} Error;

char *error_to_string(Error e);

#endif
