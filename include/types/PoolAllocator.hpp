#pragma once

#include <stddef.h>
#include <new>

template <typename T>
class PoolAllocator {
public:
	PoolAllocator(size_t blockCount) :
		m_blockCount(blockCount),
		m_storage(nullptr),
		m_freeList(nullptr)
	{
		size_t perBlock = sizeof(T) > sizeof(Node) ? sizeof(T) : sizeof(Node);
		size_t alignment = alignof(T) > alignof(Node) ? alignof(T) : alignof(Node);
		
		size_t remainder = perBlock % alignment;
		m_blockSize = (remainder == 0) ? perBlock : (perBlock + alignment - remainder);
		
		size_t totalAllocationSize = m_blockSize * m_blockCount + alignment;
		m_storage = ::operator new(totalAllocationSize);
		
		size_t rawAddress = reinterpret_cast<size_t>(m_storage);
		size_t alignedAddress = (rawAddress + alignment - 1) & ~(alignment - 1);
		
		char* current = reinterpret_cast<char*>(alignedAddress);
		for (size_t i = 0; i < m_blockCount; ++i) {
			Node* node = reinterpret_cast<Node*>(current);
			node->next = m_freeList;
			m_freeList = node;
			current += m_blockSize;
		}
	}

	~PoolAllocator() {
		::operator delete(m_storage);
	}

	T* allocate() {
		if (m_freeList == nullptr) {
			return nullptr;
		}
		
		Node* node = m_freeList;
		m_freeList = m_freeList->next;
		
		T* memory = reinterpret_cast<T*>(node);
		return ::new (static_cast<void*>(memory)) T();
	}

	void deallocate(T* ptr) {
		if (ptr == nullptr) {
			return;
		}
		
		ptr->~T();
		
		Node* node = reinterpret_cast<Node*>(ptr);
		node->next = m_freeList;
		m_freeList = node;
	}

private:
	union Node {
		Node* next;
		char storage[sizeof(T)];
	};

	size_t m_blockCount;
	size_t m_blockSize;
	void* m_storage;
	Node* m_freeList;

	PoolAllocator(const PoolAllocator&);
	PoolAllocator& operator=(const PoolAllocator&);
};
