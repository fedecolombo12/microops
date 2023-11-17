#include "my_malloc_manager.h"

// Implementación de la función create_new_chunk
void *create_new_chunk(uint16_t units_needed, int is_large_allocation, MemoryChunkHeader *next) {
    // Calcula el total de unidades para mmap
    MemoryChunkHeader *new_chunk = NULL;
    uint16_t total_units_to_mmap = is_large_allocation ? units_needed + STRUCT_UNITS : UNITS_PER_CHUNK; 
    uint16_t used_units = is_large_allocation ? STRUCT_UNITS : STRUCT_UNITS + BITMAP_UNITS;
    // Reserva memoria con mmap
    void *mem = mmap(NULL, total_units_to_mmap * UNIT_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);

    if (mem == MAP_FAILED) {
        printf("Error");
        //error(EXIT_FAILURE, errno, "Memory mapping failed");
    }
    // Inicializa el header del chunk
    new_chunk = (MemoryChunkHeader *)mem;
    new_chunk->address = mem;
    new_chunk->id = 0;
    new_chunk->is_large_allocation = is_large_allocation;
    new_chunk->chunk_total_units = total_units_to_mmap;
    new_chunk->chunk_available_units = total_units_to_mmap - used_units;
    new_chunk->bitmap = is_large_allocation ? NULL: (unsigned char *)mem + STRUCT_UNITS * UNIT_SIZE; // Establece en NULL por ahora
    new_chunk->bitmap_size = is_large_allocation ? 8 : BITMAP_SIZE; // Establece en 0 por ahora
    new_chunk->next = next; 
    if(!is_large_allocation){
        // Inicializa el bitmap
        for (u_int16_t i = 0; i < used_units; i++) {
            u_int16_t byte_index = i / 8;
            u_int16_t bit_index = i % 8;
            new_chunk->bitmap[byte_index]  |= (1 << (7 - bit_index));
        }
    }
    return new_chunk;
}