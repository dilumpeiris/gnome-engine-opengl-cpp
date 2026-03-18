#pragma once
#include "components/Mesh.h"
#include "components/Material.h"
#include "components/Shader.h"
#include "components/Transform.h"
#include "ecs/ECS.h"
#include "renderer/OpenGLRenderer/OpenGLMesh.h"
#include "renderer/OpenGLRenderer/OpenGLShader.h"

class RenderSystem : public System {
  public:
	GPUMesh *gpuMesh;
	GPUShader *gpuShader;

	void init() override {
		gpuMesh = new OpenGLMesh();
		gpuMesh->init();
		gpuShader = new OpenGLShader();
		gpuShader->init();

		for (auto &entity : this->manager->entities) {
			Transform *transform = transform = entity->getComponent<Transform>();
			Material *material = material = entity->getComponent<Material>();
			Shader *shader = shader = entity->getComponent<Shader>();
			Mesh *mesh = mesh = entity->getComponent<Mesh>();

			gpuMesh->addEntity(entity->id, mesh->vertices, mesh->indices, mesh->verticesCount,
			                   mesh->indicesCount);
			gpuShader->addShader(entity->id, shader->vertexSrc, shader->fragmentSrc);

			// This should not exist after delocating GPU logic in the components.
			material->setShaderTextures(gpuShader->getShaderID(entity->id));
			transform->setShader(gpuShader->getShaderID(entity->id));
		}
	}

	void update() override {
		for (auto &entity : this->manager->entities) {

			Transform *transform = transform = entity->getComponent<Transform>();
			Material *material = material = entity->getComponent<Material>();
			Shader *shader = shader = entity->getComponent<Shader>();

			gpuShader->use(entity->id);
			material->bind();
			gpuMesh->draw(entity->id);
		}
	}
};