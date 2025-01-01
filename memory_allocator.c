//
// Created by shaheen on 01/01/2025.
//
#include "memory_allocator.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define ALIGNMENT 8
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~(ALIGNMENT-1))
#define MAGIC_NUMBER 0xDEADBEEF
#define MIN_BLOCK_SIZE (sizeof(block_header_t) + ALIGNMENT)


static uint32_t calculate_checksum(block_header_t* block) {
    uint32_t sum = 0;
    uint8_t* ptr = (uint8_t*)block;
    for (size_t i = 0; i < offsetof(block_header_t, checksum); i++) {
        sum += ptr[i];
    }
    return sum;
}



memory_arena_t* init_arena(size_t size) {
    size = ALIGN(size);

    memory_arena_t* arena = malloc(sizeof(memory_arena_t));
    if (!arena) return NULL;

    arena->start = malloc(size);
    if (!arena->start) {
        free(arena);
        return NULL;
    }

    arena->size = size;
    arena->min_alloc = MIN_BLOCK_SIZE;
    arena->used_memory = 0;
    arena->peak_memory = 0;

    arena->first_block = (block_header_t*)arena->start;
    arena->first_block->size = size;
    arena->first_block->is_free = true;
    arena->first_block->next = NULL;
    arena->first_block->prev = NULL;
    arena->first_block->magic = MAGIC_NUMBER;
    arena->first_block->checksum = calculate_checksum(arena->first_block);

    return arena;
}

