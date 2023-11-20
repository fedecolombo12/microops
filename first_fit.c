#include "my_malloc_manager.h"

// Implementa un algoritmo de asignación de memoria.
// Busca en un bitmap (una representación compacta de bits) para encontrar la primera secuencia de bits libres del tam especificado. 
// Si encuentra un bloque libre, lo marca como ocupado y devuelve el índice del primer bit en ese bloque.

int first_fit(unsigned char *bitmap, size_t bitmap_size, size_t units_needed) {
    size_t consecutive_free_bits = 0;  // conteo de los bits consecutivos libres encontrados.
    size_t first_free_bit_index = -1; // guarda la posición del primer bit libre en una secuencia.
    for (size_t i = 0; i < bitmap_size * 8; i++) { // Se itera sobre cada bit del bitmap.
        size_t byte = i / 8; // Se calcula byte y bit para acceder a los bits del bitmap.
        size_t bit = i % 8;
        if (!(bitmap[byte] & (1 << (7-bit)))) { // verifica si el bit actual es libre
            // Se actualizan las variables y, si se encuentra suficiente espacio, 
            // se marcan los bits como ocupados y se devuelve la posición del primer bit libre.
            if (consecutive_free_bits == 0) { 
                first_free_bit_index = i;
            }
            consecutive_free_bits++;
            if (consecutive_free_bits == units_needed) {
                for (size_t j = 0; j < units_needed; j++) {
                    size_t byte_to_set = (first_free_bit_index + j) / 8;
                    size_t bit_to_set = (first_free_bit_index + j) % 8;
                    bitmap[byte_to_set] |= (1 << (7 - bit_to_set));
                }
                return first_free_bit_index;
            }
        } else { //Si no es libre, se reinicia el contador de bits consecutivos libres.
            consecutive_free_bits = 0;
        }
    }
    return -1; // No se encontró espacio adecuado.
}