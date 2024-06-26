#include <dirent.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nown.h"

WINDOW *menu_win;
WINDOW *editor_win;

int main(void) {
  initscr();
  raw();
  noecho();
  keypad(stdscr, TRUE);
  refresh();

  int menu_w = COLS / MENU_W;
  int editor_w = COLS - menu_w;
  enum Window active = MENU;

  int curs_x = 1;
  int curs_y = 2;

  note_files *files_d = load_notes();

  menu_win = newwin(LINES, menu_w, 0, 0);
  keypad(menu_win, TRUE);
  print_menu(menu_win, files_d, curs_y);

  editor_win = newwin(LINES, editor_w, 0, menu_w);
  keypad(editor_win, TRUE);

  wmove(active_window(active), curs_y, curs_x);

  int key;
  while (key = wgetch(active_window(active)), key != KEY_F(1)) {
    switch (key) {
    case KEY_DOWN:
      move_cursor(active, ++curs_y, curs_x);
      break;
    case KEY_UP:
      move_cursor(active, --curs_y, curs_x);
      break;
    }

    print_menu(menu_win, files_d, curs_y);
    wrefresh(active_window(active));
  }

  free(files_d);
  endwin();

  return 0;
}

WINDOW *active_window(enum Window win) {
  switch (win) {
  case MENU:
    return menu_win;
  case EDITOR:
    return editor_win;
  }
}

void move_cursor(enum Window win_e, int y, int x) {
  switch (win_e) {
  case MENU:
    if (y < 2)
      y = 2;
    break;
  }

  WINDOW *win = active_window(win_e);
  wmove(win, y, x);
}

note_files *load_notes() {
  struct dirent **namelist;
  int n;

  if ((n = scandir("./vault", &namelist, NULL, alphasort)) == -1) {
    perror("Error scanning vault.");
    exit(EXIT_FAILURE);
  }

  note_files *nf = (note_files *)malloc(sizeof(note_files));
  nf->files = namelist;
  nf->n = n;

  return nf;
}

void print_menu(WINDOW *win, note_files *nf, int highlight) {
  for (int i = 0; i < nf->n; i++) {
    char *name = nf->files[i]->d_name;

    if (i == 1) {
      wattron(win, A_BOLD);
      mvwprintw(win, i, 1, "Notes");
      wattroff(win, A_BOLD);
    }

    if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
      continue;

    if (i == highlight)
      wattron(win, A_REVERSE);

    mvwprintw(win, i, 1, "%s\n", nf->files[i]->d_name);

    if (i == highlight)
      wattroff(win, A_REVERSE);
  }

  box(win, 0, 0);
  wrefresh(win);
}