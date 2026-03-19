#pragma once
#include "ecs/ECS.h"

#include "components/Mesh.h"
#include "components/Material.h"
#include "components/Shader.h"
#include "components/Transform.h"

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
			gpuTexture->setShaderTextures(entity->id, gpuShader->getShaderID(entity->id));

			gpuShader->addShaderVariable(entity->id, "model");
			gpuShader->addShaderVariable(entity->id, "view");
			gpuShader->addShaderVariable(entity->id, "projection");
		}
	}

	void update() override {
		for (auto &entity : this->manager->entities) {

			Transform *transform = entity->getComponent<Transform>();
			Material *material = entity->getComponent<Material>();
			Shader *shader = entity->getComponent<Shader>();

			gpuShader->setShaderMatrix(entity->id, "model", transform->model);
			gpuShader->setShaderMatrix(entity->id, "view", transform->view);
			gpuShader->setShaderMatrix(entity->id, "projection", transform->projection);

			gpuShader->use(entity->id);
			gpuTexture->use(entity->id);
			gpuMesh->draw(entity->id);
		}
	}
};