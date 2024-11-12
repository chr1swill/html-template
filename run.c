#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <stdint.h>

#define STRING(x, l) \
  do { \
    write(OUT, (x), (l));\
  } while(0); \

void render() {
  char *john = "hellllllllooo world"; 
#include <string.h>
#define OUT STDOUT_FILENO
#include "test.h"
}

int main(void) {
  render();
  return 0;
}
