#pragma once

#include <iostream>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

  public:
	GRect() : GRect(0, 0, 0, 0) {}
	GRect(int x, int y, int width, int height) : Entity(0) {

		this->addComponent<Shader>();
		this->addComponent<Transform>();
		this->addComponent<Material>();
		this->addComponent<Mesh>();

		transform = this->getComponent<Transform>();
		material = this->getComponent<Material>();
		shader = this->getComponent<Shader>();
	}

	void draw() override {

		if (InputHandler::get().isKeyDown(GLFW_KEY_SPACE)) {
			std::cout << "Space key pressed" << std::endl;
		}
	}
};
