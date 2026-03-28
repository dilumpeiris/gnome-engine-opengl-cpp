#pragma once

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "./TextureAsset.h"
#include <string>

struct ShaderPass {
	const char *vertexSrc;
	const char *fragmentSrc;
	std::string name;
	bool depthTest;
	bool depthWrite;
	GLenum blendSrc, blendDst;
};

struct MaterialAsset {

	std::string name;

	std::vector<ShaderPass> passes;

	// textures mapped to shader uniform slots
	TextureAsset albedo;    // u_albedo     (diffuse/color map)
	TextureAsset normal;    // u_normal     (normal map)
	TextureAsset roughness; // u_roughness  (roughness map)

	glm::vec4 color;

	void addShaderPass(std::string name, const char *vertexSrc, const char *fragmentSrc) {
		passes.push_back({vertexSrc, fragmentSrc, name, true, true, GL_ONE, GL_ZERO});
	}
};
