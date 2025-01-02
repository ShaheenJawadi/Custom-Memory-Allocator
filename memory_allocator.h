//
// Created by shaheen on 01/01/2025.
//

#ifndef MEMORY_ALLOCATOR_H
#define MEMORY_ALLOCATOR_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
// Block header structure - keeps track of memory blocks
typedef struct block_header {
    size_t size;            // Size of the block including header
    bool is_free;           // Whether block is free
    struct block_header* next;     // Next block in the list
    struct block_header* prev;     // Previous block in the list
    uint32_t magic;         // Magic number for validation
    uint32_t checksum;      // For memory corruption detection
} block_header_t;
// Memory arena structure
typedef struct {
    void* start;            // Start of memory pool
    size_t size;           // Total size of memory pool
    block_header_t* first_block;  // First block in the list
    size_t min_alloc;      // Minimum allocation size
    size_t used_memory;    // Currently used memory
    size_t peak_memory;    // Peak memory usage
} memory_arena_t;
// Initialize memory arena
memory_arena_t* init_arena(size_t size);

// Allocate memory from arena
void* arena_alloc(memory_arena_t* arena, size_t size);
// Free memory back to arena
void arena_free(memory_arena_t* arena, void* ptr);
// Cleanup arena
void destroy_arena(memory_arena_t* arena);
#endif