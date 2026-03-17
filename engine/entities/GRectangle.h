#pragma once

#include <iostream>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "components/Transform.h"
#include "components/Material.h"
#include "components/Shader.h"
#include "input/InputHandler.h"

// =====================================================================================================
// GRectangle
// =====================================================================================================
class GRect : public Entity {
  public:
	unsigned int VAO;

	Transform *transform;
	Shader *shader;
	Material *material;

  public:
	GRect() : GRect(0, 0, 0, 0) {}
	GRect(int x, int y, int width, int height) : Entity(0), transform(0) {
		int screen_width = 1024;
		int screen_height = 768;

		// glfwGetFramebufferSize(window, &width, &height);
		float pos_x, pos_y;
		float size_x, size_y;

		pos_x = x / (screen_width / 2.0f);
		pos_y = y / (screen_height / 2.0f);
		size_x = width / (screen_width / 2.0f);
		size_y = height / (screen_height / 2.0f);

		float vertices[] = {
		    // positions // colors // texture coords
		    size_x / 2.0f,
		    size_y / 2.0f,
		    0.0f,
		    1.0f,
		    0.0f,
		    0.0f,
		    1.0f,
		    1.0f, // top right

		    size_x / 2.0f,
		    -size_y / 2.0f,
		    0.0f,
		    0.0f,
		    1.0f,
		    0.0f,
		    1.0f,
		    0.0f, // bottom right

		    -size_x / 2.0f,
		    -size_y / 2.0f,
		    0.0f,
		    0.0f,
		    0.0f,
		    1.0f,
		    0.0f,
		    0.0f, // bottom left

		    -size_x / 2.0f,
		    size_y / 2.0f,
		    0.0f,
		    1.0f,
		    1.0f,
		    0.0f,
		    0.0f,
		    1.0f // top left
		};

		int indices[] = {0, 1, 3, 1, 2, 3};

		unsigned int VBO;
		unsigned int EBO;

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// Position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
		glEnableVertexAttribArray(0);

		// Color
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
		                      (void *)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		// Material coordinates
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
		                      (void *)(6 * sizeof(float)));
		glEnableVertexAttribArray(3);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		this->addComponent<Shader>();
		this->addComponent<Transform>(this->getComponent<Shader>()->ID);
		this->addComponent<Material>();

		transform = this->getComponent<Transform>();
		material = this->getComponent<Material>();
		shader = this->getComponent<Shader>();

		transform->translate(pos_x, pos_y, 0.0f);
		material->setShaderTextures(shader->ID);
	}

	void draw() override {
		shader->use();

		material->bind();

		transform->update();

		glBindVertexArray(VAO);
		glUniform1i(glGetUniformLocation(shader->ID, "frameIndex"), 0);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		if (InputHandler::get().isKeyDown(GLFW_KEY_SPACE)) {
			std::cout << "Space key pressed" << std::endl;
		}
	}
};
