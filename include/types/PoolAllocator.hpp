#pragma once

#include <stddef.h>
#include <new>

template <typename T>
class PoolAllocator {
private:
	union Node {
		Node* next;
		char storage[sizeof(T)];
	};

	struct Chunk {
		void* storage;
		Chunk* next;
	};

	size_t m_blockCount;
	size_t m_blockSize;
	Chunk* m_chunks;
	Node* m_freeList;

	void allocateChunk(size_t blockCount) {
		size_t perBlock = sizeof(T) > sizeof(Node) ? sizeof(T) : sizeof(Node);
		size_t alignment = alignof(T) > alignof(Node) ? alignof(T) : alignof(Node);
		
		size_t remainder = perBlock % alignment;
		m_blockSize = (remainder == 0) ? perBlock : (perBlock + alignment - remainder);
		
		size_t totalAllocationSize = m_blockSize * blockCount + alignment;
		void* rawStorage = ::operator new(totalAllocationSize);

		Chunk* newChunk = reinterpret_cast<Chunk*>(::operator new(sizeof(Chunk)));
		newChunk->storage = rawStorage;
		newChunk->next = m_chunks;
		m_chunks = newChunk;

		size_t rawAddress = reinterpret_cast<size_t>(rawStorage);
		size_t alignedAddress = (rawAddress + alignment - 1) & ~(alignment - 1);
		
		char* current = reinterpret_cast<char*>(alignedAddress);
		for (size_t i = 0; i < blockCount; ++i) {
			Node* node = reinterpret_cast<Node*>(current);
			node->next = m_freeList;
			m_freeList = node;
			current += m_blockSize;
		}
	}

public:
	PoolAllocator(size_t blockCount = 128) :
		m_blockCount(blockCount),
		m_blockSize(0),
		m_chunks(nullptr),
		m_freeList(nullptr)
	{
		allocateChunk(m_blockCount);
	}

	~PoolAllocator() {
		Chunk* current = m_chunks;
		while (current != nullptr) {
			Chunk* next = current->next;
			::operator delete(current->storage);
			::operator delete(current);
			current = next;
		}
	}

	T* allocate() {
		if (m_freeList == nullptr) {
			allocateChunk(m_blockCount);
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
	PoolAllocator(const PoolAllocator&);
	PoolAllocator& operator=(const PoolAllocator&);
};
