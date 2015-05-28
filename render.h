#ifndef _RENDER_H
#define _RENDER_H

#include <curses.h>

typedef enum {
  VIEW_CONCH,
  VIEW_LIST,
  VIEW_DETAIL,
} view_type;

typedef struct {
  unsigned int top, left, bottom, right, width, height;
} winrect;

typedef struct window_chrome_s {
  int border_width;
  int padding_x;
} window_chrome_s;
extern window_chrome_s chrome;

void render_view(WINDOW *window, view_type current_view, void *view_state);
void conch_status_clear();
void conch_status_set(const char *status);
void conch_spinner_hide();
void conch_spinner_show();

#endif
