#pragma once

#include <cstddef>

#include <glm/glm.hpp>

class GPUShader {
  public:
	virtual void init() = 0;
	virtual void use(std::size_t entityID) = 0;
	virtual void addShader(std::size_t entityID, const char *vertexShaderSrc,
	                       const char *fragmentShaderSrc) = 0;
	virtual unsigned int getShaderID(std::size_t entityID) = 0;
	virtual void addShaderVariable(std::size_t entityID, const char *variable) = 0;
	virtual void setShaderData(std::size_t entityID, const char *variable, float data) = 0;
	virtual void setShaderData(std::size_t entityID, const char *variable, int data) = 0;
	virtual void setShaderMatrix(std::size_t entityID, const char *variable, glm::mat4 data) = 0;
	virtual void setShaderTexture(std::size_t entityID, int textureLocation) = 0;
	virtual void setActiveTexture(std::size_t entityID, int textureLocation) = 0;
	virtual ~GPUShader() = default;
};