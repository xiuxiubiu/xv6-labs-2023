#include "../kernel/types.h"
#include "user.h"

int
main() 
{

  int p[2], pid;

  if (pipe(p) < 0) {
    fprintf(2, "pipe error\n");
    exit(1);
  }

  pid = fork(); 
  if (pid < 0) {
    fprintf(2, "fork failed\n");
    exit(1);
  }

  if (pid == 0) {

    // children
    if (read(p[0], 0, 1) < 0) {
      fprintf(2, "children read error\n");
      exit(1);
    }
  
    printf("%d: received ping\n", getpid());
  
    if (write(p[1], "0", 1) < 1) {
      fprintf(2, "children write error\n");
      exit(1);
    }

    exit(0);
  }

  // parent 
  if (write(p[1], "0", 1) < 1) {
    fprintf(2, "parent write error\n");
    exit(1);
  }

  if (read(p[0], 0, 1) < 0) {
    fprintf(2, "parent read error\n");
    exit(1);
  }

  printf("%d: received pong\n", getpid());
  
  wait(0);
  exit(0);
}
