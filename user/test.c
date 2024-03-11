#include "../kernel/types.h"
#include "user.h"
#include "../kernel/fcntl.h"
#include "../kernel/stat.h"

void fork_snippet() {
  int pid = fork();

  if (pid > 0) {
    printf("parent: child=%d\n", pid);
    pid = wait((int *) 0);
    printf("child %d is done\n", pid);

  } else if (pid == 0) {
    sleep(10);
    printf("child: exiting\n"); 
    exit(0);

  } else {
    printf("fork error\n");
    exit(1);
  }

  exit(0);
}

void exec_snippet() {
  char *argv[3];
  argv[0] = "echo";
  argv[1] = "hello";
  argv[2] = 0;

  exec("echo", argv);
  printf("exec error\n");

  exit(0);
}

void link_snippet() {

  int afd = open("a", O_CREATE | O_WRONLY);
  link("a", "b");
  int bfd = open("a", O_CREATE | O_WRONLY);

  unlink("b");
  unlink("a");


  struct stat as, bs;

  fstat(afd, &as);
  fstat(bfd, &bs);

  printf("a: %d, b: %d\n", as.ino, bs.ino);

  exit(0);
}

int main() {
  // fork_snippet();
  
  // exec_snippet();
  
  link_snippet();

  return 0;
}
