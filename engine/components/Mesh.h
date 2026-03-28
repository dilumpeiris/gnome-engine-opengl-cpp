#pragma once
#include "ecs/ECS.h"
#include "meshes/GRectangleMesh.h"
#include "assets/MeshAsset.h"

// =====================================================================================================
// Mesh Component
// =====================================================================================================
class Mesh : public Component {
  public:
	bool collide;
	bool visible;
	int renderMode;

	float *vertices;
	int *indices;

	int verticesCount;
	int indicesCount;

	MeshAsset asset;

  public:
	Mesh() : collide(false), visible(true), renderMode(4) {
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
};