#include <curses.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "colors.h"

#include "render.h"
#include "conchview-render.h"
#include "listview-render.h"
#include "detailview-render.h"

#define CHROME_BORDER_HEIGHT 1
#define CHROME_PADDING_X 1

// Placeholder for status message
#define STATUS_MAXLEN 64
static char status[STATUS_MAXLEN];

// 64 characters provides space for a 14-character status with the clock
#define MIN_WIDTH_FOR_CLOCK 64

// Should we display the spinner?
static bool show_spinner = false;

static void render_clock(WINDOW *window, char *clock_text) {
  int max_x = getmaxx(window);
  mvwaddstr(window, 0, max_x - strlen(clock_text) - CHROME_PADDING_X,
            clock_text);
}

static void generate_clock_text(WINDOW *window, int time_str_limit,
                                char *time_str) {
  time_t now = time(NULL);
  struct tm *now_tm = localtime(&now);
  strftime(time_str, time_str_limit, " %Y-%m-%d %H:%M:%S ", now_tm);
}

static void render_help(WINDOW *window, char *help_text) {
  int last_line = getmaxy(window) - 1;
  mvwaddstr(window, last_line, CHROME_PADDING_X, help_text);
}

static void render_watermark(WINDOW *window, bool spin) {
  int max_x = getmaxx(window);
  int max_y = getmaxy(window) - 1;
  static unsigned int spinner_state;
  static char const *spinner[] = {
    " /dev/fort 11 ", " -dev-fort 11 ", " \\dev\\fort 11 ", " |dev|fort 11 ",
  };

  // Spin, either if we've been explicitly instructed to spin, or if we have not
  // completed a whole revolution of the spinner.
  if (spin || spinner_state != 0) {
    spinner_state =
        (spinner_state + 1) % (sizeof(spinner) / sizeof(char const *));
  }

  mvwaddstr(window, max_y,
            max_x - strlen(spinner[spinner_state]) - CHROME_PADDING_X,
            spinner[spinner_state]);
}

static void render_chrome(WINDOW *window, char *title_text) {
  int max_x = getmaxx(window);
  int last_line = getmaxy(window) - 1;

  mvwhline(window, 0, 0, ACS_HLINE, max_x);
  mvwhline(window, last_line, 0, ACS_HLINE, max_x);

  mvwaddstr(window, 0, CHROME_PADDING_X, title_text);
}

void conch_status_clear() { status[0] = '\0'; }

void conch_status_set(const char *msg) { strncpy(status, msg, STATUS_MAXLEN); }

void conch_spinner_hide() { show_spinner = false; }
void conch_spinner_show() { show_spinner = true; }

static void render_status_message(WINDOW *window) {
  size_t len = strlen(status);
  if (len == 0) {
    return;
  }
  int center_offset = (getmaxx(window) - len + 2 /* whitespace padding */) / 2;
  mvwprintw(window, 0, center_offset, " %s ", status);
}

void render_view(WINDOW *window, view_type current_view, void *view_state) {
  int max_x = getmaxx(window);
  int max_y = getmaxy(window);

  werase(window);

  if (max_y < 2 * CHROME_BORDER_HEIGHT) {
    mvwaddstr(window, 0, 0, "Window too small! Embiggen!");
    wrefresh(window);
    return;
  }

  // The two -1s here are because ncurses co-ordinates are *inclusive*
  winrect rect = {
    .top = CHROME_BORDER_HEIGHT,
    .left = 0,
    .bottom = max_y - CHROME_BORDER_HEIGHT - 1,
    .right = max_x - 1,
    .width = max_x,
    .height = max_y - 2 * CHROME_BORDER_HEIGHT,
  };

  conch_spinner_hide();
  conch_status_clear();

  switch (current_view) {
  case VIEW_CONCH:
    conch_conchview_render((conchview *)view_state, window, &rect);
    break;
  case VIEW_LIST:
    conch_listview_render((listview *)view_state, window, &rect);
    break;
  case VIEW_DETAIL:
    conch_detailview_render((detailview *)view_state, window, &rect);
    break;
  }

  render_chrome(window, " conch 螺 ");

  if (MIN_WIDTH_FOR_CLOCK <= max_x) {
    char clock_text[1024];
    generate_clock_text(window, sizeof(clock_text), clock_text);
    render_clock(window, clock_text);
  }

  render_status_message(window);

  render_help(window, " j/k: down/up  s: stick to top  0: to top  TAB: next "
                      "unread  /: search  q: quit ");

  render_watermark(window, show_spinner);

  wrefresh(window);
}
