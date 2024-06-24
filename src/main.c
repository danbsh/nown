#include <dirent.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nown.h"

static app_state *state;

int main(void) {
  state = (app_state *)malloc(sizeof(app_state));
  init_app();

  while (getch() != KEY_F(1)) {
  }

  free(state->vault);
  free(state->menu);
  free(state);
  endwin();

  return 0;
}

void init_app() {
  init_main_screen();
  load_vault();
  init_menu();
  init_editor();
}

void init_main_screen() {
  initscr();
  raw();
  noecho();
  keypad(stdscr, TRUE);
  refresh();
}

void init_menu(note_vault files) {
  int menu_len = COLS / MENU_W;

  state->menu = (note_menu *)malloc(sizeof(note_menu));
  state->menu->win = newwin(LINES, menu_len, 0, 0);
  state->menu->highlight = 2;
  state->menu->choice = 2;

  print_menu();
}

void init_editor() {
  int menu_len = COLS / MENU_W;
  int editor_len = COLS - menu_len;

  state->editor = (note_editor *)malloc(sizeof(note_editor));
  state->editor->win = newwin(LINES, editor_len, 0, menu_len);
  box(state->editor->win, 0, 0);
  wrefresh(state->editor->win);
}

void load_vault() {
  struct dirent **namelist;
  int n;

  state->vault = (note_vault *)malloc(sizeof(note_vault));

  if ((n = scandir("./vault", &namelist, NULL, alphasort)) == -1) {
    perror("Error scanning vault.");
    exit(EXIT_FAILURE);
  }

  state->vault->files = namelist;
  state->vault->n = n;
}

void print_menu() {
  for (int i = 0; i < state->vault->n; i++) {
    char *name = state->vault->files[i]->d_name;

    if (i == 1) {
      wattron(state->menu->win, A_BOLD);
      mvwprintw(state->menu->win, i, 1, "Notes");
      wattroff(state->menu->win, A_BOLD);
    }

    if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
      continue;

    mvwprintw(state->menu->win, i, 1, "%s\n", state->vault->files[i]->d_name);
  }

  box(state->menu->win, 0, 0);
  wmove(state->menu->win, state->menu->highlight, 1);
  wrefresh(state->menu->win);
}