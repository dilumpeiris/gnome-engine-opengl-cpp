#pragma once
#include "ecs/ECS.h"

#include "shaders/GRectangle/Fragment.h"
#include "shaders/GRectangle/Vertex.h"
#include <OpenGL/gltypes.h>

// =====================================================================================================
// Shader Structs
// =====================================================================================================

// =====================================================================================================
// Shader Component
// =====================================================================================================
class Shader : public Component {
  public:
	const char *vertexSrc;
	const char *fragmentSrc;

  public:
	Shader() {
		this->vertexSrc = GRectShader::vertexSrc;
		this->fragmentSrc = GRectShader::fragmentSrc;
	}

	void update() override {}
};
