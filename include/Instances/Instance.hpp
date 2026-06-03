#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include "types/String.hpp"
#include "Signal.hpp"
#include "PropertySignalNode.hpp"
#include "types/FlatMap.hpp"
#include "MemorySystem.hpp"

namespace Engine {

class Instance {
protected:
	String    m_name;
	uint64_t  m_unique_id;
	
	Instance* m_parent;
	FlatMap<uint64_t, Instance*> m_children;

	PropertySignalNode* m_propertySignalsHead = nullptr;
	PropertySignalNode* m_Internal_propertySignalsHead = nullptr;
	void FirePropertyChangedSignal(const char* propertyName);

	void RemoveChild(Instance* child); 
	void AddChild(Instance* child); 
public:
	Signal ChildAdded;
	Signal ChildRemoved;
	Signal DescendantAdded;
	Signal DescendantRemoving;
	Signal Destroying;
	Signal* GetPropertyChangedSignal(const char* propertyName); 

	// Internal variants pass 'this' as an argument to safely decouple C++ backend systems (like the Renderer) from public 0-argument game scripts.
	Signal* Internal_GetPropertyChangedSignal(const char* propertyName);

	Instance();
	virtual ~Instance(); 
	void SetParent(Instance* new_parent); 
	Instance* FindFirstChild(const char* target_name);

	// for IsA

	typedef const void* ClassId;
	static ClassId GetClassIdStatic(); 
	virtual ClassId GetClassId() const; 
	virtual bool IsA(ClassId targetId) const; 

	virtual const char* GetClassName() const;
	const char* GetName() const;
	void SetName(const char* new_name);
	uint64_t GetUniqueID() const;
	Instance* GetParent() const;
	const FlatMap<uint64_t, Instance*>& GetChildren();
private:
	void Internal_FirePropertyChangedSignal(const char* propertyName);
	void CascadeDescendantAdded(Instance* child);
	void CascadeDescendantRemoving(Instance* child);
};
} // namespace Engine
