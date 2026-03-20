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

		for (auto &entity : this->manager->entities) {

			Transform *transform = entity->getComponent<Transform>();
			Material *material = entity->getComponent<Material>();
			Shader *shader = entity->getComponent<Shader>();
			Mesh *mesh = entity->getComponent<Mesh>();

			gpuTexture->init(entity->id);

			gpuMesh->addEntity(entity->id, mesh->vertices, mesh->indices, mesh->verticesCount,
			                   mesh->indicesCount);

			gpuShader->addShader(entity->id, shader->vertexSrc, shader->fragmentSrc);

			for (auto &texture : material->texture_items) {
				gpuTexture->addTexture(entity->id, texture.name, texture.filePath);
			}

			gpuShader->addShaderVariable(entity->id, "model");
			gpuShader->addShaderVariable(entity->id, "view");
			gpuShader->addShaderVariable(entity->id, "projection");

			gpuShader->setActiveTexture(entity->id, material->getCurrentTextureLocation());
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

			gpuShader->setActiveTexture(entity->id, material->getCurrentTextureLocation());

			// Bind every texture to a slot.
			gpuTexture->bind(entity->id);

			// Move the mainTexture to the slot asked by the material.
			gpuShader->setShaderTexture(entity->id, material->getCurrentTextureLocation());

			gpuShader->use(entity->id);
			gpuMesh->draw(entity->id);
		}
	}
};