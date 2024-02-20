#include "../kernel/types.h"
#include "user.h"
#include "../kernel/fs.h"
#include "../kernel/fcntl.h"
#include "../kernel/stat.h"

void find(char *path, char *name) {

  int fd;
  struct stat st;
  struct dirent de;
  char *p, buf[512], *tmp;

  if ((fd = open(path, O_RDONLY)) < 0) {
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if (fstat(fd, &st) < 0) {
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch (st.type) {
  case T_DEVICE:
  case T_FILE: 
    p = path;
    while (*p++)
      ;
    for (; path <= p && *p != '/'; p--)
      ;
    p++;

    if (strcmp(p, name) == 0)
      printf("%s\n", path);
    break;

  case T_DIR:
    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)) {
      printf("find: path too long\n");
      break;
    }

    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
      strcpy(buf, path);
      p = buf + strlen(buf);
      *p++ = '/';

      if (de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) 
        continue;
      
      tmp = buf; 
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      
      find(tmp, name);
    }

    break;
  }

  close(fd);
}

int main(int argc, char *argv[]) {
  
  if (argc < 3) {
    fprintf(2, "usage: find path name\n");
    exit(0);
  }

  find(argv[1], argv[2]);

  exit(0);
}
