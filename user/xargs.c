#include "../kernel/types.h"
#include "../kernel/param.h"
#include "user.h"


int main(int argc, char **argv) {

  char c, l[512], *tl, **tv, *v[MAXARG];
  int i;

  tv = v;
  for (i = 1; i < argc; i++)
    *tv++ = argv[i];
  *tv = tl = l;

  while (read(0, &c, 1) > 0 && c)
    if (c == '\n') {
      *tl = '\0';

      if (fork() == 0) {
        exec(argv[1], v);
        exit(0);
      }

      wait(0); 
      tl = l;

    } else 
      *tl++ = c;
    
  return 0;
}
