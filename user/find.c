#include "../kernel/types.h"
#include "user.h"
#include "../kernel/fs.h"
#include "../kernel/fcntl.h"
#include "../kernel/stat.h"

// Regexp matcher from Kernighan & Pike,
// The Practice of Programming, Chapter 9, or
// https://www.cs.princeton.edu/courses/archive/spr09/cos333/beautiful.html

int matchhere(char*, char*);
int matchstar(int, char*, char*);

int
match(char *re, char *text)
{
  if(re[0] == '^')
    return matchhere(re+1, text);
  do{  // must look at empty string
    if(matchhere(re, text))
      return 1;
  }while(*text++ != '\0');
  return 0;
}

// matchhere: search for re at beginning of text
int matchhere(char *re, char *text)
{
  if(re[0] == '\0')
    return 1;
  if(re[1] == '*')
    return matchstar(re[0], re+2, text);
  if(re[0] == '$' && re[1] == '\0')
    return *text == '\0';
  if(*text!='\0' && (re[0]=='.' || re[0]==*text))
    return matchhere(re+1, text+1);
  return 0;
}

// matchstar: search for c*re at beginning of text
int matchstar(int c, char *re, char *text)
{
  do{  // a * matches zero or more instances
    if(matchhere(re, text))
      return 1;
  }while(*text!='\0' && (*text++==c || c=='.'));
  return 0;
}

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

    // if (strcmp(p, name) == 0)
    if (match(name, p))
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
