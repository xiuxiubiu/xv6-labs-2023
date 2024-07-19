#include "../kernel/types.h"
#include "user.h"

void test() {
  printf("I'm sigalarm test!\n");
}

void call(void (*f)()) {
  f();    
}


int main() {
  sigalarm(3, test);
  return 0;
}



