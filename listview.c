#include <stdlib.h>

#include "listview.h"

screen_state_s *conch_listview_new(blastlist *bl, int stick_to_top) {
  screen_state_s *lv = malloc(sizeof(screen_state_s));
  lv->head = bl;
  lv->current_blast = bl;
  lv->blast_offset = 0;
  lv->stick_to_top = stick_to_top;
  return lv;
}

void conch_listview_free(screen_state_s *lv) { free(lv); }
