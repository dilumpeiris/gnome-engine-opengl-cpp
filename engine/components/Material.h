#pragma once
#include "ecs/ECS.h"

// =====================================================================================================
// Structs
// =====================================================================================================

// Texture struct later needs to be flushed out.
struct Texture {
	unsigned int ID;
	const char *filePath;
	const char *name;
	float transparency;
};

// =====================================================================================================
// Material Component
// =====================================================================================================
class Material : public Component {

  public:
	std::vector<Texture> texture_items;
	int currentTextureLocation = 0;

  public:
	Material() {}

	void addTexture(const char *filePath, const char *name) {
		Texture text;
		text.filePath = filePath;
		text.name = name;
		text.transparency = 1.0f;

		texture_items.emplace_back(text);
	}

	int getTextureArrayLength() { return texture_items.size(); }

	int getCurrentTextureLocation() { return currentTextureLocation; }

	const char *getCurrentTextureName() { return texture_items[currentTextureLocation].name; }

	void setCurrentTextureLocation(int location) { currentTextureLocation = location; }

	void useTexture(const char *name) {
		for (auto &img : texture_items) {
			if (std::string(img.name) == name) {
				currentTextureLocation = &img - &texture_items[0];
				break;
			}
		}
	}
};
