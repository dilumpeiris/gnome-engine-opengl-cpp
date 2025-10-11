#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ECS.h"

// =====================================================================================================
// Transform Component
// =====================================================================================================

class Transform : public Component {
  public:
	unsigned int shader;

	glm::mat4 position;
	glm::mat4 rotation;
	// glm::mat4 scale;

	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;

	unsigned int shaderID;

	unsigned int shader_model;
	unsigned int shader_view;
	unsigned int shader_projection;

  public:
	Transform(unsigned int shaderID) {

		this->shaderID = shaderID;

		shader_model = glGetUniformLocation(shaderID, "model");
		shader_view = glGetUniformLocation(shaderID, "view");
		shader_projection = glGetUniformLocation(shaderID, "projection");

		model = glm::mat4(1.0f);
		view = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f), 1024.0f / 768.0f, 0.1f, 100.0f);

		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	}

	void update() override {
		glUniformMatrix4fv(shader_model, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(shader_view, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(shader_projection, 1, GL_FALSE, glm::value_ptr(projection));
	}

	void translate(float x, float y, float z) { view = glm::translate(view, glm::vec3(x, y, z)); }

	void rotate(float angle, float x, float y, float z) {}

	void scale(float x, float y, float z) { view = glm::translate(view, glm::vec3(x, y, z)); }
};
