#include <assert.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define OUT STDOUT_FILENO

struct string_t {
  char *data;
  size_t len;
};

struct string_t string_from(int n, char *data) {
  struct string_t res = {.len = n, .data = data};
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
  char *buffer = calloc(1, sizeof(char) * (size_t)m + 1);
  assert(buffer);

  size_t n = fread(buffer, 1, (size_t)m, f);
  assert(n == (size_t)m);

  fclose(f);
  return string_from(n, buffer);
}

struct string_t chop_until_delim(struct string_t *string, const char delim) {
  if (string->len < 0) {
    return string_from(0, NULL);
  };

  size_t i = 0;
  while (i < string->len && string->data[i] != delim)
    i++;

  struct string_t line;
  line.data = string->data;
  line.len = i;

  if (i == string->len) {
    string->data += string->len;
    string->len = 0;
  } else {
    string->data += i + 1;
    string->len -= i + 1;
  }

  return line;
}

void compile_c_code(struct string_t s) { printf("%.*s\n", (int)s.len, s.data); }

void compile_byte_array(struct string_t s) {
  printf("write(OUT, \"");
  for (uint64_t i = 0; i < s.len; i++) {
    printf("\\x%02x", s.data[i]);
  }
  printf("\", %lu);\n", s.len);
}

#define COMPILE_TEMPLATE(template_filepath)                                   \
  do {                                                                         \
    struct string_t template = file_to_buff((template_filepath));              \
    int c_code_mode = 0;                                                       \
    while (template.len) {                                                     \
      struct string_t token = chop_until_delim(&template, '%');                \
      if (c_code_mode) {                                                       \
        compile_c_code(token);                                                 \
      } else {                                                                 \
        compile_byte_array(token);                                             \
      }                                                                        \
      c_code_mode = !c_code_mode;                                              \
    }                                                                          \
  } while (0);

int main(/**int argc, char **argv*/ void) {
  // if (argc != 2) {
  //   printf("Ussage: %s <template_filepath.>\n", argv[0]);
  //   return -1;
  // }

  // struct string_t template = file_to_buff(argv[1]);

  // int c_code_mode = 0;
  // while (template.len) {
  //   struct string_t token = chop_until_delim(&template, '%');
  //   if (c_code_mode) {
  //     compile_c_code(token);
  //   } else {
  //     compile_byte_array(token);
  //   }
  //   c_code_mode = !c_code_mode;
  // }

  COMPILE_TEMPLATE("test.html");
  return 0;
}
