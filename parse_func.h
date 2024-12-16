#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdbool.h>
#include <fcntl.h>
#include <ctype.h> 

#ifndef PARSE_FUNC
#define PARSE_FUNC

#define MAX_CMD_INPUT 100
#define MAX_DIR_LENGTH 100

void evaluate_input(char *line_input);

void add_char_to_word(char* word, char c);

void break_into_words(char *input, char* words[], int *number_of_elems, char break_on);

void isolate_arguments_in(char *words[], int number_of_words, char *arguments[]);

bool find_absolute_path(char *no_path, char* with_path);

bool find_file_directories(int *input_source, int *output_source, char *words[], int word_count);

void print_char(char *char_arr[], int arr_len);

#endif
