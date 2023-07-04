# Design notes

## Names

- `BasicAllocator` (`std::allocator` equivalent) -> `templated class <typename T>` -> for vector.
- `MemoryPoolContainer` (contains chunks) -> `templated static class <typename T or size_t ObjectSize, size_t ChunkCapacity>` -> is used by allocators like `BasicAllocator` or `std::allocator`.
- `MemoryPoolChunk` -> allocates memory using single list approach.

generic pool allocator

## Steps

- [x] buscar custom allocator lib (se puede usar para univer?)
- [x] pushear el pool memory allocator con restricción acerca del single allocation (se puede crear un fallback allocator con el default allocator para tamaños mayores a 1)
- [ ] crear tests para esa versión actual
- [ ] remover el next del chunk (?) no solo sacarlo del mecanismo del allocate, es decir para fines de alloc/dealloc si hay 2 chunks es como si fuera todo continuo, pero el next sirve para tener una referencia por si se quiere hacer un profile y para el delete de todos los chunks
- [ ] generalizar para distintos tamaños.

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
