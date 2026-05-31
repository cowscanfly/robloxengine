#pragma once

#include <stdlib.h>
#include <assert.h>

namespace Engine {
template <typename T>
class Vector {
private:
	T* m_data = nullptr;
	size_t m_capacity = 0;
	size_t m_size = 0;

public:
	Vector() = default;

	// FIX: Use delete[] instead of free() to cleanly destroy elements
	~Vector() {
		if (m_data) {
			delete[] m_data;
		}
	}

	// FIX: Use new[] and a copy loop instead of realloc()
	void PushBack(const T& value) {
		if (m_size >= m_capacity) {
			m_capacity = m_capacity == 0 ? 4 : m_capacity * 2;
			
			// Allocate new memory block invoking default constructors
			T* new_data = new T[m_capacity];
			
			// Safely copy old data into the new block
			for (size_t i = 0; i < m_size; i++) {
				new_data[i] = m_data[i];
			}
			
			// Clean up old memory block
			delete[] m_data;
			m_data = new_data;
		}

		m_data[m_size] = value;
		m_size++;
	}

	// FIX: Remove manual destructor invocation since delete[] manages life cycles
	void Pop() {
		assert(m_size > 0 && "Cannot pop from an empty vector!");
		m_size--;
	}

	// FIX: Shift items down using assignment operators safely
	void EraseAt(size_t index) {
		assert(index < m_size && "Index out of bounds!");

		for (size_t i = index; i < m_size - 1; i++) {
			m_data[i] = m_data[i + 1];
		}

		m_size--;
	}

	T& operator[](size_t index) {
		assert(index < m_size && "Index out of bounds!");
		return m_data[index];
	}

	const T& operator[](size_t index) const {
		assert(index < m_size && "Index out of bounds!");
		return m_data[index];
	}

	size_t Size() const { return m_size; }
	size_t Capacity() const { return m_capacity; }
	T* Data() { return m_data; }
};

} // namespace Engine
