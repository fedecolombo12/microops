#include "my_malloc_manager.h"

int first_fit(unsigned char *bitmap, size_t bitmap_size, size_t units_needed) {
    size_t consecutive_free_bits = 0;
    size_t first_free_bit_index = -1;

    for (int i = 0; i < bitmap_size * 8; i++) {
        size_t byte = i / 8;
        size_t bit = i % 8;

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
