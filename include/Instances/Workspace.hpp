#pragma once

#include "Instances/Instance.hpp"
#include "Instances/InstanceMacros.hpp"

namespace Engine {

class Workspace : public Instance {
	protected:
		float FallenPartsDestroyHeight;
	public:
		DECLARE_CLASS_ID()
		Workspace(); 

		virtual const char* GetClassName() const override;
		float GetFallenPartsDestroyHeight() const; 
	};

} // namespace Engine
