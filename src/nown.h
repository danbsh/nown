typedef struct _note_vault {
  struct dirent **note_files;
  int n;
} vault;

void init_main_screen();
vault load_vault();
void print_menu(vault files);