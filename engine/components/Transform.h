#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ECS.h"
#include "types.h"

// =====================================================================================================
// Transform Component
// =====================================================================================================

class Transform : public Component {
  public:
	glm::mat4 position;
	glm::mat4 rotation;
	// glm::mat4 scale;

	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;

  public:
	void init() override {
		model = glm::mat4(1.0f);
		view = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f), 1024.0f / 768.0f, 0.1f, 100.0f);

		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	}
	void update() override {}

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
