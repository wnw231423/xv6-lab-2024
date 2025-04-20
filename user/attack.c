#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "kernel/riscv.h"

int
main(int argc, char *argv[])
{
  // your code here.  you should write the secret to fd 2 using write
  // (e.g., write(2, secret, 8)
  while (1) {
    char *end = sbrk(PGSIZE);
    *(end+31) = '\0';
    if (strcmp(end, "my very very very secret pw is ") == 0) {
      write(2, end+32, 8);
      break;
    }
  }
  exit(1);
}
