#pragma once

#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "components/Mesh.h"
#include "components/Transform.h"
#include "components/Material.h"
#include "components/Shader.h"
#include "meshes/GLineMesh.h"
#include "shaders/GLine/Vertex.h"
#include "shaders/GLine/Fragment.h"

class GLine : public Entity {
  public:
	Transform *transform;
	Shader *shader;
	Material *material;
	Mesh *mesh;

  public:
	std::pair<glm::vec2, glm::vec2> getLineEndpoints() {
		glm::vec4 p1 = transform->view * glm::vec4(-1.0f, 0.0f, 0.0f, 1.0f);
		glm::vec4 p2 = transform->view * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		return { glm::vec2(p1.x, p1.y), glm::vec2(p2.x, p2.y) };
	}

	void setColor(float r, float g, float b, float a) {
		material->hasColor = true;
		material->color = glm::vec4(r, g, b, a);
	}

	GLine() : Entity(0) {
		this->addComponent<Shader>();
		this->addComponent<Transform>();
		this->addComponent<Material>();
		this->addComponent<Mesh>();

		transform = this->getComponent<Transform>();
		material = this->getComponent<Material>();
		shader = this->getComponent<Shader>();
		mesh = this->getComponent<Mesh>();

		// Set GLine mesh
		mesh->vertices = GLineMesh::vertices;
		mesh->indices = GLineMesh::indices;
		mesh->verticesCount = GLineMesh::verticesCount;
		mesh->indicesCount = GLineMesh::indicesCount;
		mesh->renderMode = 1; // GL_LINES = 1

		// Set Shaders
		shader->vertexSrc = GLineShader::vertexSrc;
		shader->fragmentSrc = GLineShader::fragmentSrc;
	}

	void draw() override {}
};
