#include "my_malloc_manager.h"

/* Establece o borra bits en un bitmap. 
Dependiendo del valor del parámetro set, la función establecerá (set = 1) o borrará (set = 0) 
ciertos bits en el bitmap, según la posición de inicio y la cantidad especificada. */

void set_or_clear_bits(int set, Bitmap bitmap, uint16_t start_byte_index, uint16_t start_bit_index, uint16_t qty) {
    if (set) { // Escribe unos en el bitmap
        for (int i = 0; i < qty; i++) {
            bitmap[start_byte_index] |= (1 << (7 - (start_bit_index + i)));
        }
    } else { // Escribe ceros en el bitmap
        for (int i = 0; i < qty; i++) {
            bitmap[start_byte_index] &= ~(1 << (7 - (start_bit_index + i)));
        }
    }
}