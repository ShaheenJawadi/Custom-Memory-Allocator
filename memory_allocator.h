//
// Created by shaheen on 01/01/2025.
//

#ifndef MEMORY_ALLOCATOR_H
#define MEMORY_ALLOCATOR_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct block_header {
    size_t size;
    bool is_free;
    struct block_header* next;
    struct block_header* prev;
    uint32_t magic;
    uint32_t checksum;
} block_header_t;

typedef struct {
    void* start;
    size_t size;
    block_header_t* first_block;
    size_t min_alloc;
    size_t used_memory;
    size_t peak_memory;
} memory_arena_t;
memory_arena_t* init_arena(size_t size);
void* arena_alloc(memory_arena_t* arena, size_t size);
void arena_free(memory_arena_t* arena, void* ptr);
void destroy_arena(memory_arena_t* arena);
#endif