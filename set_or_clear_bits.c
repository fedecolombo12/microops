#include "my_malloc_manager.h"

// Implementación de la función set_or_clear_bits
void set_or_clear_bits(int set, Bitmap bitmap, uint16_t start_byte_index, uint16_t start_bit_index, uint16_t qty) {
    if (set) {
        // Escribe unos en el bitmap
        for (int i = 0; i < qty; i++) {
            bitmap[start_byte_index] |= (1 << (7 - (start_bit_index + i)));
        }
    } else {
        // Escribe ceros en el bitmap
        for (int i = 0; i < qty; i++) {
            bitmap[start_byte_index] &= ~(1 << (7 - (start_bit_index + i)));
        }
    }
}