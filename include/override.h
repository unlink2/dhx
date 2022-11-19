#ifndef OVERRIDE_H_
#define OVERRIDE_H_ 

#include <scl/types.h>
#include "error.h"

// An override can change values at a 
// certain address 
typedef struct Override {
  usize address;
  usize len;
  
  const u8 *data;
} Override;

Override override_init(usize address, const u8 *data, usize len);

// get an override value or the default
u8 override_get(Override *o, usize address, u8 or);

#define OVERRIDE_MAX 512 

typedef struct OverrideList {
  Override list[OVERRIDE_MAX];
  
  usize len;
} OverrideList;


OverrideList ol_init();

void ol_add(OverrideList *ol, Override o);
Override ol_get(OverrideList *ol, usize index, Error *e);

void ol_free(OverrideList *ol);

#endif 
