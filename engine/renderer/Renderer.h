#pragma once
#include <cstddef>

class Renderer {
  public:
	virtual void init() = 0;

	virtual void draw(std::size_t ID) = 0;

	virtual void addEntity(std::size_t ID, float *vertices, int *indices, int vertexCount,
	                       int indexCount) = 0;
};