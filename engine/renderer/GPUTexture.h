#pragma once

#include <cstddef>

class GPUTexture {
  public:
	virtual void init() = 0;
	virtual void use(std::size_t textureID) = 0;
	virtual void addTexture(std::size_t entityID, const char *filePath) = 0;
	virtual void setShaderTextures(std::size_t entityID, unsigned int shaderID) = 0;
	virtual ~GPUTexture() = default;
};