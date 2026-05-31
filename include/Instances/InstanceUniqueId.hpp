#pragma once
#include <stdint.h>
static uint64_t uniqueidcounter = 0;

static uint64_t generateUniqueId() {
	return uniqueidcounter++;
}
