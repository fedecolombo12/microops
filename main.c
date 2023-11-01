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

void free_block(unsigned char *bitmap, size_t bitmap_size, int block_index, size_t units_to_free) {
    if (block_index < 0 || block_index >= bitmap_size * 8) {
        printf("Invalid block index\n");
        return;
    }

    int byte_to_clear = block_index / 8;
    int bit_to_clear = block_index % 8;

    // Verificar si el bloque está asignado
    if (!(bitmap[byte_to_clear] & (1 << (7 - bit_to_clear)))) {
        printf("Block at index %d is not allocated\n", block_index);
        return;
    }

    for (int i = 0; i < units_to_free; i++) {
        int bit_to_clear_i = (block_index + i) % 8;
        int byte_to_clear_i = (block_index + i) / 8;
        bitmap[byte_to_clear_i] &= ~(1 << (7 - bit_to_clear_i));
    }

    printf("Freed %zu units starting at index %d.\n", units_to_free, block_index);

    printf("Updated Bitmap: ");
    print_bitmap(bitmap, bitmap_size);
}


int main() {
    unsigned char bitmap[BITMAP_SIZE] = {0};

    printf("Bitmap Size: %d bytes\n", BITMAP_SIZE);

    while (1) {
        printf("Enter the number of bits to occupy (0 to exit, -1 to free): ");
        int units_needed;
        scanf("%d", &units_needed);

        if (units_needed == 0) {
            break;
        } else if (units_needed == -1) {
            int block_index_to_free;
            printf("Enter the block index to free: ");
            scanf("%d", &block_index_to_free);

            size_t units_to_free;
            printf("Enter the number of bits to free: ");
            scanf("%zu", &units_to_free);

            free_block(bitmap, BITMAP_SIZE, block_index_to_free, units_to_free);
        } else {
            int index = first_fit(bitmap, BITMAP_SIZE, units_needed);

            if (index != -1) {
                printf("Allocated %d units starting at index %d.\n", units_needed, index);
                printf("Updated Bitmap: ");
                print_bitmap(bitmap, BITMAP_SIZE);
            } else {
                printf("Failed to allocate %d units.\n", units_needed);
            }
        }
    }

    return 0;
}