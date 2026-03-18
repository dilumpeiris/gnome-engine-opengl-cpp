#pragma once

#include <cstddef>

class GPUShader {
  public:
	virtual void init() = 0;
	virtual void use(std::size_t entityID) = 0;
	virtual void addShader(std::size_t entityID, const char *vertexShaderSrc,
	                       const char *fragmentShaderSrc) = 0;
	virtual unsigned int getShaderID(std::size_t entityID) = 0;
	virtual ~GPUShader() = default;
};