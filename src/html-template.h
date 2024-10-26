#ifndef _HTML_TEMPLATE_H
#define _HTML_TEMPLATE_H

struct template_data {
  char *key;
  char *value;
};

int replace_word(char* str, char* old_word, char* new_word);

int execute_template(int output_location_fd, const char *template_name, struct template_data data[]);

#endif //_HTML_TEMPLATE_H
