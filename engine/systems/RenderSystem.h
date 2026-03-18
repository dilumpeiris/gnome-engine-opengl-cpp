#pragma once
#include "components/Mesh.h"
#include "components/Material.h"
#include "components/Shader.h"
#include "components/Transform.h"
#include "ecs/ECS.h"
#include "renderer/OpenGLRenderer/OpenGLMesh.h"
#include "renderer/OpenGLRenderer/OpenGLShader.h"
#include "renderer/OpenGLRenderer/OpenGLTexture.h"

class RenderSystem : public System {
  public:
	GPUMesh *gpuMesh;
	GPUShader *gpuShader;
	GPUTexture *gpuTexture;

	void init() override {
		gpuMesh = new OpenGLMesh();
		gpuMesh->init();
		gpuShader = new OpenGLShader();
		gpuShader->init();
		gpuTexture = new OpenGLTexture();
		gpuTexture->init();

		for (auto &entity : this->manager->entities) {
			Transform *transform = entity->getComponent<Transform>();
			Material *material = entity->getComponent<Material>();
			Shader *shader = entity->getComponent<Shader>();
			Mesh *mesh = entity->getComponent<Mesh>();

			gpuMesh->addEntity(entity->id, mesh->vertices, mesh->indices, mesh->verticesCount,
			                   mesh->indicesCount);
			gpuShader->addShader(entity->id, shader->vertexSrc, shader->fragmentSrc);
			gpuTexture->addTexture(entity->id, material->texture_items[0].filePath);

			// This should not exist after delocating GPU logic in the components.
			transform->setShader(gpuShader->getShaderID(entity->id));
			gpuTexture->setShaderTextures(entity->id, gpuShader->getShaderID(entity->id));
		}
	}
	void update() override {
		for (auto &entity : this->manager->entities) {

			Transform *transform = entity->getComponent<Transform>();
			Material *material = entity->getComponent<Material>();
			Shader *shader = entity->getComponent<Shader>();

			gpuShader->use(entity->id);
			gpuTexture->use(entity->id);
			gpuMesh->draw(entity->id);
		}
	}
};