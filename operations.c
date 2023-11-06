#include <stdio.h>
#include <stdlib.h>
#include "my_malloc_manager.h"
#include <sys/mman>
#include <error.h>
#include <errno.h>

void *first_chunk = NULL; // Debes inicializar esta variable en otro archivo
uint16_t chunk_current_id = 0; // Debes inicializar esta variable en otro archivo

// Función para crear un nuevo chunk
void *create_new_chunk(uint16_t units_needed, int is_large_allocation, MemoryChunkHeader *next) {
    // Calcula el total de unidades para mmap
    uint16_t total_units_to_mmap = units_needed + sizeof(MemoryChunkHeader) / UNIT_SIZE;
    
    void *mem = mmap(NULL, total_units_to_mmap * UNIT_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    
    if (mem == MAP_FAILED) {
        error(EXIT_FAILURE, errno, "Memory mapping failed");
    }
    
    // Inicializa y configura el nuevo chunk
    MemoryChunkHeader *new_chunk = (MemoryChunkHeader *)mem;
    new_chunk->id = chunk_current_id++;
    new_chunk->is_large_allocation = is_large_allocation;
    new_chunk->chunk_total_units = units_needed;
    new_chunk->chunk_available_units = units_needed;
    new_chunk->bitmap = NULL; // Establece en NULL por ahora
    new_chunk->bitmap_size = 0; // Establece en 0 por ahora
    new_chunk->next = next;
    
    return new_chunk;
}

// Función auxiliar para buscar un chunk disponible o crear uno nuevo
MemoryChunkHeader *find_or_create_chunk(size_t units_needed) {
    MemoryChunkHeader *current_chunk = (MemoryChunkHeader *)first_chunk;
    MemoryChunkHeader *prev_chunk = NULL;

    while (current_chunk) {
        if (current_chunk->is_large_allocation == 0 && current_chunk->chunk_available_units >= units_needed) {
            return current_chunk; // Se encontró un chunk adecuado
        }

        prev_chunk = current_chunk;
        current_chunk = current_chunk->next;
    }

    // No se encontró un chunk disponible, crea uno nuevo
    MemoryChunkHeader *new_chunk = create_new_chunk(units_needed, 0, NULL);

    if (prev_chunk) {
        prev_chunk->next = new_chunk;
    } else {
        first_chunk = new_chunk;
    }

    return new_chunk;
}

// Implementación de my_malloc
void *my_malloc(size_t nbytes) {
    if (nbytes == 0) {
        return NULL; // No se permite asignar memoria de tamaño 0
    }

    size_t units_needed = (nbytes + sizeof(AllocationHeader) - 1) / sizeof(AllocationHeader) + 1;

    MemoryChunkHeader *chunk = find_or_create_chunk(units_needed);

    if (chunk) {
        // Encuentra un espacio en el chunk y marca los bits en el bitmap
        for (int i = 0; i < chunk->chunk_total_units; i++) {
            if ((i + units_needed) > chunk->chunk_total_units) {
                continue; // No hay suficiente espacio en este chunk
            }

            if (bitmap_find_free_space(chunk->bitmap, i, units_needed)) {
                AllocationHeader *header = (AllocationHeader *)(chunk + 1);
                header->nunits = units_needed;
                header->bit_index = i;

                chunk->chunk_available_units -= units_needed;
                return (void *)(header + 1);
            }
        }
    }

    return NULL; // No se pudo asignar memoria
}

// Implementación de my_free
void my_free(void *ptr) {
    if (ptr == NULL) {
        return; // No es necesario liberar memoria nula
    }

    AllocationHeader *header = ((AllocationHeader *)ptr) - 1;
    MemoryChunkHeader *chunk = (MemoryChunkHeader *)((char *)header - (header->bit_index * UNIT_SIZE));

    // Marca los bits correspondientes en el bitmap como disponibles
    bitmap_mark_space_free(chunk->bitmap, header->bit_index, header->nunits);
    chunk->chunk_available_units += header->nunits;
}
