#pragma once
#include "types/PoolAllocator.hpp"

namespace Engine {

// The static callback function pointer layout
typedef void (*EventCallback)(void* instance, void* payload);

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

	// We pass a generic payload pointer along when firing the event
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
