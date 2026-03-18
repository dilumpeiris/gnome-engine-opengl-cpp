#pragma once
#include "renderer/GPUMesh.h"
#include <unordered_map>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct OpenGLObject {
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
};

class OpenGLMesh : public GPUMesh {
  private:
	std::unordered_map<std::size_t, OpenGLObject> objects;

  public:
	void init() override {}

	void draw(std::size_t ID) override {
		glBindVertexArray(objects[ID].VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void addEntity(std::size_t ID, float *vertices, int *indices, int vertexCount,
	               int indexCount) override {
		unsigned int VAO;
		unsigned int VBO;
		unsigned int EBO;

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(int), indices, GL_STATIC_DRAW);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(float), vertices, GL_STATIC_DRAW);

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

		objects[ID] = OpenGLObject{VAO, VBO, EBO};
	}
};
