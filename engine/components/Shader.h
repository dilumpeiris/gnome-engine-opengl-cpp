#pragma once
#include "ecs/ECS.h"
#include <iostream>
#include <glad/glad.h>

#include "shaders/GRectangle/Vertex.h"
#include "shaders/GRectangle/Fragment.h"

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
