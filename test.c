#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <fcntl.h>

#define OUT STDOUT_FILENO

struct string_t {
  char *data;
  size_t len;
};

struct string_t string_from(int n, char *data) {
  struct string_t res = { .len = n, .data = data };
  return res;
}

struct string_t file_to_buff(const char *filepath) {
    assert(filepath);

    FILE *f = fopen(filepath, "rb");
    assert(f);

    fseek(f, 0, SEEK_END);
    long m = ftell(f);
    assert(m >= 0);
    fseek(f, 0, SEEK_SET);
    char *buffer = calloc(1, sizeof(char) * (size_t) m + 1);
    assert(buffer);

    size_t n = fread(buffer, 1, (size_t) m, f);
    assert(n == (size_t) m);

    fclose(f);
    return string_from(n, buffer);
}

int main(void) {
  int yes = 1;

  int fd = open("__test.h", O_CREAT | O_RDWR);
  assert(fd > 0);

  char *cursor; 

  struct string_t string = file_to_buff("test.html");
  char *result1 = strstr(string.data, "<%");
  assert(result1);
  int n  = &result1[0] - &string.data[0];
  write(fd, string.data, n); 
  printf("number:  %d\n", n);

  cursor = &result1[n + 2];
  write(fd, string.data, n - string.len); 
  char *result2 = strstr(string.data, "%>");
  write(fd, &string.data[n], &result2[0] - &cursor[0]); 

  close(fd);
  return 0;

  //printf("%s\n", result1);
  //printf("%s\n", cursor);

  assert(result2);
  //cursor = &result2[0] + 2;
  result2[0] = (char)0;
  printf("%s\n", cursor);

  //printf("%s\n", result2);
  //printf("%s\n", cursor);
  //write(OUT, string.data, string.len);
  close(fd);
}
