#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

void xargs(int argc, char* argv[]) {
  char buf[512];
  char *p = buf;
  while (read(0, p, 1) == 1) {
    if (*p == '\n') {
      *p = '\0';
      char* new_argv[argc + 1];
      for (int i = 0; i < argc; i++) {
        new_argv[i] = argv[i];
      }
      new_argv[argc] = buf;
      if (fork() == 0) {
        exec(argv[0], new_argv);
      }
      wait(0);
      p = buf;
    } else {
      p++;
    }
  }
}

int main(int argc, char* argv[]) {
  xargs(argc - 1, argv + 1);
  exit(0);
}
