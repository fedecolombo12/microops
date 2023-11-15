#include "my_malloc_manager.h"

// Declaración de variables globales


// Función para reservar memoria
void *my_malloc(size_t nbytes) {
    if (nbytes == 0 || nbytes > MAX_MALLOC_SIZE) {
        return NULL; // No se puede reservar memoria
    }

    MemoryChunkHeader *current_chunk = first_chunk;
    int bit_index;

    while (current_chunk != NULL) {
        // Encuentra un espacio en el chunk usando el algoritmo de first fit
        bit_index = first_fit(current_chunk->bitmap, current_chunk->bitmap_size, nbytes / UNIT_SIZE);

        if (bit_index != -1) {
            // Reserva el espacio y devuelve un puntero
            AllocationHeader *allocation_header = (AllocationHeader *)((char *)current_chunk + bit_index * UNIT_SIZE);
            allocation_header->nunits = nbytes / UNIT_SIZE;
            allocation_header->bit_index = bit_index;

            set_or_clear_bits(1, current_chunk->bitmap, bit_index / 8, bit_index % 8, nbytes / UNIT_SIZE);

            return (void *)((char *)allocation_header + sizeof(AllocationHeader));
        }

        current_chunk = current_chunk->next;
    }

    // No se encontró espacio suficiente en ningún chunk, crea un nuevo chunk
    current_chunk = create_new_chunk(UNITS_PER_CHUNK, 0, first_chunk);
    first_chunk = current_chunk;

    bit_index = first_fit(current_chunk->bitmap, current_chunk->bitmap_size, nbytes / UNIT_SIZE);

    if (bit_index != -1) {
        // Reserva el espacio y devuelve un puntero
        AllocationHeader *allocation_header = (AllocationHeader *)((char *)current_chunk + bit_index * UNIT_SIZE);
        allocation_header->nunits = nbytes / UNIT_SIZE;
        allocation_header->bit_index = bit_index;

        set_or_clear_bits(1, current_chunk->bitmap, bit_index / 8, bit_index % 8, nbytes / UNIT_SIZE);

        return (void *)((char *)allocation_header + sizeof(AllocationHeader));
    }

    return NULL; // No se pudo reservar memoria
}

// Función para liberar memoria
void my_free(void *ptr) {
    if (ptr == NULL) {
        return; // No se puede liberar un puntero nulo
    }

    // Calcula la dirección de inicio del bloque de asignación
    AllocationHeader *allocation_header = (AllocationHeader *)((char *)ptr - sizeof(AllocationHeader));

    // Encuentra el chunk correspondiente
    MemoryChunkHeader *current_chunk = first_chunk;

    while (current_chunk != NULL) {
        int bit_index = allocation_header->bit_index;

        if ((char *)allocation_header >= (char *)current_chunk && (char *)allocation_header < (char *)current_chunk + current_chunk->chunk_total_units * UNIT_SIZE) {
            // Marca el espacio como disponible en el bitmap
            set_or_clear_bits(0, current_chunk->bitmap, bit_index / 8, bit_index % 8, allocation_header->nunits);

            // Implementa la lógica para liberar chunks completos si están completamente vacíos
            if (current_chunk->next != NULL && current_chunk->chunk_available_units == current_chunk->chunk_total_units) {
                // Liberar el chunk
                MemoryChunkHeader *temp = current_chunk->next;
                munmap(current_chunk->address, current_chunk->chunk_total_units * UNIT_SIZE);
                current_chunk = temp;
            }

            break;
        }

        current_chunk = current_chunk->next;
    }
}

// Implementación de create_new_chunk y otras funciones si es necesario
// ...

