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
    MemoryChunkHeader* header = (MemoryChunkHeader*) create_new_chunk(UNITS_PER_CHUNK, 0, NULL);
    for (int i = 0; i < BITMAP_SIZE; i++) {
        header->bitmap[i] = 0;
    }

    printf("Bitmap Size: %d bytes\n", BITMAP_SIZE);

    while (1) {
        int choice;
        if (!get_user_input("Enter your choice (1 to set, 2 to clear, 3 to allocate, 4 to free, or 0 to exit): ", &choice)) {
            continue;
        }

        if (choice == 0) {
            break;
        }
        int start_byte_index;
        int start_bit_index;
        int qty;

        if (choice == 1) {
            if (!get_user_input("Enter the number of bits to set: ", &qty) || 
                !get_user_input("Enter in which byte start to set: ", &start_byte_index) || 
                !get_user_input("Enter the position of the bit to start set: ", &start_bit_index)) {
                continue;
            }
            set_or_clear_bits(1, header->bitmap, start_byte_index, start_bit_index, qty);
        } else if (choice == 2) {
            if (!get_user_input("Enter the number of bits to clear: ", &qty) || 
                !get_user_input("Enter in which byte start to clear: ", &start_byte_index) || 
                !get_user_input("Enter the position of the bit to start clear: ", &start_bit_index)) {
                continue;
            }
            set_or_clear_bits(0, header->bitmap, start_byte_index, start_bit_index, qty);
        } 
        printf("Updated Bitmap: ");
        print_bitmap(header->bitmap, BITMAP_SIZE);
    }

    return 0;
}
