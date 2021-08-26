#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include <common.h>

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;
  uint32_t info;
  char exp[300];

  /* TODO: Add more members if necessary */

} WP;
void init_wp_pool();
WP *  new_wp();
void free_wp(int, WP **);

WP *wp;
uint32_t wp_size;
#endif
