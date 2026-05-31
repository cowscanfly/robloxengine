#include "Instances/BasePart.hpp"


#include "Instances/Instance.hpp"
#include "Instances/InstanceMacros.hpp"
#include "types/Vector3.hpp"
#include "types/Color3.hpp"
#include "types/CFrame.hpp"

namespace Engine {

DEFINE_CLASS_ID(BasePart, Instance)

// initalization & destructors

BasePart::BasePart() : Instance()
{
	m_name     = "BasePart";
	cframe     = CFrame();
	m_size     = { 1.0f, 1.0f, 1.0f };
	m_color    = Color3();
}

void BasePart::SetPosition(Vector3 pos) { 
	Vector3 old_position = cframe.Position();
	if (pos == old_position) return;
	cframe = CFrame(pos) * cframe.Rotation(); 
	FirePropertyChangedSignal("CFrame");
	FirePropertyChangedSignal("Position");
}
void BasePart::SetCFrame(CFrame _cframe) {
	Vector3 old_position = cframe.Position();
	cframe = _cframe; 
	FirePropertyChangedSignal("CFrame");
	if (cframe.Position() != old_position) {
		FirePropertyChangedSignal("Position");
	}
}
void BasePart::SetColor3(Color3 color) { 
	m_color = color; 
	FirePropertyChangedSignal("Color");
}
void BasePart::SetSize(Vector3 size) {
	m_size = size; 
	FirePropertyChangedSignal("Size");
}

CFrame BasePart::GetCFrame() const { return cframe; }
Color3 BasePart::GetColor3() const { return m_color; }
Vector3 BasePart::GetSize() const { return m_size; }
Vector3 BasePart::GetPosition() const { return cframe.Position(); }

const char* BasePart::GetClassName() const { return "BasePart"; }

} // namespace Engine

