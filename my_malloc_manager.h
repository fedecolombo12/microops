#ifndef MY_MALLOC_MANAGER_H
#define MY_MALLOC_MANAGER_H

#include <stddef.h>

#define BITMAP_SIZE 16

int first_fit(unsigned char *bitmap, size_t bitmap_size, size_t units_needed);
void print_bitmap(unsigned char *bitmap, size_t bitmap_size);

#endif // MY_MALLOC_MANAGER_H