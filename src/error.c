#include "error.h"
#include <scl/types.h>

char *error_to_string(Error e) {
  switch (e) {
  case OK:
    return NULL;
  case ERR_BAD_FORMAT:
    return "Bad format";
  case ERR_FILE_NOT_FOUND:
    return "File not found";
  default:
    return scl_error_to_str((SclError)e);
  }
}
