#include <stdio.h>
#include <assert.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#define TO_STR(x) #x

struct template_data {
  char *key;
  char *value;
};

int execute_template(int output_location_fd, const char *template_name, struct template_data **data) {
  struct stat st = {0};
  if ((stat(template_name, &st)) != 0) {
    fprintf(stderr, "Error accessing stats on file = (%s): %s\n", template_name, strerror(errno));
    return -1;
  }

  assert(output_location_fd > -1);
  if (data == NULL) {
    int template_fd = open(template_name, O_RDONLY);
    if (template_fd == -1) {
      fprintf(stderr, "Error opening template file = (%s) for reading: %s\n", template_name, strerror(errno));
      return -1;
    }

    char *template_buffer = malloc(st.st_size);
    if (template_buffer == NULL) {
      fprintf(stderr, "Error creating buffer for template = (%s): %s\n", template_name, strerror(errno));
      close(template_fd);
      return -1;
    }

    int bytes_read = read(template_fd, template_buffer, st.st_size);
    assert(bytes_read == st.st_size);
    if (bytes_read == -1) {
      fprintf(stderr, "Error reading template = (%s) to buffer: %s\n", template_name, strerror(errno));
      close(template_fd);
      free(template_buffer);
      return -1;
    }

    int bytes_wrote = write(output_location_fd, template_buffer, st.st_size);
    assert(bytes_wrote == st.st_size);
    if (bytes_wrote == -1) {
      fprintf(stderr, "Error opening template file = (%s) for writing: %s\n", template_name, strerror(errno));
      close(template_fd);
      free(template_buffer);
      return -1;
    }

    return 0;
  } 

  printf("todo: handle case where the data struct != NULL\n");
  return 0;
};

int main() {
  printf("Testing out null data executeing\n");
  int err = execute_template(STDOUT_FILENO, "test-input/no-slot.html", NULL);
  if (err != 0) {
    printf("we fuck something up :(\n");
    return -1;
  }

  printf("hey hey hey that shit apparently worded ;)\n");
  return 0;
}
