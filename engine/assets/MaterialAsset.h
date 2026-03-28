#pragma once

#include <vector>
#include <glad/glad.h>

struct ShaderPass {
	const char *vertexSrc;
	const char *fragmentSrc;

	bool depthTest;
	bool depthWrite;
	GLenum blendSrc, blendDst;
};

struct MaterialAsset {
	std::vector<ShaderPass> passes;
};
