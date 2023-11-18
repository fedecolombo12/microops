#include "my_malloc_manager.h"

void my_malloc_init(){
    // initialize the memory manager
    // create the first chunk
    // MemoryChunkHeader *first_chunk = (MemoryChunkHeader*) create_new_chunk(UNITS_PER_CHUNK, 0, NULL);
    first_chunk = create_new_chunk(UNITS_PER_CHUNK, 0, NULL);
    // for (int i = 0; i < BITMAP_SIZE; i++){
    //     header->bitmap[i] = 0;
    // }
}

void *my_malloc(size_t nbytes){
    // allocates nbytes bytes and returns a pointer to the allocated memory
    // Calculate the number of units needed
    // Always add the size in units of the allocated memory at the beginning, using sizeof (AllocationHeader) bytes for it
    uint16_t units_needed = (nbytes + sizeof(AllocationHeader) + UNIT_SIZE - 1) / UNIT_SIZE;
    MemoryChunkHeader *chunk = NULL;
    int bit_index;
    int is_large_allocation = IS_LARGE_ALLOCATION(units_needed);
    if (first_chunk == NULL) {
        my_malloc_init(); // \nitialize the memory manager
    }
    // large allocation, create a new chunk
    if (is_large_allocation) {
        printf("\n New chunk for large allocation will be created. We need %hd units.\n", units_needed);
        chunk = first_chunk->next = create_new_chunk(units_needed, 1, first_chunk->next); // 1 means TRUE is_large_allocation
        bit_index = STRUCT_UNITS;
    } else {// standard allocation, we might well find a hole in existing chunks
        for (chunk = first_chunk; chunk != NULL; chunk = chunk->next) {
            // Attempt to find the first fit in the bitmap
            printf("Units needed %hd. \n", units_needed);
            printf("Will look in chunk id %hd. \n", chunk->id);
            
            if (units_needed > chunk->chunk_available_units) {// if the chunk is for large allocation it will have 0 available units too
                printf("Not enough space in chunk id %hd that has %hd available units\n", chunk->id, chunk->chunk_available_units);
                continue; // go to next chunk
            }
            bit_index = first_fit(chunk->bitmap, chunk->bitmap_size, units_needed);
            if (bit_index == -1) {
                printf("Not enough space for first fit in chunk id %hd\n", chunk->id);
            } else {// found!
                break;
            }
        }
        if (chunk == NULL) {// have to create a new standard chunk, and will insert it right after the first chunk
            printf("\nNew chunk for standard allocation will be created. We need %hd units. \n", units_needed);
            chunk = first_chunk->next = create_new_chunk (units_needed, 0, first_chunk->next); // 0 means FALSE is_large_allocation
            bit_index = first_fit(chunk->bitmap, chunk->bitmap_size, units_needed);
            if (bit_index == -1) { // should never happen
                printf("Not enough space for first fit in chunk id %hd\n", chunk->id);
                // error (EXIT_FAILURE, 0, "bit index return -1 on new chunk, programming error. Exiting.");
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
my_free(void *ptr){
    // frees the memory space pointed to by ptr, which must have been returned by a previous call to my_malloc
    // if ptr is NULL, no operation is performed
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