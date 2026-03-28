#pragma once

#include <vector>
#include <glad/glad.h>

struct MeshAsset {
	std::vector<float> vertices;
	std::vector<unsigned int> indices;

	int vertexCount;
	int indexCount;

	GLenum renderMode;
};
