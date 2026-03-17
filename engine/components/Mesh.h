#pragma once
#include "ecs/ECS.h"

class Mesh : public Component {
  private:
	bool collide;
	bool visible;

	float *vertices;
	float *indices;
	int verticesCount;
	int indicesCount;

  public:
	Mesh() : collide(false), visible(true) {}
	Mesh(float *vertices, float *indices, int verticesCount, int indicesCount)
	    : vertices(vertices), indices(indices), verticesCount(verticesCount),
	      indicesCount(indicesCount) {}

	void setCollide(bool collide) { this->collide = collide; }
	void setVisible(bool visible) { this->visible = visible; }
	bool getCollide() { return collide; }
	bool getVisible() { return visible; }
};