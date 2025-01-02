# Custom Memory Allocator

This project implements a simple custom memory allocator in C, demonstrating concepts of memory management such as block allocation, freeing memory, and handling fragmentation.
## Features

- Memory arena initialization (`init_arena`).
- Memory block allocation with handling of fragmentation (`arena_alloc`).
- Freeing memory and merging adjacent free blocks (`arena_free`).
- Validation of memory blocks using checksums and magic numbers.
- Debugging output to track memory allocation and deallocation.

 

## Usage

```c
#include "memory_allocator.h"

int main() {
    // Initialize 1MB memory arena
    memory_arena_t* arena = init_arena(1024 * 1024);
    
    // Allocate memory
    void* ptr = arena_alloc(arena, size);
    
    // Free memory
    arena_free(arena, ptr);
    
    // Cleanup
    destroy_arena(arena);
}
```

## Memory Block Structure

```c
typedef struct block_header {
    size_t size;            // Block size including header
    bool is_free;           // Block status
    struct block_header* next;     // Next block in list
    struct block_header* prev;     // Previous block in list
    uint32_t magic;         // Validation number
    uint32_t checksum;      // Corruption detection
} block_header_t;
```


## Advanced Usage

### Memory Statistics
```c
printf("Used Memory: %zu bytes\n", arena->used_memory);
printf("Peak Memory: %zu bytes\n", arena->peak_memory);
```

### Custom Block Size
```c
// Minimum allocation size
arena->min_alloc = custom_size;
```

## Contributing

1. Fork the repository
2. Create feature branch
3. Commit changes
4. Push to branch
5. Create Pull Request

## License

MIT License
