#include "my_malloc_manager.h"

/* Establece o borra bits en un bitmap. 
Dependiendo del valor del parámetro set, la función establecerá (set = 1) o borrará (set = 0) 
ciertos bits en el bitmap, según la posición de inicio y la cantidad especificada. */

/*(void set_or_clear_bits(int set, Bitmap bitmap, uint16_t start_byte_index, uint16_t start_bit_index, uint16_t qty) {
    if (set) { // Escribe unos en el bitmap
        for (int i = 0; i < qty; i++) {
            bitmap[start_byte_index] |= (1 << (7 - (start_bit_index + i)));
        }
    } else { // Escribe ceros en el bitmap
        for (int i = 0; i < qty; i++) {
            bitmap[start_byte_index] &= ~(1 << (7 - (start_bit_index + i)));
        }
    }
} */

void set_or_clear_bits(int set, Bitmap bitmap, uint16_t start_byte_index, uint16_t start_bit_index, uint16_t qty) {
    unsigned char mask; // Mask to isolate target bit within a byte
    uint16_t byte_index, bit_index;
    // Loop through the bytes, starting from start_byte_index
    for (byte_index = start_byte_index; qty > 0; byte_index++) {
        mask = 0x80 >> start_bit_index; // 0x80 is binary 1000 0000. Initialize mask based on start_bit_index
        // Loop through the bits within a byte
        for (bit_index = start_bit_index; qty > 0 && bit_index < 8; bit_index++, qty--) {
            if (set) {
                bitmap[byte_index] |= mask; // Set the bit to 1
            } else {
                bitmap[byte_index] &= ~mask; // Set the bit to 0
            }
            mask >>= 1; // Shift mask for next bit
        }
        start_bit_index = 0; // Reset start_bit_index for next byte
    }
}