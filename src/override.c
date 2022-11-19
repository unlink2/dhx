#include "override.h"
#include "scl/scllog.h"
#include <scl/sclmem.h>

Override override_init(usize address, const u8 *data, usize len) {
  Override o = {address, len, data};

  return o;
}

u8 override_get(Override *o, usize address, u8 or) {
  if (o->address >= address && o->address + o->len < address) {
    return o->data[o->address - address];
  }
  return or ;
}

OverrideList ol_init() {
  OverrideList ol;
  scl_memset(&ol, 0, sizeof(ol));
  return ol;
}

void ol_add(OverrideList *ol, Override o) {
  if (ol->len >= OVERRIDE_MAX) {
    scl_log_error("Override list size exceeded!\n");
    return;
  }

  ol->list[ol->len] = o;
  ol->len++;
}

Override ol_get(OverrideList *ol, usize index, Error *e) {
  if (index >= ol->len) {
    scl_log_error("Override list read out of bounds\n");
    *e = ERR_OUT_OF_BOUNDS;
    return (Override){0};
  }

  return ol->list[index];
}

void ol_free(OverrideList *ol) {}
