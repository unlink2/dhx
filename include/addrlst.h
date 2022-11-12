#ifndef ADDRLST_H_
#define ADDRLST_H_

#include "scl/types.h"

#define ADDR_MAX 256

// TODO make dynamically expanding in the future
typedef struct AddrList {
  usize addrs[ADDR_MAX];
  usize len;
} AddrList;

AddrList addr_list_init();

void addr_list_add(AddrList *l, usize addr);
usize addr_list_get(AddrList *l, usize idx);

bool addr_list_contains(AddrList *l, usize addr);

void addr_list_free(AddrList *l);

#endif
