#pragma once
#include "Signal.hpp"
#include "PropertySignalNode.hpp"
#include "types/PoolAllocator.hpp"

namespace Engine {
namespace Memory {

namespace {
	inline Signal::Allocator* signalAllocator = nullptr;
	inline PoolAllocator<PropertySignalNode> propertySignalNodeAllocator;
}

inline void Initialize(Signal::Allocator* _signalAllocator) {
	signalAllocator = _signalAllocator;
}

inline Signal::Allocator& GetSignalAllocator() {
	return *signalAllocator;
}

inline PoolAllocator<PropertySignalNode>& GetPropertySignalNodeAllocator() {
	return propertySignalNodeAllocator;
}

} // namespace Memory
} // namespace Engine
