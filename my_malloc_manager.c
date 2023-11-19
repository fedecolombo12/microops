#include "my_malloc_manager.h"

/* Inicializa admin de memoria. Crea el primer bloque (first_chunk) 
llamando a create_new_chunk con el tamaño especificado (UNITS_PER_CHUNK). */

void my_malloc_init(){
    first_chunk = create_new_chunk(UNITS_PER_CHUNK, 0, NULL);
}

/* Utiliza para asignar memoria dinámica. 
Calcula la cantidad de unidades necesarias para satisfacer la solicitud, busca en los bloques existentes 
o crea nuevos bloques según sea necesario, y marca la memoria asignada. */

void *my_malloc(size_t nbytes){
    uint16_t units_needed = (nbytes + sizeof(AllocationHeader) + UNIT_SIZE - 1) / UNIT_SIZE;
    MemoryChunkHeader *chunk = NULL;
    int bit_index;
    int is_large_allocation = IS_LARGE_ALLOCATION(units_needed); // indica si la asignación es grande.
    if (first_chunk == NULL) { //Se verifica si el primer bloque ya existe; si no, se inicializa el administrador de memoria.
        my_malloc_init(); 
    }
    if (is_large_allocation) { // Dependiendo de si es una asignación grande o estándar, se toman diferentes caminos.
        printf("\n New chunk for large allocation will be created. We need %hd units.\n", units_needed);
        chunk = first_chunk->next = create_new_chunk(units_needed, 1, first_chunk->next); 
        bit_index = STRUCT_UNITS;
    } else {
        for (chunk = first_chunk; chunk != NULL; chunk = chunk->next) {
            printf("Units needed %hd. \n", units_needed);
            printf("Will look in chunk id %hd. \n", chunk->id);
            if (units_needed > chunk->chunk_available_units) {
                printf("Not enough space in chunk id %hd that has %hd available units\n", chunk->id, chunk->chunk_available_units);
                continue; 
            }
            bit_index = first_fit(chunk->bitmap, chunk->bitmap_size, units_needed);
            if (bit_index == -1) {
                printf("Not enough space for first fit in chunk id %hd\n", chunk->id);
            } else {
                break;
            }
        }
        if (chunk == NULL) {
            printf("\nNew chunk for standard allocation will be created. We need %hd units. \n", units_needed);
            chunk = first_chunk->next = create_new_chunk (units_needed, 0, first_chunk->next); 
            bit_index = first_fit(chunk->bitmap, chunk->bitmap_size, units_needed);
            if (bit_index == -1) {
                printf("Not enough space for first fit in chunk id %hd\n", chunk->id);
            }
        }
        printf("\nFound a hole in chunk id %hd at bit index %d\n", chunk->id, bit_index);
    }
    chunk->chunk_available_units -= units_needed;
    size_t offset = bit_index * UNIT_SIZE;
    AllocationHeader *allocation_header = (AllocationHeader *) ((char *) chunk->address + offset);
    allocation_header->nunits = is_large_allocation ? 0 : units_needed;
    allocation_header->bit_index = bit_index;
    
    return (char *) allocation_header + sizeof (AllocationHeader);
}

/* Libera la memoria previamente asignada por my_malloc. 
Recupera información del encabezado de asignación y, dependiendo de si es una asignación estándar o grande, 
ajusta el bitmap o libera completamente el bloque utilizando munmap si es una asignación grande. */

void my_free(void *ptr){
    if (ptr == NULL) {
        return;
    }
    AllocationHeader *allocation_header = (AllocationHeader *) ((char *) ptr - sizeof (AllocationHeader));
    MemoryChunkHeader *chunk = (MemoryChunkHeader *) ((char *) allocation_header - allocation_header->bit_index * UNIT_SIZE);
    if (chunk->is_large_allocation) {
        printf("Freeing a large allocation. Chunk id %hd\n", chunk->id);
        munmap(chunk->address, chunk->chunk_total_units * UNIT_SIZE);
        return;
    }
    printf("Freeing a standard allocation. Chunk id %hd\n", chunk->id);
    set_or_clear_bits(0, chunk->bitmap, allocation_header->bit_index / 8, allocation_header->bit_index % 8, allocation_header->nunits);
    chunk->chunk_available_units += allocation_header->nunits;
    if (chunk->chunk_available_units == UNITS_PER_CHUNK - STRUCT_UNITS - BITMAP_UNITS) {
        printf("Freeing a chunk. Chunk id %hd\n", chunk->id);
        if (chunk == first_chunk) {
            first_chunk = chunk->next;
        } else {
            MemoryChunkHeader *prev_chunk = first_chunk;
            while (prev_chunk->next != chunk) {
                prev_chunk = prev_chunk->next;
            }
            prev_chunk->next = chunk->next;
        }
        munmap(chunk->address, chunk->chunk_total_units * UNIT_SIZE);
    }
}