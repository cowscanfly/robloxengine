#pragma once

#include "Signal.hpp"

namespace Engine {

struct PropertySignalNode {
	const char* propertyName;
	Signal signal;
	PropertySignalNode* next = nullptr;
};

} // namespace Engine
