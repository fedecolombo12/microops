#include "my_malloc_manager.h"


// Función para obtener la entrada del usuario
int get_user_input(char *prompt, int *input) {
    printf("%s", prompt);
    if(scanf("%d", input) != 1) {
        printf("Valor inválido. Por favor ingresar un valor correcto.\n");
        return 0;
    }
    return 1;
}
//inicialeza first_chunk
MemoryChunkHeader *first_chunk = NULL;
uint16_t chunk_current_id = 0;

int main() {
    char *ptr = NULL; // Creamos e inicializamos el mapa de bits a cero
    AllocationHeader *header;
    MemoryChunkHeader *chunk;
    while (1) {
        int choice;
        if (!get_user_input("Elegir la opción (1 para set, 2 para clear, 0 para salir): ", &choice)) {
            continue;
        }
        if (choice == 0) {
            break;
        }        
        if (choice == 1) {
            size_t nbytes;
            if (!get_user_input("Ingresar número de bytes que se dese alocar: ", &nbytes)) {
                continue;
            }
            ptr = my_malloc(nbytes);
            if (ptr == NULL) {
                printf("Error en la alocación.\n");
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
            printf("Bitmap: ");
            print_bitmap(chunk->bitmap, chunk->bitmap_size);
        } else if (choice == 2) {
                printf("Ingrese un puntero: ");
                scanf("%p", &ptr);
                my_free(ptr);
                printf("Bitmap: ");
                print_bitmap(chunk->bitmap, chunk->bitmap_size);
            printf("Liberado exitoso.\n");
        } else {
            printf("Elección inválida.\n");
            continue;
        }
    }
    return 0;
}
