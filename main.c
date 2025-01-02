#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memory_allocator.h"

int main() {
    printf("Custom Memory Allocator Demo!\n");

    size_t arena_size = 1024 * 1024;
    memory_arena_t* arena = init_arena(arena_size);
    if (arena == NULL) {
        fprintf(stderr, "Failed to initialize memory arena.\n");
        return EXIT_FAILURE;
    }
    printf("Memory arena of size %zu bytes initialized successfully.\n", arena_size);
    printf("Allocating memory for an array of 10 integers...\n");
    int* numbers = arena_alloc(arena, sizeof(int) * 10);
    if (numbers == NULL) {
        fprintf(stderr, "Failed to allocate memory.\n");
        destroy_arena(arena);
        return EXIT_FAILURE;
    }
    for (int i = 0; i < 10; i++) {
        numbers[i] = i * i;
    }
    printf("Allocated and initialized array: ");
    for (int i = 0; i < 10; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n");

    printf("Freeing the allocated memory...\n");
    arena_free(arena, numbers);
    printf("Allocating and freeing memory to demonstrate fragmentation...\n");
    void* block1 = arena_alloc(arena, 128);
    void* block2 = arena_alloc(arena, 256);
    void* block3 = arena_alloc(arena, 512);

    arena_free(arena, block2);
    void* block4 = arena_alloc(arena, 200);

    if (block4 != NULL) {
        printf("Successfully allocated memory in a fragmented space.\n");
    } else {
        printf("Failed to allocate memory in the fragmented space.\n");
    }

    destroy_arena(arena);
    printf("Memory arena destroyed. Exiting program.\n");

    return EXIT_SUCCESS;
}
