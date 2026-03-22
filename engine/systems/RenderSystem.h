#pragma once
#include "Animation.h"
#include "ecs/ECS.h"

#include "components/Mesh.h"
#include "components/Material.h"
#include "components/Shader.h"
#include "components/Transform.h"
#include "components/Animation.h"

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

			// for (auto &texture : material->texture_items) {
			// 	gpuTexture->addTexture(entity->id, texture.name, texture.filePath);
			// }

			for (auto &animation : entity->getComponent<Animation>()->animations) {
				for (auto &frame : animation.frames) {
					gpuTexture->addTexture3D(entity->id, animation.name.c_str(), frame.filePath);
				}
			}

			gpuShader->addShaderVariable(entity->id, "model");
			gpuShader->addShaderVariable(entity->id, "view");
			gpuShader->addShaderVariable(entity->id, "projection");

			gpuShader->addShaderVariable(entity->id, "isAnimated");
			gpuShader->addShaderVariable(entity->id, "frameIndex");

			gpuShader->setShaderData(entity->id, "isAnimated", 1);
			gpuShader->setShaderData(entity->id, "frameIndex", 1);

			// gpuShader->setActiveTexture(entity->id, material->getCurrentTextureLocation());
			gpuTexture->printTextureNames(entity->id);
		}
	}

	void update() override {
		for (auto &entity : this->manager->entities) {

			Transform *transform = entity->getComponent<Transform>();
			Material *material = entity->getComponent<Material>();
			Shader *shader = entity->getComponent<Shader>();
			Animation *animation = entity->getComponent<Animation>();

			gpuShader->use(entity->id);

			gpuShader->setShaderMatrix(entity->id, "model", transform->model);
			gpuShader->setShaderMatrix(entity->id, "view", transform->view);
			gpuShader->setShaderMatrix(entity->id, "projection", transform->projection);

			// Bind every texture to a slot.
			gpuTexture->bind(entity->id);

			// Move the mainTexture to the slot asked by the material.
			gpuShader->setShaderTexture(entity->id, "mainTexture", 1);
			gpuShader->setShaderTexture(entity->id, "spriteArray", 0);

			gpuShader->setShaderData(entity->id, "isAnimated", true);
			gpuShader->setShaderData(entity->id, "frameIndex",
			                         animation->currentAnimation.currentFrameIndex);

			gpuMesh->draw(entity->id);
		}
	}
};