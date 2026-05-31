#pragma once

#include <stdlib.h>
#include <string.h>
#include <assert.h>

namespace Engine {

class String {
private:
	char* m_data = nullptr;
	size_t m_length = 0; 

public:
	String();
	String(const char* str);
	~String();
	String(const String& other);
	String& operator=(const String& other);

	void Append(const char* str);

	bool operator==(const char* str) const;
	bool operator==(const String& other) const;

	const char* CStr() const;
	size_t Length() const;
};

} // namespace Engine
