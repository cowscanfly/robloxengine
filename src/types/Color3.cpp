#include "types/Color3.hpp"

namespace Engine {

Color3::Color3() : R(0.0f), G(0.0f), B(0.0f) {}
Color3::Color3(float _R, float _G, float _B) : R(_R), G(_G), B(_B) {}

Color3 Color3::fromRGB(int R, int G, int B) {
	return Color3( (float)R / 255, (float)G / 255, (float)B / 255 );
}

} // namespace Engine
