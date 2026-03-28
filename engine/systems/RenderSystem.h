#pragma once
#include "ecs/ECS.h"
#include "renderer/OpenGLRenderer/OpenGLMaterial.h"
#include "components/Material.h"
#include "components/Mesh.h"

class RenderSystem : public System {
  public:
	GPUMaterial *gpuMaterial;

	void init() override {

		gpuMaterial = new OpenGLMaterial();

		for (auto &entity : this->manager->entities) {
			Material *mat = entity->getComponent<Material>();
			Mesh *mesh = entity->getComponent<Mesh>();
			if (!mat || mat->materials.empty() || !mesh)
				continue;
			gpuMaterial->init(entity->id, mat->materials[0], mesh->asset);

			for (auto &shader : mat->materials[0].passes) {
				gpuMaterial->addShader(entity->id, shader);
			}
		}
	}

	void update() override {
		for (auto &entity : this->manager->entities) {
			gpuMaterial->update(entity->id);
		}
		for (auto &entity : this->manager->entities) {
			gpuMaterial->draw(entity->id);
		}
	}
};