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

int replace_word(char* str, char* old_word, char* new_word) {
    char *pos = NULL; 
    char *temp = malloc(strlen(str) + 1);
    if (temp == NULL) {
      fprintf(stderr, "Error allocating temp buffer for string: %s", str);
      return -1;
    }

    printf("old = (%s), new = (%s)\n", old_word, new_word);
    int index = 0;
    int owlen = strlen(old_word);
 
    // Repeat This loop until all occurrences are replaced.
 
    while ((pos = strstr(str, old_word)) != NULL) {
        // Bakup current line
        strcpy(temp, str);
 
        // Index of current found word
        index = pos - str;
 
        // Terminate str after word found index
        str[index] = '\0';
 
        // Concatenate str with new word
        strcat(str, new_word);
 
        // Concatenate str with remaining words after
        // oldword found index.
        strcat(str, temp + index + owlen + 1);
        puts("here");
    }

    temp != NULL ? free(temp) : 0;
    return 0;
}

int execute_template(int output_location_fd, const char *template_name, struct template_data data[]) {
  struct stat st = {0};
  if ((stat(template_name, &st)) != 0) {
    fprintf(stderr, "Error accessing stats on file = (%s): %s\n", template_name, strerror(errno));
    return -1;
  }

  int template_fd = open(template_name, O_RDONLY);
  if (template_fd == -1) {
    fprintf(stderr, "Error opening template file = (%s) for reading: %s\n", template_name, strerror(errno));
    return -1;
  }

  char *template_buffer = malloc(st.st_size + 1);
  if (template_buffer == NULL) {
    fprintf(stderr, "Error creating buffer for template = (%s): %s\n", template_name, strerror(errno));
    close(template_fd);
    return -1;
  } 
  template_buffer[st.st_size + 1] = '\0';

  int bytes_read = read(template_fd, template_buffer, st.st_size);
  assert(bytes_read == st.st_size);
  if (bytes_read == -1) {
    fprintf(stderr, "Error reading template = (%s) to buffer: %s\n", template_name, strerror(errno));
    close(template_fd);
    free(template_buffer);
    return -1;
  }

  assert(output_location_fd > -1);
  if (data == NULL) {
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

  for (int i = 0; data[i].key != NULL && data[i].value != NULL; i++) {
    char *key = data[i].key;
    char *value = data[i].value;
    //printf("key = (%s), value = (%s)\n", key, value);
    
    if ((key[0] == '{') && (key[1] == '{') &&
        (key[strlen(key)] == '}') && (key[strlen(key) - 1] == '}')) {
      fprintf(stderr, "Error template syntax invalid, expected '{{key_name}}' but got; %s\n", key);
      close(template_fd);
      free(template_buffer);
      return -1;
    }

    if ((strcmp(key, "")) == 0) {
      fprintf(stderr, 
          "Error cannot replace empty string with something: key = (%s), value = (%s)," 
          "expected template slot format '{{key_name}}'\n",
          key, value);
      close(template_fd);
      free(template_buffer);
      return -1;
    };

   int rc = replace_word(template_buffer, key, value);
   if (rc != 0) {
     fprintf(stderr, 
         "Error replacing key = (%s) with value = (%s) in template: %s",
         key, value, template_name);
     close(template_fd);
     free(template_buffer);
     return -1;
   }
  }

  int bytes_wrote = write(output_location_fd, template_buffer, strlen(template_buffer));
  if (bytes_wrote == -1) {
    fprintf(stderr, "Error writing data filled template buffer to you specified location: %d\n", output_location_fd);
     close(template_fd);
     free(template_buffer);
     return -1;
  }

  return 0;
};

int main() {
  printf("TESTING: single sloted data\n");
  int err = execute_template(STDOUT_FILENO, "test-input/multi-multi-slot.html", 
      (struct template_data[]){ { "{{title}}", "fuck you tom" }, { "{{h1}}", "this is a website about someone names tom" }, { "{{p}}", "I DONT KNOW ANYONE NAMED TOM" }, { NULL, NULL }});
  if (err != 0) {
    printf("we fuck something up :(\n");
    return -1;
  }

  printf("hey hey hey that shit apparently worded ;)\n");
  return 0;
}
