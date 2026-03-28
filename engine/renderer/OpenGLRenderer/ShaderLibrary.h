#pragma once

#include <string>
#include <unordered_map>
#include <glad/glad.h>
#include <glm/glm.hpp>

struct GPUShaderPass {
	GLuint ID;
	std::unordered_map<std::string, int> variables;
};

class ShaderLibrary {
  public:
	std::unordered_map<std::string, GPUShaderPass> shaders;

  public:
	static ShaderLibrary &get() {
		static ShaderLibrary instance;
		return instance;
	}

	GPUShaderPass addShader(std::string name, const char *vertexSrc, const char *fragmentSrc) {
		GPUShaderPass shader;

		unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(vertex_shader, 1, &vertexSrc, NULL);
		glShaderSource(fragment_shader, 1, &fragmentSrc, NULL);

		glCompileShader(vertex_shader);
		glCompileShader(fragment_shader);

		shader.ID = glCreateProgram();

		glAttachShader(shader.ID, vertex_shader);
		glAttachShader(shader.ID, fragment_shader);

		glLinkProgram(shader.ID);

		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		shaders[name] = shader;
		return shader;
	}

	void use(std::string name) { glUseProgram(shaders[name].ID); }

	GPUShaderPass get(std::string name) { return shaders[name]; }

	void setShaderUniformi(std::string name, int value) {
		glUniform1i(shaders[name].variables[name], value);
	}

	void setShaderUniformf(std::string name, float value) {
		glUniform1f(shaders[name].variables[name], value);
	}

	void setShaderUniformVec2(std::string name, glm::vec2 value) {
		glUniform2f(shaders[name].variables[name], value.x, value.y);
	}

	void setShaderUniformVec3(std::string name, glm::vec3 value) {
		glUniform3f(shaders[name].variables[name], value.x, value.y, value.z);
	}

	void setShaderUniformVec4(std::string name, glm::vec4 value) {
		glUniform4f(shaders[name].variables[name], value.x, value.y, value.z, value.w);
	}

	void setShaderUniformMat4(std::string name, glm::mat4 value) {
		glUniformMatrix4fv(shaders[name].variables[name], 1, GL_FALSE, &value[0][0]);
	}
};

#define shaderLibrary ShaderLibrary::get()