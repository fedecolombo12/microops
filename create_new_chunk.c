#include "my_malloc_manager.h"

/* Crea nuevo bloque de memoria (MemoryChunkHeader) utilizando la función mmap. 
Utiliza para asignar memoria en el espacio de direcciones del proceso. 
La función también inicializa el encabezado del nuevo bloque con información relevante, 
como el tamaño total, la disponibilidad de unidades, el puntero al bitmap (si es aplicable), y 
la conexión al siguiente bloque en la lista. */

MemoryChunkHeader *create_new_chunk(uint16_t units_needed, int is_large_allocation, MemoryChunkHeader *next) {
    // Calcula el total de unidades para mmap
    MemoryChunkHeader *new_chunk;
    uint16_t total_units_to_mmap = is_large_allocation ? units_needed + STRUCT_UNITS : UNITS_PER_CHUNK; // Calcula la cantidad total de unidades a asignar en el nuevo bloque.
    uint16_t used_units = is_large_allocation ? STRUCT_UNITS : STRUCT_UNITS + BITMAP_UNITS; // Calcula la cantidad de unidades utilizadas para el bitmap y otros encabezados.
    // Reserva memoria con mmap
    void *mem = mmap(NULL, total_units_to_mmap * UNIT_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (mem == MAP_FAILED) {
        printf("Error");
    }
    new_chunk = (MemoryChunkHeader *)mem; // Inicializa el header del chunk
    new_chunk->address = mem;
    new_chunk->id = 0;
    // Si es una asignación grande, el bitmap se establece en NULL; 
    // de lo contrario, se calcula su posición en la memoria asignada y se inicializa.
    new_chunk->is_large_allocation = is_large_allocation;
    new_chunk->chunk_total_units = total_units_to_mmap;
    new_chunk->chunk_available_units = total_units_to_mmap - used_units;
    new_chunk->bitmap = is_large_allocation ? NULL: (unsigned char *)mem + STRUCT_UNITS * UNIT_SIZE; // Establece en NULL por ahora
    new_chunk->bitmap_size = is_large_allocation ? 8 : BITMAP_SIZE; // Establece en 0 por ahora
    new_chunk->next = next; 
    if(!is_large_allocation){ // Inicializa el bitmap
        for (uint16_t i = 0; i < used_units; i++) {
            uint16_t byte_index = i / 8;
            uint16_t bit_index = i % 8;
            new_chunk->bitmap[byte_index]  |= (1 << (7 - bit_index));
        }
    }
    return new_chunk; // Devuelve un puntero al nuevo bloque de memoria.
}