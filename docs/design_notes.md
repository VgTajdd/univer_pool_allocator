# Design notes

## Names

- `MemoryPoolAllocator` (`std::allocator` equivalent) -> `templated class <typename T>` -> for vector.
- `MemoryPoolContainer` (contains chunks) -> `templated static class <typename T or size_t ObjectSize, size_t ChunkCapacity>` -> is used by allocators like `MemoryPoolAllocator` or `std::allocator`.
- `MemoryPoolChunk` -> allocates memory using single list approach.

generic pool allocator

## Steps

- [x] Research if there are any other curated custom allocator libs on github (is it possible to use any of them in univer?).
- [x] Push the pool memory allocator with the single allocation specification (it's possible to create un fallback allocator with the default allocator for sizes greater than 1).
- [x] Implement tests for the actual version.
- [x] Transform to static library.

## Future work

- [ ] Remove the "next" from the chunk (?) -> no, only remove it from allocation logic, that means: for allocation/deallocation purposes, if there are 2 chunks that's like them we a single block. So, the next wil be used if we need a reference for profiling and to delete all the chunks.
- [ ] Generalize for different sizes.

## Notes

```cpp
0 1
1 2
2 4
3 8
4 16
5 32
6 64
7 128
```
