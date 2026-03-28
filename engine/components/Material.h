#pragma once
#include "assets/MaterialAsset.h"
#include "ecs/ECS.h"
#include <glm/glm.hpp>
#include "assets/TextureAsset.h"

// =====================================================================================================
// Material Component
// =====================================================================================================
class Material : public Component {

  public:
	std::vector<MaterialAsset> materials;
	int currentMaterialIndex = 0;

  public:
	Material() {}

	void addTexture(const char *filePath, const char *name, int matIndex) {
		TextureAsset text;
		text.filePath = filePath;
		text.name = name;
		materials[matIndex].normal = text;
	}

	int getCurrentMaterialIndex() { return currentMaterialIndex; }

	std::string getCurrentMaterialName() {
		if (materials.empty())
			return "";
		return materials[currentMaterialIndex].name;
	}

	void setCurrentMaterialIndex(int index) { currentMaterialIndex = index; }

	void useMaterial(const char *name) {
		for (auto &mat : materials) {
			if (std::string(mat.name) == name) {
				currentMaterialIndex = &mat - &materials[0];
				break;
			}
		}
	}
};
