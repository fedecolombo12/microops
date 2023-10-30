#include "my_malloc_manager.h"
#include <stdio.h>

int first_fit(unsigned char *bitmap, size_t bitmap_size, size_t units_needed) {
    for (size_t i = 0; i < bitmap_size * 8 - units_needed + 1; i++) {
        int j;
        for (j = 0; j < units_needed; j++) {
            if (bitmap[i / 8] & (1 << (i % 8 + j))) {
                break;
            }
        }
        if (j == units_needed) {
            for (int k = 0; k < units_needed; k++) {
                bitmap[i / 8] |= (1 << (i % 8 + k));
            }
            return i;
        }
    }
    return -1;
}

void print_bitmap(unsigned char *bitmap, size_t bitmap_size) {
    for (size_t i = 0; i < bitmap_size; i++) {
        for (int j = 7; j >= 0; j--) {
            if (bitmap[i] & (1 << j)) {
                printf("1");
            } else {
                printf("0");
            }
        }
        if (i < bitmap_size - 1) {
            printf(" ");
        }
    }
    printf("\n");
}
