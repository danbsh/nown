#include <dirent.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nown.h"

// TODO: vault path should be configurable

int main(void) {
  init_main_screen();
  vault files = load_vault();
  print_menu(files);

  while (getch() != KEY_F(1)) {
  }

  endwin();
  free(files.note_files);
  return 0;
}

void init_main_screen() {
  initscr();
  raw();
  noecho();
  keypad(stdscr, TRUE);
}

vault load_vault() {
  struct dirent **namelist;
  int n;

  if ((n = scandir("./vault", &namelist, NULL, alphasort)) == -1) {
    perror("Error scanning vault.");
    exit(EXIT_FAILURE);
  }

  vault files = {namelist, n};

  return files;
}

void print_menu(vault files) {
  for (int i = 0; i < files.n; i++) {
    char *name = files.note_files[i]->d_name;

    if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
      continue;

    printw("%s\n", files.note_files[i]->d_name);
  }
}