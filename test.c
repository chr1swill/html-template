#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

int process_template(const char *template_filepath) {
  int template_filepath_len = strlen(template_filepath);
  assert(template_filepath_len < (256 - 2));

  struct string_t template_output_filepath;

  template_output_filepath.len = template_filepath_len + 2;
  template_output_filepath.data = malloc(sizeof(char) * template_output_filepath.len);
  if (template_output_filepath.data == NULL) {
    fprintf(stderr, "Failed to allocate memory for template data\n");
    return -1;
  }

  memmove(template_output_filepath.data, (template_filepath), template_filepath_len);

  template_output_filepath.data[template_filepath_len] = '.';
  template_output_filepath.data[template_filepath_len + 1] = 'h';
  assert(template_output_filepath.data[template_filepath_len - 1] != '\0');

  int fd = open(template_output_filepath.data, O_CREAT | O_RDWR, S_IRWXU);
  if (fd < 0) {
    fprintf(stderr, "Error opening/creating file in read/write mode: %s\n",
            strerror(errno));
    free(template_output_filepath.data);
    return -1;
  }

  struct string_t template = file_to_buff((template_filepath));
  int c_code_mode = 0;
  while (template.len) {
    struct string_t token = chop_until_delim(&template, '%');
    if (c_code_mode) {
      compile_c_code(token);
    } else {
      compile_byte_array(token);
    }
    c_code_mode = !c_code_mode;
  }

  free(template_output_filepath.data);
  return fd;
}

int main(void) {
  int template_fd = process_template("test.html");
  if (template_fd < 0) {
    fprintf(stderr, "Error compiling template\n");
    return -1;
  }

  return 0;
}
