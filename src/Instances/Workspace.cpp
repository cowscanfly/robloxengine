#include "Instances/Workspace.hpp"
#include "Instances/Instance.hpp"
#include "Instances/InstanceMacros.hpp"

namespace Engine {

DEFINE_CLASS_ID(Workspace, Instance)

Workspace::Workspace() : Instance() {
	m_name = "Workspace";
}

const char* Workspace::GetClassName() const { return "Workspace"; }

float Workspace::GetFallenPartsDestroyHeight() const { return FallenPartsDestroyHeight; }

} // namespace Engine

