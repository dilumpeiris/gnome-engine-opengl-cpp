#pragma once
#include "ecs/ECS.h"
#include <iostream>
#include <glad/glad.h>

// =====================================================================================================
// Shader Component
// =====================================================================================================
class Shader : public Component {
  public:
	unsigned int ID;

	const char *vertexSrc;
	const char *fragmentSrc;

  public:
	Shader() {
		// m:shader
		vertexSrc = R"(
			#version 330 core
	
			layout (location = 0) in vec3 aPos;
			layout (location = 1) in vec3 aColor;
			layout (location = 3) in vec2 aTexCoord;
	
			out vec3 ourColor;
			out vec2 TexCoord;
	
			uniform mat4 model;
			uniform mat4 view;
			uniform mat4 projection;
	
			uniform mat4 transform;
	
	
			void main() {
	
			gl_Position = projection * view * model * vec4(aPos, 1.0);
	
			ourColor = aColor;
			TexCoord = aTexCoord;
	
			}
		)";

		fragmentSrc = R"(
			#version 330 core
	
			out vec4 FragColor;
			
			in vec3 ourColor;
			in vec2 TexCoord;
	

			uniform sampler2DArray spriteArray;

			uniform int frameIndex;
			void main() {
	
			 FragColor = texture(spriteArray, vec3(TexCoord, frameIndex));
			}
		)";

		useShaders(vertexSrc, fragmentSrc);
		std::cout << "shading";
	}
	void useShaders(const char *vertexShaderSrc, const char *fragmentShaderSrc) {

		unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(vertex_shader, 1, &vertexShaderSrc, NULL);
		glShaderSource(fragment_shader, 1, &fragmentShaderSrc, NULL);

		glCompileShader(vertex_shader);
		glCompileShader(fragment_shader);

		ID = glCreateProgram();

		glAttachShader(ID, vertex_shader);
		glAttachShader(ID, fragment_shader);

		glLinkProgram(ID);

		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);
	}

	void use() { glUseProgram(ID); }
	void update() override {}
};
