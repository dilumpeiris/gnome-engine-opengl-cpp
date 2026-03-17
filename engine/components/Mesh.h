#pragma once
#include "GRectangleMesh.h"
#include "ecs/ECS.h"

class Mesh : public Component {
  public:
	bool collide;
	bool visible;

	float *vertices;
	int *indices;

	int verticesCount;
	int indicesCount;

  public:
	Mesh() : collide(false), visible(true) {
		this->vertices = GRectangleMesh::vertices;
		this->indices = GRectangleMesh::indices;
		this->verticesCount = GRectangleMesh::verticesCount;
		this->indicesCount = GRectangleMesh::indicesCount;
	}

	void setCollide(bool collide) { this->collide = collide; }
	void setVisible(bool visible) { this->visible = visible; }
	bool getCollide() { return collide; }
	bool getVisible() { return visible; }

	void setShape(float *vertices, int *indices, int verticesCount, int indicesCount);
	void loadShapeFromFile(const std::string &filename);
};