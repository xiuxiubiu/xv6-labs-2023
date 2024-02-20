#include "../kernel/types.h"
#include "user.h"

int prime_process(int *);

int main() {
  int i, p[2], pid;

  pipe(p);

  // children
  pid = fork();
  if (pid == 0) {
    for (i = 2; i <= 35; i++) 
      write(p[1], &i, sizeof(i));
    exit(0);
  }

  prime_process(p);
  close(p[0]);
  close(p[1]);
  wait(0);
  exit(0);
}

int prime_process(int *p) {
  int prime, n, new_pipe[2], has_process;

  has_process = 0;
  prime = -1;
  pipe(new_pipe);

  close(p[1]);
  while (read(p[0], &n, sizeof(n)) > 0) {
    if (prime == -1) {
      prime = n;
      printf("prime %d\n", prime);
      continue;
    }

    if (n % prime == 0)  
      continue; 
    
    if (!has_process) {
      has_process = 1;
      if (fork() == 0) {
        prime_process(new_pipe);
        return 0;
      }
    }
    
    write(new_pipe[1], &n, sizeof(n));
  }

  close(p[0]);
  close(new_pipe[0]);
  close(new_pipe[1]);
  wait(0);
  return 0;
}

