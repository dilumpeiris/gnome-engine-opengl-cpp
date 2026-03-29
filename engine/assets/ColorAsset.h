#pragma once

#include <glm/glm.hpp>

struct ColorAsset {
	glm::vec4 color;
	ColorAsset() { color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); }
	ColorAsset(glm::vec4 color) : color(color) {}
	ColorAsset(float r, float g, float b, float a) : color(r, g, b, a) {}
};