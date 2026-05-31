#include "Instances/Part.hpp"

#include "enums/PartType.hpp"
#include "Instances/BasePart.hpp"
#include "Instances/InstanceMacros.hpp"
namespace Engine {

DEFINE_CLASS_ID(Part, BasePart)

Part::Part()
	: BasePart()
	, m_shape(Enum::PartType::Block)
{
	m_name = "Part";
}

const char* Part::GetClassName() const { return "Part"; }

Enum::PartType Part::GetShape() const { return m_shape; }
void Part::SetShape(Enum::PartType shape) { 
	m_shape = shape; 
	FirePropertyChangedSignal("Shape");
}

} // namespace Engine

