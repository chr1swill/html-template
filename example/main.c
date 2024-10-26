#include "../src/html-template.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
  printf("TESTING: single sloted data\n");
  int err = execute_template(STDOUT_FILENO, "example/multi-multi-slot.html", 
      (struct template_data[]){ { "{{title}}", "thank you tom" },/* { "{{h1}}", "this is a website about someone names tom" }, { "{{p}}", "I DONT KNOW ANYONE NAMED TOM" },*/ { NULL, NULL }});
  if (err != 0) {
    printf("we fuck something up :(\n");
    return -1;
  }

  printf("hey hey hey that shit apparently worded ;)\n");
  return 0;
}
