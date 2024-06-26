#include <ncurses.h>

typedef struct _note_files {
  struct dirent **files;
  int n;
} note_files;

typedef struct _note_menu {
  WINDOW *win;
  int highlight;
  int choice;
} note_menu;

typedef struct _note_editor {
  WINDOW *win;
} note_editor;

#define MENU_W 3

enum Window {
  MENU,
  EDITOR
};

/*
  Load the file names for note files stored in the notes directory.
*/
note_files *load_notes();

WINDOW *active_window(enum Window win);

void move_cursor(enum Window win_e, int y, int x);

/*
  Draw the menu to the screen.
*/
void print_menu(WINDOW *win, note_files *nf, int highlight);

/*
  Draw the editor to the screen.
*/
void print_editor();