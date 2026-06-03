#pragma once

#include <stdint.h>
#include <stddef.h>
#include "Iterator.hpp"

constexpr float max_load_factor = 0.7f;
constexpr size_t default_inital_capacity = 8;

template <typename K, typename V>
struct MapEntry {
	K key;
	V value;
};

template <typename KeyType, typename ValueType>
class FlatMap {
	private:
		enum State { EMPTY, OCCUPIED, DELETED };

		struct Entry {
			KeyType key;
			ValueType value;
			State state = EMPTY;
		};

		Entry* table;
		size_t capacity;
		size_t size;

		inline uint32_t CalculateHash(const KeyType& key) {
			const uint8_t* bytes = (const uint8_t*)(&key);
			uint32_t length = sizeof(KeyType);
			
			uint32_t hash = 5381;

			for (uint32_t i = 0; i < length; ++i)
			{
				hash = ((hash << 5) + hash) + bytes[i];
			}

			hash ^= (hash >> 16);
			
			return hash;
		}

		inline uint32_t Hash(const KeyType& key) {
			return CalculateHash(key) % capacity;
		}
		
		inline size_t NextIndex(size_t current_index) {
			return (current_index + 1) % capacity;
		}

		bool InsertInternal(const KeyType& key, const ValueType& value) {
			size_t index = Hash(key);
			size_t first_deleted_index = -1;

			while (1) {
				Entry& entry = table[index];

				if (entry.state == EMPTY) {
					size_t target_index = (first_deleted_index != -1) ? first_deleted_index : index;
					table[target_index].key = key;
					table[target_index].value = value;
					table[target_index].state = OCCUPIED;
					size++;
					return true;
				}
				if (entry.state == OCCUPIED && entry.key == key) {
					entry.value = value;
					return false;
				}
				if (entry.state == DELETED && first_deleted_index == -1) {
					first_deleted_index = index;
				}

				index = NextIndex(index);
			}
		}
		
	public:
		FlatMap(size_t inital_capacity = default_inital_capacity) {
			capacity = (inital_capacity == 0) ? default_inital_capacity : inital_capacity;
			size = 0;
			table = new Entry[capacity];
			for (size_t i = 0; i < capacity; i++) {
				table[i].state = EMPTY;
			}
		}

		~FlatMap() {
			delete[] table;
		}

		FlatMap(const FlatMap&) = delete;
		FlatMap& operator=(const FlatMap&) = delete;

		size_t GetSize() const { return size; }
		size_t GetCapacity() const { return capacity; }

		bool Insert(const KeyType& key, const ValueType& value) {
			if (capacity == 0 || (float)size / (float)capacity >= max_load_factor) {
				size_t next_capacity = (capacity == 0) ? default_inital_capacity : capacity * 2;
				if (next_capacity < capacity) {
					return false;
				}
				Resize(next_capacity);
			}
			return InsertInternal(key, value);
		}

		ValueType* Find(const KeyType& key) {
			size_t index = Hash(key);
			size_t start_index = index;

			while (1) {
				Entry& entry = table[index];

				if (entry.state == EMPTY) {
					return nullptr;
				}

				if (entry.state == OCCUPIED && entry.key == key) {
					return &entry.value;
				}

				index = NextIndex(index);

				if (index == start_index) {
					return nullptr;
				}
			}
		}

		bool Erase(const KeyType& key) {
			size_t index = Hash(key);
			size_t start_index = index;

			while (1) {
				Entry& entry = table[index];

				if (entry.state == EMPTY) {
					return false;
				}

				if (entry.state == OCCUPIED && entry.key == key) {
					entry.state = DELETED;
					size--;
					return true;
				}

				index = NextIndex(index);

				if (index == start_index) {
					return false;
				}
			}
		}

		void Resize(size_t new_capacity) {
			if (new_capacity == 0 || new_capacity == (size_t)-1) {
				new_capacity = default_inital_capacity;
			}

			size_t max_allowed_capacity = 9223372036854775807ULL / sizeof(Entry);
			if (new_capacity > max_allowed_capacity) {
				new_capacity = max_allowed_capacity;
			}

			if (new_capacity <= capacity) {
				return;
			}

			Entry* old_table = table;
			size_t old_capacity = capacity;

			capacity = new_capacity;
			size = 0;
			table = new Entry[new_capacity];

			for (size_t i = 0; i < new_capacity; i++) {
				table[i].state = EMPTY;
			}

			for (size_t i = 0; i < old_capacity; i++) {
				Entry& old_entry = old_table[i];
				if (old_entry.state == OCCUPIED) {
					InsertInternal(old_entry.key, old_entry.value);
				}
			}
			delete[] old_table;
		}

		static bool NextItem(void* state, void** current, MapEntry<KeyType, ValueType>& out_value) {
			const FlatMap* map = static_cast<const FlatMap*>(state);
			
			size_t index = (size_t)(*current);

			while (index < map->capacity) {
				Entry& entry = map->table[index];
				if (entry.state == OCCUPIED) {
					out_value = MapEntry<KeyType, ValueType>{ entry.key, entry.value };
					*current = (void*)(index + 1);
					return true;
				}
				index++;
			}

			return false;
		}

		Iterator<MapEntry<KeyType, ValueType>> GetIterator() const {
			return Iterator<MapEntry<KeyType, ValueType>>{ 
				NextItem, 
				const_cast<FlatMap*>(this), 
				(void*)0, 
				MapEntry<KeyType, ValueType>{ table[0].key, table[0].value } 
			};
		}
};
