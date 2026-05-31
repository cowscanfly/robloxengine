#pragma once

#include "Instances/Instance.hpp"
#include "Instances/InstanceMacros.hpp"
#include "types/Vector3.hpp"
#include "types/Color3.hpp"
#include "types/CFrame.hpp"

namespace Engine {

class BasePart : public Instance {
	protected:
		CFrame cframe;
		Vector3 m_size;
		Color3 m_color;
	public:
		DECLARE_CLASS_ID()

		BasePart();

		void SetPosition(Vector3 pos); 
		void SetCFrame(CFrame _cframe);
		void SetColor3(Color3 color);
		void SetSize(Vector3 size);
		CFrame GetCFrame() const;
		Color3 GetColor3() const;
		Vector3 GetSize() const;
		Vector3 GetPosition() const;
		virtual const char* GetClassName() const override;
	};

} // namespace Engine
