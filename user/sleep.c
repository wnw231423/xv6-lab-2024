#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(2, "usage: sleep [sleep_time]\n");
    exit(1);
  }

  int t = atoi(argv[1]);
  int res = sleep(t);
  exit(res);
}
