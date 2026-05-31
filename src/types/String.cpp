#include "types/String.hpp"

namespace Engine {

String::String() {
	m_data = (char*)malloc(1);
	m_data[0] = '\0';
	m_length = 0;
}

String::String(const char* str) {
	if (str) {
		m_length = strlen(str);
		m_data = (char*)malloc(m_length + 1);
		assert(m_data != nullptr && "Out of memory!");
		memcpy(m_data, str, m_length + 1);
	} else {
		m_data = (char*)malloc(1);
		m_data[0] = '\0';
		m_length = 0;
	}
}

String::~String() {
	if (m_data) {
		free(m_data);
	}
}

String::String(const String& other) {
	m_length = other.m_length;
	m_data = (char*)malloc(m_length + 1);
	assert(m_data != nullptr && "Out of memory!");
	memcpy(m_data, other.m_data, m_length + 1);
}

String& String::operator=(const String& other) {
	if (this != &other) {
		char* new_data = (char*)realloc(m_data, other.m_length + 1);
		assert(new_data != nullptr && "Out of memory!");
		
		m_data = new_data;
		m_length = other.m_length;
		memcpy(m_data, other.m_data, m_length + 1);
	}
	return *this;
}

void String::Append(const char* str) {
	if (!str) return;
	
	size_t append_len = strlen(str);
	if (append_len == 0) return;

	size_t new_length = m_length + append_len;
	char* new_data = (char*)realloc(m_data, new_length + 1);
	assert(new_data != nullptr && "Out of memory!");

	m_data = new_data;
	memcpy(m_data + m_length, str, append_len + 1); 
	m_length = new_length;
}

bool String::operator==(const char* str) const {
	if (!str) return false;
	return strcmp(m_data, str) == 0;
}

bool String::operator==(const String& other) const {
	return strcmp(m_data, other.m_data) == 0;
}

const char* String::CStr() const { return m_data; }
size_t String::Length() const { return m_length; }

} // namespace Engine
