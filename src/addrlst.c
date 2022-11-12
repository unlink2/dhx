#include "addrlst.h"
#include "scl/scllog.h"
#include <scl/sclmem.h>

AddrList addr_list_init() {
  AddrList l;
  scl_memset(&l, 0, sizeof(l));

  return l;
}

void addr_list_add(AddrList *l, usize addr) {
  if (l->len >= ADDR_MAX) {
    scl_log_error("Address list capacity reached!\n");
    return;
  }

  l->addrs[l->len] = addr;
  l->len++;
}

usize addr_list_get(AddrList *l, usize idx) {
  if (idx < l->len) {
    return l->addrs[idx];
  }
  scl_log_error("Out of bounds address read");
  return 0;
}

bool addr_list_contains(AddrList *l, usize addr) {
  for (usize i = 0; i < l->len; i++) {
    if (l->addrs[i] == addr) {
      return TRUE;
    }
  }

  return FALSE;
}

void addr_list_free(AddrList *l) {}
