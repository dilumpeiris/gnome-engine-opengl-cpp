#pragma once

#include <iostream>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Animation.h"
#include "components/Mesh.h"
#include "components/Transform.h"
#include "components/Material.h"
#include "components/Shader.h"
#include "input/InputHandler.h"

// =====================================================================================================
// GRectangle
// =====================================================================================================
class GRect : public Entity {
  public:
	Transform *transform;
	Shader *shader;
	Material *material;
	Mesh *mesh;
	Animation *animation;

  public:
	std::pair<glm::vec2, glm::vec2> getLineEndpoints() {
		glm::vec4 p1 = transform->view * glm::vec4(-1.0f, 0.0f, 0.0f, 1.0f);
		glm::vec4 p2 = transform->view * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		return { glm::vec2(p1.x, p1.y), glm::vec2(p2.x, p2.y) };
	}

	void setColor(float r, float g, float b, float a) {
		material->materials[material->getCurrentMaterialIndex()].color = glm::vec4(r, g, b, a);
	}

	GRect() : GRect(0, 0, 0, 0) {}
	GRect(int x, int y, int width, int height) : Entity(0) {

		this->addComponent<Shader>();
		this->addComponent<Transform>();
		this->addComponent<Material>();
		this->addComponent<Mesh>();
		this->addComponent<Animation>();

		transform = this->getComponent<Transform>();
		material = this->getComponent<Material>();
		shader = this->getComponent<Shader>();
		mesh = this->getComponent<Mesh>();
		animation = this->getComponent<Animation>();
	}

	void draw() override {}
};
