#pragma once
#include "ecs/ECS.h"

// Texture struct later needs to be flushed out.
struct MTexture {
	const char *name;
};

// =====================================================================================================
// Material Component
// =====================================================================================================
class Material : public Component {

  public:
	std::vector<MTexture> texture_items;

  public:
	Material() {}

	void addTexture(const char *filePath) {
		MTexture text;
		text.name = filePath;
		texture_items.emplace_back(text);
	}
};
