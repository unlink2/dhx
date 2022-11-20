#ifndef CTX_H_
#define CTX_H_ 

#include <scl/types.h>

// The context is a stateful 
// object for keeping track of 
// runtime state 
typedef struct Context {
  usize current_row_len;
  usize current_col_len;
  u8 *buffer;
  u8 *buffer_start;
  
  usize len;
  usize max_len;

  // track how many characters we have written 
  // to allow drawing boxes 
  usize line_wr;
  usize row_wr; 
      
  usize address;
  usize rowlen;
} Context;

Context ctx_init();


void ctx_line_wr(Context *ctx, usize n);

void ctx_row_wr(Context *ctx, usize n);

Context ctx_init_buffer(usize len);
void ctx_free(Context *ctx);

#endif 
