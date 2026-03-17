#pragma once
#include "components/Mesh.h"
#include "components/Material.h"
#include "components/Shader.h"
#include "components/Transform.h"
#include "ecs/ECS.h"
#include "renderer/OpenGLRenderer/OpenGLRenderer.h"

class RenderSystem : public System {
  public:
	Renderer *renderer;
	unsigned int VAO;

	void init() override {
		renderer = new OpenGLRenderer();
		renderer->init();

		for (auto &entity : this->manager->entities) {
			Transform *transform = transform = entity->getComponent<Transform>();
			Material *material = material = entity->getComponent<Material>();
			Shader *shader = shader = entity->getComponent<Shader>();
			Mesh *mesh = mesh = entity->getComponent<Mesh>();

			material->setShaderTextures(shader->ID);
			renderer->addEntity(entity->id, mesh->vertices, mesh->indices, mesh->verticesCount,
			                    mesh->indicesCount);
		}
	}

	void update() override {
		for (auto &entity : this->manager->entities) {
			Transform *transform = transform = entity->getComponent<Transform>();
			Material *material = material = entity->getComponent<Material>();
			Shader *shader = shader = entity->getComponent<Shader>();

			shader->use();
			material->bind();
			transform->update();
			renderer->draw(entity->id);
		}
	}
};