#pragma once
#include "ecs/ECS.h"

// =====================================================================================================
// Structs
// =====================================================================================================

// Texture struct later needs to be flushed out.
struct MTexture {
	unsigned int ID;
	const char *filePath;
	float transparency;
};

struct TextureArray {
	unsigned int ID;
	int currentTexLocation = 0;
	std::vector<MTexture> textures;
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
		text.filePath = filePath;
		texture_items.emplace_back(text);
	}
};
