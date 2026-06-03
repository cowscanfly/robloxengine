#pragma once
#include "Signal.hpp"
#include "PropertySignalNode.hpp"
#include "types/PoolAllocator.hpp"

namespace Engine {
class Memory {

private:
	inline static Signal::Allocator* signalAllocator = nullptr;
	inline static PoolAllocator<PropertySignalNode> propertySignalNodeAllocator;

public:
	static void Initialize(Signal::Allocator* _signalAllocator) {
		signalAllocator = _signalAllocator;
	}

	static Signal::Allocator& GetSignalAllocator() {
		return *signalAllocator;
	}

	static PoolAllocator<PropertySignalNode>& GetPropertySignalNodeAllocator() {
		return propertySignalNodeAllocator;
	}

}; 

} // namespace Engine
