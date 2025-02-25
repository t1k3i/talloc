# talloc

`talloc` is a simple memory allocator implemented in C, designed to mimic `malloc` and `free`. It uses a **first-fit** strategy for memory allocation and manages a fixed-size memory pool of **1024 bytes**.

## Features

- **First-Fit Allocation:** Finds the first available block large enough for the request.
- **Fixed Memory Pool:** Operates within a 1024-byte buffer.
- **Manual Memory Management:** Provides `tmalloc` and `tfree` for allocation and deallocation.

## Functions

### `void* tmalloc(size_t size)`

Allocates `size` bytes of memory from the available pool.

#### Parameters:

- `size`: The number of bytes to allocate.

#### Returns:

- A pointer to the allocated memory if successful.
- `NULL` if there is not enough contiguous space.

### `void tfree(void* ptr)`

Frees a previously allocated block of memory, making it available for future allocations.

#### Parameters:

- `ptr`: A pointer to the memory block to free.
