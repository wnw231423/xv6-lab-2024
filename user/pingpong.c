#include "kernel/types.h"
#include "user/user.h"

int main() {
  int p1[2]; // parent write to child
  int p2[2]; // child write to parent
  pipe(p1);
  pipe(p2);

  if (fork() == 0) {
    int pid = getpid();
    char byte;
    int n = read(p1[0], &byte, 1);
    if (n != 1) {
      exit(1);
    }
    printf("%d: received ping\n", pid);
    write(p2[1], &byte, 1);
    close(p2[1]);
    exit(0);
  } else {
    int pid = getpid();
    char byte = 'p';
    write(p1[1], &byte, 1);
    close(p1[1]);
    int n = read(p2[0], &byte, 1);
    if (n == 1) {
      printf("%d: received pong\n", pid);
      exit(0);
    }
    exit(1);
  }
}
