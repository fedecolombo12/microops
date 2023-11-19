#include "my_malloc_manager.h"

/* Imprime el contenido de un bitmap en formato binario. Itera sobre cada byte del bitmap y, 
para cada bit, imprime su valor (0 o 1). */

void print_bitmap(unsigned char *bitmap, size_t bitmap_size) {
    // Se itera sobre cada byte del bitmap y luego sobre cada bit de ese byte.
    for (size_t byte = 0; byte < bitmap_size; byte++) {
        unsigned char current_byte = bitmap[byte];
        for (int bit = 7; bit >= 0; bit--) { // se calcula usando operaciones de bits y se imprime.
            int bit_value = (current_byte >> bit) & 1;
            printf("%d", bit_value); // Imprimimos cada bit
        }
        // Si no es el último byte se imprime un espacio
        if (byte < bitmap_size - 1) {
            printf(" ");
        }
    }
    printf("\n");
}