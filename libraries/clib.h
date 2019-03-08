#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#define MAXSTRING 5000

#ifndef MY_HEADER_D_H
#define MY_HEADER_D_H

int num_places (int n);
char* check_whitespace(char *str);
char* convert_int_to_char(int input);
int min(int a, int b);
int max(int a, int b);
uint32_t min_uint32(uint32_t a, uint32_t b);
uint32_t max_uint32(uint32_t a, uint32_t b);
double get_rate(int a, int b);
char *strdup(const char *str);
char* getFileNameFromPath(const char* local_file);
int find(uint32_t *array, uint32_t data, int size);
char* return_filename_string(char const* output_folder, char* p);
int get_no_of_lines_in_file(char const * file_path);

#endif
