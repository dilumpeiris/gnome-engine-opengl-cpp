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
			Animation *animation = entity->getComponent<Animation>();

			gpuTexture->init(entity->id);

			gpuMesh->addEntity(entity->id, mesh->vertices, mesh->indices, mesh->verticesCount,
			                   mesh->indicesCount, mesh->renderMode);

			gpuShader->addShader(entity->id, shader->vertexSrc, shader->fragmentSrc);

			for (auto &texture : material->texture_items) {
				gpuTexture->addTexture(entity->id, texture.name, texture.filePath);
			}

			if (animation) {
				for (auto &anim : animation->animations) {
					for (auto &frame : anim.frames) {
						gpuTexture->addTexture3D(entity->id, anim.name.c_str(), frame.filePath);
					}
				}
			}

			gpuShader->addShaderVariable(entity->id, "model");
			gpuShader->addShaderVariable(entity->id, "view");
			gpuShader->addShaderVariable(entity->id, "projection");

			gpuShader->addShaderVariable(entity->id, "isAnimated");
			gpuShader->addShaderVariable(entity->id, "frameIndex");
			gpuShader->addShaderVariable(entity->id, "hasColor");
			gpuShader->addShaderVariable(entity->id, "solidColor");

			gpuShader->setShaderData(entity->id, "isAnimated", 1);
			gpuShader->setShaderData(entity->id, "frameIndex", 1);

			gpuShader->setActiveTexture(entity->id, material->getCurrentTextureLocation());
		}
	}

	void update() override {
		for (auto &entity : this->manager->entities) {

			Transform *transform = entity->getComponent<Transform>();
			Material *material = entity->getComponent<Material>();
			Animation *animation = entity->getComponent<Animation>();

			gpuShader->use(entity->id);

			gpuShader->setShaderMatrix(entity->id, "model", transform->model);
			gpuShader->setShaderMatrix(entity->id, "view", transform->view);
			gpuShader->setShaderMatrix(entity->id, "projection", transform->projection);

			// Bind every texture to a slot. This should be fixed so the 2D and 3D textures do not
			// collide.
			// gpuTexture->bind(entity->id);

			if (!material->texture_items.empty()) {
				gpuTexture->use(entity->id, material->getCurrentTextureName());
			}
			if (animation && !animation->animations.empty()) {
				gpuTexture->use(entity->id, animation->currentAnimation.name.c_str());
			}

			// Move the mainTexture to the slot asked by the material.
			// Maybe these should be implicit.
			gpuShader->setShaderTexture(entity->id, "mainTexture", 0);
			gpuShader->setShaderTexture(entity->id, "spriteArray", 1);

			if (material->hasColor) {
				gpuShader->setShaderData(entity->id, "hasColor", true);
				gpuShader->setShaderVector(entity->id, "solidColor", material->color);
				gpuShader->setShaderData(entity->id, "isAnimated", false);
			} else {
				gpuShader->setShaderData(entity->id, "hasColor", false);
				if (animation && animation->isPlaying) {
					gpuShader->setShaderData(entity->id, "isAnimated", true);
					gpuShader->setShaderData(entity->id, "frameIndex",
					                         animation->currentAnimation.currentFrameIndex);
				} else {
					gpuShader->setShaderData(entity->id, "isAnimated", false);
					gpuShader->setShaderData(entity->id, "frameIndex", 0);
				}
			}

			gpuMesh->draw(entity->id);
		}
	}
};