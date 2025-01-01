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

