#include "my_malloc_manager.h"

void print_bitmap(unsigned char *bitmap, size_t bitmap_size) {
    for (size_t byte = 0; byte < bitmap_size; byte++) {
        
        unsigned char current_byte = bitmap[byte];

        for (int bit = 7; bit >= 0; bit--) {
            int bit_value = (current_byte >> bit) & 1;
            // Imprimimos cada bit
            printf("%d", bit_value);
        }

        // Si no es el último byte se imprime un espacio
        if (byte < bitmap_size - 1) {
            printf(" ");
        }
    }

    printf("\n");
}