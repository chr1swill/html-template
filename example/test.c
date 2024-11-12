#include "../src/html-template.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct test_data {
  const char *test_title;
  int output_location_fd;
  const char *template_name;
  struct template_data *data;
};

int test(struct test_data *t) {
  printf("TESTING: %s\n", t->test_title);
  return execute_template(t->output_location_fd, t->template_name, t->data);
}

int main() {
  printf("running testing\n");

  struct test_data tda[] = {
    {
      .test_title = "No sloted data",
      .output_location_fd = STDOUT_FILENO,
      .template_name = "example/no-slot.html",
      .data = NULL,
    },

    {
      .test_title = "Single sloted data",
      .output_location_fd = STDOUT_FILENO,
      .template_name = "example/single-slot.html",
      .data = (struct template_data[]){
        {"{{title}}", "thank you tom"},
        {NULL, NULL}
      }
    },

    {
      .test_title = "Multiple single data",
      .output_location_fd = STDOUT_FILENO,
      .template_name = "example/multi-single-slot.html",
      .data = (struct template_data[]){
        {"{{title}}", "thank you tom"},
        {NULL, NULL}
      }
    },

    {
      .test_title = "Multiple of each slots of data",
      .output_location_fd = STDOUT_FILENO,
      .template_name = "example/multi-multi-slot.html",
      .data = (struct template_data[]){
        {"{{title}}", "thank you tom"},
        { "{{h1}}", "this is a website about someone names tom" },
        { "{{p}}", "I DONT KNOW ANYONE NAMED TOM" },
        {NULL, NULL}
      }
    },
    { .test_title = NULL, .output_location_fd = -1, .template_name = NULL, .data = NULL }
  };

  for (int i = 0; tda[i].test_title != NULL; i++) {
    puts("");
    int test_result =  test(&tda[i]);
    if (test_result != 0) {
      printf("FAILED TEST %s\n", tda[i].test_title);
      return -1;
    } else {
      printf("PASSED TEST %s\n", tda[i].test_title);
    }
  }

  printf("testing complete\n");
  return 0;
}
