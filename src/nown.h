#include <ncurses.h>

typedef struct _note_vault {
  struct dirent **files;
  int n;
} note_vault;

typedef struct _note_menu {
  WINDOW *win;
  int highlight;
  int choice;
} note_menu;

typedef struct _note_editor {
  WINDOW *win;
} note_editor;

typedef struct _app_state {
  note_vault *vault;
  note_menu *menu;
  note_editor *editor;
  WINDOW *active_win;
} app_state;

#define MENU_W 3

void init_app();
void init_main_screen();
void init_menu();
void init_editor();
void load_vault();
void print_menu();