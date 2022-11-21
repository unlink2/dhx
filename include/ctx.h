#ifndef CTX_H_
#define CTX_H_ 

#include <scl/types.h>

typedef enum ContextOutputState {
  CTX_OUTPUT_NORMAL,
  CTX_OUTPUT_HILIGHT 
} ContextOutputState;

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

  ContextOutputState out_state;
  
  // a keeps track of the total 
  // bytes written in the written counter 
  // that way it can be interrupted by a frame 
  // break
  usize written;        
  usize address;
  usize rowlen;
} Context;

Context ctx_init();


void ctx_line_wr(Context *ctx, usize n);

void ctx_row_wr(Context *ctx, usize n);

Context ctx_init_buffer(usize len);
void ctx_free(Context *ctx);

#endif 
