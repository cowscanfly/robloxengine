#pragma once
#include "types/PoolAllocator.hpp"

namespace Engine {

typedef void (*EventCallback)(void* instance, void* payload);

// TODO: use a map
class Signal {
private:
	struct Connection {
		void* instance = nullptr;
		EventCallback callback = nullptr;
		Connection* next = nullptr;
	};

	Connection* m_head = nullptr;

public:
	Signal() = default;

	void Connect(PoolAllocator<Connection>& allocator, void* instance, EventCallback callback) {
		Connection* newConn = (Connection*)allocator.allocate();
		newConn->instance = instance;
		newConn->callback = callback;
		newConn->next = m_head;
		m_head = newConn;
	}

	void Disconnect(PoolAllocator<Connection>& allocator, void* instance, EventCallback callback) {
		Connection* current = m_head;
		Connection* previous = nullptr;

		while (current != nullptr) {
			if (current->instance == instance && current->callback == callback) {
				if (previous == nullptr) {
					m_head = current->next;
				} else {
					previous->next = current->next;
				}
				
				allocator.deallocate(current);
				// keep looping to find duplicates
			}
			previous = current;
			current = current->next;
		}
	}

	void Fire(void* payload = nullptr) {
		Connection* current = m_head;
		while (current != nullptr) {
			if (current->callback) {
				current->callback(current->instance, payload);
			}
			current = current->next;
		}
	}
};

} // namespace Engine
