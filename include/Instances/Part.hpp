#pragma once

#include "../enums/PartType.hpp"
#include "BasePart.hpp"
#include "InstanceMacros.hpp"

namespace Engine {

	class Part : public BasePart {
	protected:
		Enum::PartType m_shape;
	public:
		DECLARE_CLASS_ID()

		Part();

		virtual const char* GetClassName() const override;

		Enum::PartType GetShape() const;
		void SetShape(Enum::PartType shape);
	};

} // namespace Engine
