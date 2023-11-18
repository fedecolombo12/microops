# Makefile for MicroOps

# Variable for the C compiler being used
CC = gcc

# Variable for compiler flags
CFLAGS = -Wall -Wextra -g

# Source files (.c files)
CFILES = main.c my_malloc_manager.c set_or_clear_bits.c print_bitmap.c create_new_chunk.c first_fit.c
# Header files (.h files)
HFILES = my_malloc_manager.h

# Object files (.o files)
OBJFILES = $(CFILES:.c=.o)

# Output executable name
OUT = MO

# Main target
$(OUT): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(OUT) $(OBJFILES)

# Rule for generating object files
%.o: %.c $(HFILES)
	$(CC) $(CFLAGS) -c $< -o $@

# Phony target to remove all build files
.PHONY: clean
clean:
	rm -f $(OBJFILES) $(OUT)