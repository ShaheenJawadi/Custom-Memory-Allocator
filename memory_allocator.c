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
static block_header_t* find_free_block(memory_arena_t* arena, size_t size) {
    block_header_t* current = arena->first_block;
    while (current) {
        if (current->is_free && current->size >= size) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

static void validate_block(block_header_t* block) {
    assert(block->magic == MAGIC_NUMBER);
    assert(block->checksum == calculate_checksum(block));
}


memory_arena_t* init_arena(size_t size) {
    size = ALIGN(size);
// Allocate memory for arena control structure
    memory_arena_t* arena = malloc(sizeof(memory_arena_t));
    if (!arena) return NULL;
    // Allocate memory pool
    arena->start = malloc(size);
    if (!arena->start) {
        free(arena);
        return NULL;
    }

    arena->size = size;
    arena->min_alloc = MIN_BLOCK_SIZE;
    arena->used_memory = 0;
    arena->peak_memory = 0;
    // Initialize first block
    arena->first_block = (block_header_t*)arena->start;
    arena->first_block->size = size;
    arena->first_block->is_free = true;
    arena->first_block->next = NULL;
    arena->first_block->prev = NULL;
    arena->first_block->magic = MAGIC_NUMBER;
    arena->first_block->checksum = calculate_checksum(arena->first_block);

    return arena;
}

void* arena_alloc(memory_arena_t* arena, size_t size) {
    if (size == 0) return NULL;
    // Align size and add header size
    size = ALIGN(size + sizeof(block_header_t));
    // Find a free block
    block_header_t* block = find_free_block(arena, size);
    if (!block) return NULL;

    if (block->size >= size + MIN_BLOCK_SIZE + ALIGNMENT) {
        block_header_t* new_block = (block_header_t*)((char*)block + size);
        new_block->size = block->size - size;
        new_block->is_free = true;
        new_block->magic = MAGIC_NUMBER;
        new_block->next = block->next;
        new_block->prev = block;
        if (block->next) {
            block->next->prev = new_block;
        }
        block->next = new_block;
        block->size = size;
        new_block->checksum = calculate_checksum(new_block);
    }
    // Update block metadata
    block->is_free = false;
    block->checksum = calculate_checksum(block);
    // Update arena statistics
    arena->used_memory += block->size;
    if (arena->used_memory > arena->peak_memory) {
        arena->peak_memory = arena->used_memory;
    }

    // Return pointer to usable memory (after header)
    return (void*)((char*)block + sizeof(block_header_t));
}

void arena_free(memory_arena_t* arena, void* ptr) {
    if (!ptr) return;
    // Get block header
    block_header_t* block = (block_header_t*)((char*)ptr - sizeof(block_header_t));
    // Validate block
    validate_block(block);
    // Update arena statistics
    arena->used_memory -= block->size;
    // Update arena statistics
    block->is_free = true;
    // Attempt to merge with next block if it's free
    if (block->next && block->next->is_free) {
        block->size += block->next->size;
        block->next = block->next->next;
        if (block->next) {
            block->next->prev = block;
        }
    }

    // Attempt to merge with previous block if it's free
    if (block->prev && block->prev->is_free) {
        block->prev->size += block->size;
        block->prev->next = block->next;
        if (block->next) {
            block->next->prev = block->prev;
        }
        block = block->prev;
    }
    // Update checksum after modifications
    block->checksum = calculate_checksum(block);
}



void destroy_arena(memory_arena_t* arena) {
    if (!arena) return;
    free(arena->start);
    free(arena);
}
