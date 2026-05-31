#include "BumpAllocator.hpp"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

namespace Engine {

BumpAllocator::BumpAllocator() {
	pool = malloc(poolSize);
	pool_end = reinterpret_cast<char*>(pool) + poolSize;
	bump_ptr = pool;
}

BumpAllocator::~BumpAllocator() {
	free(pool);
}

void* BumpAllocator::Allocate(const size_t size, const size_t alignment) {
	if (alignment == 0 || ((alignment & (alignment - 1)) != 0)) {
		fprintf(stderr, "invalid alignment while allocating in a bump allocator");
		return NULL;
	}
	size_t alignment_mask = alignment - 1;

	uintptr_t raw_address = reinterpret_cast<uintptr_t>(bump_ptr);
	uintptr_t aligned_address = (raw_address + alignment_mask) & ~alignment_mask;
	void* aligned_ptr = reinterpret_cast<void*>(aligned_address);

	void* next_ptr = reinterpret_cast<char*>(aligned_ptr) + size;
	if (next_ptr > pool_end) {
		fprintf(stderr, "bump allocator OOM");
		return NULL;
	}

	bump_ptr = next_ptr;
	return aligned_ptr;
}

template<typename T, typename... Args>
T* BumpAllocator::New(const Args&... args) {
	void* buffer = Allocate(sizeof(T), alignof(T));
	T* newObject = ::new (buffer) T(args...);
	return newObject;
}

void BumpAllocator::Reset() {
	bump_ptr = pool;
}
} // namespace Engine
