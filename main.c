#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#define BITMAP_SIZE 16

int first_fit(unsigned char *bitmap, size_t bitmap_size, size_t units_needed) {
    int consecutive_free_bits = 0;
    int first_free_bit_index = -1;

    for (int i = 0; i < bitmap_size * 8; i++) {
        int byte = i / 8;
        int bit = i % 8;

        if (!(bitmap[byte] & (1 << (7-bit)))) {
            if (consecutive_free_bits == 0) {
                first_free_bit_index = i;
            }
            consecutive_free_bits++;
            if (consecutive_free_bits == units_needed) {
                for (int j = 0; j < units_needed; j++) {
                    int byte_to_set = (first_free_bit_index + j) / 8;
                    int bit_to_set = (first_free_bit_index + j) % 8;
                    bitmap[byte_to_set] |= (1 << (7 - bit_to_set));
                }
                return first_free_bit_index;
            }
        } else {
            consecutive_free_bits = 0;
        }
    }

    return -1; // No se encontró espacio adecuado.
}

void print_bitmap(unsigned char *bitmap, size_t bitmap_size) {
    for (int i = 0; i < bitmap_size; i++) {
        for (int j = 7; j >= 0; j--) {
            printf("%d", (bitmap[i] >> j) & 1);
        }
        if (i < bitmap_size - 1) {
            printf(" ");
        }
    }
    printf(".\n");
}

int main() {
    unsigned char bitmap[BITMAP_SIZE] = {0};

    printf("Bitmap Size: %d bytes\n", BITMAP_SIZE);

    size_t units_needed;

    while (1) {
        printf("Enter the number of bits to occupy (0 to exit): ");
        scanf("%zu", &units_needed);

        if (units_needed == 0) {
            break;
        }

        int index = first_fit(bitmap, BITMAP_SIZE, units_needed);

        if (index != -1) {
            printf("Allocated %zu units starting at index %d.\n", units_needed, index);
            printf("Updated Bitmap: ");
            print_bitmap(bitmap, BITMAP_SIZE);
        } else {
            printf("Failed to allocate %zu units.\n", units_needed);
        }
    }

    return 0;
}