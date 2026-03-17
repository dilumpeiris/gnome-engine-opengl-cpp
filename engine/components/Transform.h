#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "ECS.h"
#include "types.h"

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
	void printMats() {
		std::cout << "View: " << std::endl;
		for (int row = 0; row < 4; row++) {
			for (int col = 0; col < 4; col++) {
				std::cout << view[col][row]; // [column][row] — column-major!
				if (col < 3)
					std::cout << "\t";
			}
			std::cout << "\n";
		}
		std::cout << "\n";
		std::cout << "Model: " << std::endl;
		for (int row = 0; row < 4; row++) {
			for (int col = 0; col < 4; col++) {
				std::cout << model[col][row]; // [column][row] — column-major!
				if (col < 3)
					std::cout << "\t";
			}
			std::cout << "\n";
		}
		std::cout << "\n";
		std::cout << "Projection: " << std::endl;
		for (int row = 0; row < 4; row++) {
			for (int col = 0; col < 4; col++) {
				std::cout << projection[col][row]; // [column][row] — column-major!
				if (col < 3)
					std::cout << "\t";
			}
			std::cout << "\n";
		}
		std::cout << "\n";
	}
	void update() override {
		glUniformMatrix4fv(shader_model, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(shader_view, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(shader_projection, 1, GL_FALSE, glm::value_ptr(projection));
	}

	void translate(float x, float y, float z) { view = glm::translate(view, glm::vec3(x, y, z)); }

	void translateAbsolute(float x, float y, float z) {
		view[3][0] += x;
		view[3][1] += y;
		view[3][2] += z;
	}

	void rotate(float angle, float x, float y, float z) {
		view = glm::rotate(view, glm::radians(angle), glm::vec3(x, y, z));
	}

	void rotateAbsolute(float angle, float x, float y, float z) {
		glm::vec4 pos = view[3];
		view[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		glm::mat4 rot = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(x, y, z));
		view = rot * view;
		view[3] = pos;
	}

	void scale(float x, float y, float z) { view = glm::scale(view, glm::vec3(x, y, z)); }

	void setPosition(float x, float y, float z) {
		view = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
	}

	void setRotation(float angle, float x, float y, float z) {
		view = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(x, y, z));
	}

	void setScale(float x, float y, float z) {
		view = glm::scale(glm::mat4(1.0f), glm::vec3(x, y, z));
	}

	Vector3 getPosition() { return Vector3{view[3][0], view[3][1], view[3][2]}; }
	Vector3 getRotation() { return Vector3{view[2][0], view[2][1], view[2][2]}; }
	Vector3 getScale() { return Vector3{view[1][0], view[1][1], view[1][2]}; }
};
