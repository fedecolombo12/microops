#include "my_malloc_manager.h"


// Función para obtener la entrada del usuario
int get_user_input(char *prompt, int *input) {
    printf("%s", prompt);
    if(scanf("%d", input) != 1) {
        printf("Invalid input.\n");
        return 0;
    }
    return 1;
}
//inicialeza first_chunk
MemoryChunkHeader *first_chunk = NULL;
uint16_t chunk_current_id = 0;

int main() {
    // Creamos e inicializamos el mapa de bits a cero
    char *ptr = NULL;
    AllocationHeader *header;
    MemoryChunkHeader *chunk;
    while (1) {
        int choice;
        if (!get_user_input("Enter your choice (1 to set, 2 to clear, 0 to exit): ", &choice)) {
            continue;
        }

        if (choice == 0) {
            break;
        }        
        if (choice == 1) {
            size_t nbytes;
            if (!get_user_input("Enter the number of bytes to allocate: ", &nbytes)) {
                continue;
            }
            ptr = my_malloc(nbytes);
            if (ptr == NULL) {
                printf("Allocation failed.\n");
                continue;
            }
            header = (AllocationHeader *)(((char *)ptr) - sizeof(AllocationHeader));
            chunk = (MemoryChunkHeader *)(((char *)header) - header->bit_index * UNIT_SIZE);
            printf("Allocation successful. Address: %p\n", ptr);
            printf("Chunk id: %hd\n", chunk->id);
            printf("Chunk address: %p\n", chunk->address);
            printf("Chunk is_large_allocation: %hd\n", chunk->is_large_allocation);
            printf("Chunk chunk_total_units: %hd\n", chunk->chunk_total_units);
            printf("Chunk chunk_available_units: %hd\n", chunk->chunk_available_units);
            printf("Chunk bitmap: %p\n", chunk->bitmap);
            printf("Chunk bitmap_size: %hd\n", chunk->bitmap_size);
            printf("Chunk next: %p\n", chunk->next);
            printf("Allocation nunits: %hd\n", header->nunits);
            printf("Allocation bit_index: %hd\n", header->bit_index);
            printf("Updated Bitmap: ");
            print_bitmap(chunk->bitmap, chunk->bitmap_size);
            
        } else if (choice == 2) {
            ptr;
                printf("Ingrese un puntero xD : ");
                scanf("%p", ptr);
                my_free(ptr);
                // print_bitmap(chunk->bitmap, chunk->bitmap_size);
            printf("Free successful.\n");
        } else {
            printf("Invalid choice.\n");
            continue;
        }
    }
    return 0;
}
