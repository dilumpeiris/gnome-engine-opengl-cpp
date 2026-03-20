#pragma once

#include <unordered_map>
#include <glad/glad.h>
#include "renderer/GPUShader.h"

#include <glm/gtc/type_ptr.hpp>

// =====================================================================================================
// OpenGLShader Structs
// =====================================================================================================

struct OpenGLShaderData {
	unsigned int ID;
	const char *vertexShaderSrc;
	const char *fragmentShaderSrc;
	std::unordered_map<const char *, unsigned int> variables;
};

// =====================================================================================================
// OpenGLShader Class
// =====================================================================================================

class OpenGLShader : public GPUShader {
  private:
	std::unordered_map<std::size_t, OpenGLShaderData> shaders;

  public:
	void init() override {}
	void use(std::size_t entityID) override { glUseProgram(shaders[entityID].ID); }
	void addShader(std::size_t entityID, const char *vertexShaderSrc,
	               const char *fragmentShaderSrc) override {

		unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(vertex_shader, 1, &vertexShaderSrc, NULL);
		glShaderSource(fragment_shader, 1, &fragmentShaderSrc, NULL);

		glCompileShader(vertex_shader);
		glCompileShader(fragment_shader);

		unsigned int ID = glCreateProgram();

		glAttachShader(ID, vertex_shader);
		glAttachShader(ID, fragment_shader);

		glLinkProgram(ID);

		shaders[entityID] = OpenGLShaderData{ID, vertexShaderSrc, fragmentShaderSrc};

		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);
	}

	unsigned int getShaderID(std::size_t entityID) override { return shaders[entityID].ID; }

	void addShaderVariable(std::size_t entityID, const char *variable) override {
		shaders[entityID].variables[variable] =
		    glGetUniformLocation(shaders[entityID].ID, variable);
	}

	void setShaderData(std::size_t entityID, const char *variable, float data) override {
		glUniform1f(shaders[entityID].variables[variable], data);
	}

	void setShaderData(std::size_t entityID, const char *variable, int data) override {
		glUniform1i(shaders[entityID].variables[variable], data);
	}

	void setShaderMatrix(std::size_t entityID, const char *variable, glm::mat4 data) override {
		glUniformMatrix4fv(shaders[entityID].variables[variable], 1, GL_FALSE,
		                   glm::value_ptr(data));
	}

	void setShaderTextures(std::size_t entityID) override {
		glUniform1i(glGetUniformLocation(shaders[entityID].ID, "spriteArray"), 0);
	}

	void setActiveTexture(std::size_t entityID, int textureLocation) override {
		glUniform1i(glGetUniformLocation(shaders[entityID].ID, "frameIndex"), textureLocation);
	}

	~OpenGLShader() override = default;
};