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

		asset.vertices.assign(GRectangleMesh::vertices,
		                      GRectangleMesh::vertices + GRectangleMesh::verticesCount);
		asset.indices.assign(GRectangleMesh::indices,
		                     GRectangleMesh::indices + GRectangleMesh::indicesCount);
		asset.vertexCount = GRectangleMesh::verticesCount;
		asset.indexCount = GRectangleMesh::indicesCount;
		asset.renderMode = GL_TRIANGLES;
	}

	void setCollide(bool collide) { this->collide = collide; }
	void setVisible(bool visible) { this->visible = visible; }
	bool getCollide() { return collide; }
	bool getVisible() { return visible; }

	void setShape(float *vertices, int *indices, int verticesCount, int indicesCount);
};