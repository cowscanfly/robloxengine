#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
constexpr size_t poolSize = 1048576; // 1 MB

namespace Engine {

class BumpAllocator {
public:
	BumpAllocator(); 
	~BumpAllocator();

	void* Allocate(const size_t size, const size_t alignment);

	template<typename T, typename... Args>
	T* New(const Args&... args); 

	void Reset(); 
private:
	void* pool;
	void* pool_end;
	void* bump_ptr;
};

} // namespace Engine
