#include <arpa/inet.h>
#include <stdbool.h>
#include <math.h>
#include "clib.h"

#ifndef MY_HEADER_C_H
#define MY_HEADER_C_H

uint32_t convert_ip_to_int (char* input);
char* convert_int_to_ip (uint32_t input);
bool is_local(uint32_t ip);
void build_masks_ones (uint32_t *mask_one);
void build_masks_zero(uint32_t *mask_zero);
bool contains(uint32_t network, int mask, uint32_t IP, uint32_t *mask_one);
int get_mask_matches(int IP1_pos, int IP2_pos, uint32_t *mask_one, uint32_t *ips_t_array);
int get_mask_matches2(uint32_t IP1, uint32_t IP2, uint32_t *mask_one);
bool is_special_large(uint32_t IP, int mask, uint32_t *mask_zero);
bool is_special(uint32_t IP, int mask, uint32_t *mask_zero);
void showbits(uint32_t x);
bool isValidIp4(const char *str);
bool ip_is_zero(uint32_t IP);
bool ips_equal(uint32_t IP1, uint32_t IP2);
int get_no_of_traces_in_file(char const * file_path);
uint32_t get_abs_uint32(uint32_t input1, uint32_t input2);
void print_uint32_array(uint32_t *array, int size);

#endif
