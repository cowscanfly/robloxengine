#pragma once
namespace Engine {
struct Color3 {
	float R = 0.0f;
	float G = 0.0f;
	float B = 0.0f;

	Color3();
	Color3(float _R, float _G, float _B);

	static Color3 fromRGB(int R, int G, int B);
};
} // namespace Engine
