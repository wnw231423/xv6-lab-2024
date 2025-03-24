#include "kernel/types.h"
#include "user/user.h"

void prime(int) __attribute__((noreturn));

void prime(int fd) {
  // read the first number from previous pipe.
  int base;
  int n = read(fd, &base, 4);
  if (n == 0) {
    // read nothing.
    close(fd);
    exit(0);
  }
  // able to read the base number. print it.
  printf("prime %d\n", base);

  int p[2];
  pipe(p);
  if (fork() == 0) {
    // child here.
    close(fd);   // don't forget release this.
    close(p[1]); // doesn't need
    prime(p[0]);
  } else {
    // parent here.
    close(p[0]); // doesn't need
    int bigger;
    while (1) {
      // read other numbers from previous pipe.
      int nn = read(fd, &bigger, 4);
      if (nn == 0) {
        // nothing to read from parent, wait child.
        close(p[1]);
        close(fd);
        wait(0);
        exit(0);
      } else {
        // contiously read from parent, send it to child if satisfy condition.
        if (bigger % base != 0) {
          write(p[1], &bigger, 4);
        }
      }
    }
  }
}

int main() {
  int p[2];
  pipe(p);
  if (fork() == 0) {
    // child here.
    close(p[1]); // child doesn't need it.
    prime(p[0]);
  } else {
    // parent here.
    close(p[0]); // parent doesn't need it.
    for (int i = 2; i <= 280; i++) {
      write(p[1], &i, 4);
    }
    close(p[1]);
    wait(0);
    exit(0);
  }
}
