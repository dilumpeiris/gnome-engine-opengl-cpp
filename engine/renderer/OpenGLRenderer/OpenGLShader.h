#pragma once

#include <unordered_map>
#include <glad/glad.h>
#include "renderer/GPUShader.h"

struct OpenGLShaderData {
	unsigned int ID;
	const char *vertexShaderSrc;
	const char *fragmentShaderSrc;
};

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

	// Maybe use a template here.
	void setShaderData(std::size_t entityID, char *variable, float data) override {
		glUniform1f(glGetUniformLocation(shaders[entityID].ID, variable), data);
	}

	~OpenGLShader() override = default;
};