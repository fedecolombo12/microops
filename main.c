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

void set_or_clear_bits(int set, unsigned char *bitmap, size_t bitmap_size, int start_index, size_t qty) {
    if (start_index < 0 || start_index >= bitmap_size * 8) {
        printf("Invalid start index\n");
        return;
    }

    int byte_to_modify = start_index / 8;
    int bit_to_modify = start_index % 8;

    for (size_t i = 0; i < qty; i++) {
        if (set == 1) {
            // Establecer bits
            bitmap[byte_to_modify] |= (1 << (7 - bit_to_modify));
        } else if (set == 0) {
            // Borrar bits
            bitmap[byte_to_modify] &= ~(1 << (7 - bit_to_modify));
        } else {
            printf("Invalid set value\n");
            return;
        }

        start_index++;
        byte_to_modify = start_index / 8;
        bit_to_modify = start_index % 8;
    }

    printf("%s %zu units starting at index %d.\n", set ? "Set" : "Cleared", qty, start_index - qty);
    printf("Updated Bitmap: ");
    print_bitmap(bitmap, bitmap_size);
}

int main() {
    unsigned char bitmap[BITMAP_SIZE] = {0};
    printf("Bitmap Size: %d bytes\n", BITMAP_SIZE);
    while (1) {
        printf("Enter the number of bits to occupy (0 to exit, -1 to set/clear): ");
        int action;
        scanf("%d", &action);
        if (action == 0) {
            break;
        } else if (action == -1) {
            int start_byte_index, start_bit_index, qty;
            printf("Enter start byte index: ");
            scanf("%d", &start_byte_index);
            printf("Enter start bit index: ");
            scanf("%d", &start_bit_index);
            printf("Enter quantity: ");
            scanf("%d", &qty);
            int set;
            printf("Enter 1 to set or 0 to clear: ");
            scanf("%d", &set);
            set_or_clear_bits(set, bitmap, BITMAP_SIZE, start_byte_index * 8 + start_bit_index, qty);
        } else {
            int index = first_fit(bitmap, BITMAP_SIZE, action);
            if (index != -1) {
                printf("Allocated %d units starting at index %d.\n", action, index);
                printf("Updated Bitmap: ");
                print_bitmap(bitmap, BITMAP_SIZE);
            } else {
                printf("Failed to allocate %d units.\n", action);
            }
        }
    }
    return 0;
}
