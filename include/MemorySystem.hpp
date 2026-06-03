#pragma once
#include "Signal.hpp"
#include "PropertySignalNode.hpp"
#include "types/PoolAllocator.hpp"

namespace Engine {
namespace Memory {

inline Signal::Allocator g_signalAllocator;
inline PoolAllocator<PropertySignalNode> g_propertySignalNodeAllocator;

} // namespace Memory
} // namespace Engine
