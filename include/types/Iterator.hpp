#pragma once

template <typename T>
struct Iterator {
	bool (*next)(void* state, void** current, T& out_value);
	void* state;
	void* current;
	T value;

	bool step() {
		return next(state, &current, value);
	}
};
