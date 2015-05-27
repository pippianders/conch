#include "listview-keys.h"

#include <curses.h>

extern void fatal_error(const char *, ...);

keypress_result conch_listview_keypress_dispatch(int key, listview *lv) {

  switch (key) {
  case '0':
    conch_listview_jump_to_top(lv);
    break;

  case '.':
  case '\t':
    conch_listview_jump_to_next_unread(lv);
    break;

  case 'j':
    conch_listview_select_next_blast(lv);
    break;

  case 'k':
    conch_listview_select_prev_blast(lv);
    break;

  case 's':
    conch_listview_toggle_stick_to_top(lv);
    break;
  }
  return CONCH_NOP;
}
