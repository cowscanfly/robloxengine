#include "Instances/Instance.hpp"

#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <new>
#include "MemorySystem.hpp"
#include "PropertySignalNode.hpp"
#include "types/FlatMap.hpp"
#include "types/String.hpp"
#include "Instances/InstanceUniqueId.hpp"

namespace Engine {

Instance::Instance()
	: m_name("Instance"), m_unique_id(generateUniqueId()), m_parent(nullptr) {}

Instance::~Instance() {
	Destroying.Fire(this);
	
	if (m_parent) {
		Instance* temp_parent = m_parent;
		m_parent = nullptr; 
		temp_parent->RemoveChild(this);
	}

	auto it = m_children.GetIterator();
	while (it.step()) {
		it.value.value->m_parent = nullptr;
	}

	size_t childCount = 0;
	Instance** childArray = new Instance*[m_children.GetSize()]; 

	auto it2 = m_children.GetIterator();
	while (it2.step()) {
		childArray[childCount++] = it2.value.value;
	}

	for (size_t i = 0; i < childCount; ++i) {
		delete childArray[i];
	}
	delete[] childArray;

	PropertySignalNode* current = m_propertySignalsHead;
	while (current != nullptr) {
		PropertySignalNode* nextNode = current->next;
		current->signal.~Signal();
		Engine::Memory::GetPropertySignalNodeAllocator().deallocate(current);
		current = nextNode;
	}

	PropertySignalNode* internalCurrent = m_Internal_propertySignalsHead;
	while (internalCurrent != nullptr) {
		PropertySignalNode* nextNode = internalCurrent->next;
		internalCurrent->signal.~Signal();
		Engine::Memory::GetPropertySignalNodeAllocator().deallocate(internalCurrent);
		internalCurrent = nextNode;
	}
}

void Instance::SetParent(Instance* new_parent) {
	if (m_parent == new_parent) return;

	if (m_parent) {
		m_parent->RemoveChild(this);
	}

	m_parent = new_parent;

	if (m_parent) {
		m_parent->AddChild(this);
	}

	FirePropertyChangedSignal("Parent");
}

void Instance::AddChild(Instance* child) {
	m_children.Insert(child->m_unique_id, child);

	ChildAdded.Fire(child);

	for (Instance* ascendant = child->m_parent; ascendant != nullptr; ascendant = ascendant->m_parent) {
		ascendant->CascadeDescendantAdded(child);
	}
}

void Instance::RemoveChild(Instance* child) {
	m_children.Erase(child->m_unique_id);

	ChildRemoved.Fire(child);

	for (Instance* ascendant = child->m_parent; ascendant != nullptr; ascendant = ascendant->m_parent) {
		ascendant->CascadeDescendantRemoving(child);
	}
}

Instance* Instance::FindFirstChild(const char* target_name) {
	auto it = m_children.GetIterator();
	while (it.step()) {
		if (it.value.value->m_name == target_name) {
			return it.value.value;
		}
	}
	return nullptr;
}

// for IsA

typedef const void* ClassId;

ClassId Instance::GetClassIdStatic() {
	static const char id = 0; 
	return &id;
}

ClassId Instance::GetClassId() const {
	return GetClassIdStatic();
}

bool Instance::IsA(ClassId targetId) const {
	return GetClassId() == targetId;
}

Signal* Instance::GetPropertyChangedSignal(const char* propertyName) {
	PropertySignalNode* current = m_propertySignalsHead;
	while (current != nullptr) {
		if (strcmp(current->propertyName, propertyName) == 0) {
			return &(current->signal);
		}
		current = current->next;
	}

	PropertySignalNode* newNode = (PropertySignalNode*)Engine::Memory::GetPropertySignalNodeAllocator().allocate();
	new (&newNode->signal) Signal();
	newNode->propertyName = propertyName;
	
	newNode->next = m_propertySignalsHead;
	m_propertySignalsHead = newNode;

	return &(newNode->signal);
}

void Instance::FirePropertyChangedSignal(const char* propertyName) {

	Internal_FirePropertyChangedSignal(propertyName);

	PropertySignalNode* current = m_propertySignalsHead;
	while (current != nullptr) {
		if (strcmp(current->propertyName, propertyName) == 0) {
			current->signal.Fire(nullptr); 
			return;
		}
		current = current->next;
	}
}

void Instance::SetName(const char* new_name) {
	m_name = new_name; 
	FirePropertyChangedSignal("Name");
}

const char* Instance::GetClassName() const { return "Instance"; }
const char* Instance::GetName() const { return m_name.CStr(); }
uint64_t Instance::GetUniqueID() const { return m_unique_id; }
Instance* Instance::GetParent() const { return m_parent; }
const FlatMap<uint64_t, Instance*>& Instance::GetChildren() { return m_children; }

// Recursively walks DOWN a target subtree, triggering the calling ancestor's event at every node
void Instance::CascadeDescendantAdded(Instance* sub_target) {
	DescendantAdded.Fire(sub_target);

	auto it = sub_target->m_children.GetIterator();
	while (it.step()) {
		CascadeDescendantAdded(it.value.value);
	}
}

void Instance::CascadeDescendantRemoving(Instance* sub_target) {
	DescendantRemoving.Fire(sub_target);

	auto it = sub_target->m_children.GetIterator();
	while (it.step()) {
		CascadeDescendantRemoving(it.value.value);
	}
}

// Internal variants pass 'this' as an argument to safely decouple C++ backend systems (like the Renderer) from public 0-argument game scripts.

Signal* Instance::Internal_GetPropertyChangedSignal(const char* propertyName) {
	PropertySignalNode* current = m_Internal_propertySignalsHead;
	while (current != nullptr) {
		if (strcmp(current->propertyName, propertyName) == 0) {
			return &(current->signal);
		}
		current = current->next;
	}

	PropertySignalNode* newNode = (PropertySignalNode*)Engine::Memory::GetPropertySignalNodeAllocator().allocate();
	new (&newNode->signal) Signal();
	newNode->propertyName = propertyName;
	
	newNode->next = m_Internal_propertySignalsHead;
	m_Internal_propertySignalsHead = newNode;

	return &(newNode->signal);
}

void Instance::Internal_FirePropertyChangedSignal(const char* propertyName) {
	PropertySignalNode* current = m_Internal_propertySignalsHead;
	while (current != nullptr) {
		if (strcmp(current->propertyName, propertyName) == 0) {
			current->signal.Fire(this); 
			return;
		}
		current = current->next;
	}
}

} // namespace Engine
