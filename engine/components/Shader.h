#pragma once
#include "ecs/ECS.h"

#include "shaders/GRectangle/Fragment.h"
#include "shaders/GRectangle/Vertex.h"
#include <OpenGL/gltypes.h>

// =====================================================================================================
// Shader Structs
// =====================================================================================================

struct ShaderPass {
	const char *vertexSrc;
	const char *fragmentSrc;
	bool depthWrite;
	bool depthTest;
	GLenum blendSrc, blendDst;
};

// =====================================================================================================
// Shader Component
// =====================================================================================================
class Shader : public Component {
  public:
	const char *vertexSrc;
	const char *fragmentSrc;

	std::vector<ShaderPass> passes;

  public:
	Shader() {
		this->vertexSrc = GRectShader::vertexSrc;
		this->fragmentSrc = GRectShader::fragmentSrc;
	}

	void update() override {}

	void addPass(const char *vertexSrc, const char *fragmentSrc, bool depthWrite, bool depthTest,
	             GLenum blendSrc, GLenum blendDst) {
		passes.push_back({vertexSrc, fragmentSrc, depthWrite, depthTest, blendSrc, blendDst});
	}
};
